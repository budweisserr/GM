#include "shape.h"

void Shape::computeShape(int &gridSize, double &pixelsPerCm) {
    m_points.clear();
    m_pointsLeftLower.clear();
    m_pointsLeftUpper.clear();
    m_pointsRightLower.clear();
    m_pointsRightUpper.clear();

    double A = a * a + b * b - d * d;
    double B = b * b - a * a - d * d;
    double C = B * B - 4 * a * a * d * d;

    for (double x = -1000; x <= 1000; x += step) {
        if (x * x <= A + sqrt(A * A - C) && B - x * x + sqrt(2 * (A - B) * x * x + B * B - C) >= 0) {
            double y = sqrt(B - x * x + sqrt(2 * (A - B) * x * x + B * B - C));
            if (x < 0) {
                m_pointsLeftUpper.emplace_back(x * pixelsPerCm, y * pixelsPerCm);
                m_pointsLeftLower.emplace_back(x * pixelsPerCm, -y * pixelsPerCm);
            } else if (x > 0) {
                m_pointsRightUpper.emplace_back(x * pixelsPerCm, y * pixelsPerCm);
                m_pointsRightLower.emplace_back(x * pixelsPerCm, -y * pixelsPerCm);
            }else {
                m_pointsLeftUpper.emplace_back(x * pixelsPerCm, y * pixelsPerCm);
                m_pointsLeftLower.emplace_back(x * pixelsPerCm, -y * pixelsPerCm);
                m_pointsRightUpper.emplace_back(x * pixelsPerCm, y * pixelsPerCm);
                m_pointsRightLower.emplace_back(x * pixelsPerCm, -y * pixelsPerCm);
            }
        }
    }
    m_points.insert(m_points.end(), m_pointsLeftUpper.begin(), m_pointsLeftUpper.end());
    m_points.insert(m_points.end(), m_pointsLeftLower.begin(), m_pointsLeftLower.end());
    m_points.insert(m_points.end(), m_pointsRightLower.begin(), m_pointsRightLower.end());
    m_points.insert(m_points.end(), m_pointsRightUpper.begin(), m_pointsRightUpper.end());
}

void Shape::draw(QPainter *painter) {
    painter->setPen(QPen(Qt::black, 2));
    for (int i = 1; i < m_pointsLeftLower.size(); i++) {
        painter->drawLine(Point::toQPoint(m_pointsLeftLower[i - 1]), Point::toQPoint(m_pointsLeftLower[i]));
    }
    for (int i = 1; i < m_pointsLeftUpper.size(); i++) {
        painter->drawLine(Point::toQPoint(m_pointsLeftUpper[i - 1]), Point::toQPoint(m_pointsLeftUpper[i]));
    }
    for (int i = 1; i < m_pointsRightLower.size(); i++) {
        painter->drawLine(Point::toQPoint(m_pointsRightLower[i - 1]), Point::toQPoint(m_pointsRightLower[i]));
    }
    for (int i = 1; i < m_pointsRightUpper.size(); i++) {
        painter->drawLine(Point::toQPoint(m_pointsRightUpper[i - 1]), Point::toQPoint(m_pointsRightUpper[i]));
    }
    painter->drawLine(Point::toQPoint(m_pointsLeftLower[0]), Point::toQPoint(m_pointsLeftUpper[0]));
    painter->drawLine(Point::toQPoint(m_pointsRightLower[m_pointsRightLower.size() - 1]), Point::toQPoint(m_pointsRightUpper[m_pointsRightUpper.size() - 1]));
    painter->drawLine(Point::toQPoint(m_pointsLeftLower[m_pointsLeftLower.size() - 1]), Point::toQPoint(m_pointsLeftUpper[m_pointsLeftUpper.size() - 1]));
    painter->drawLine(Point::toQPoint(m_pointsRightLower[0]), Point::toQPoint(m_pointsRightUpper[0]));
}

void Shape::transform(double **matrix) {
    for (auto& point : m_pointsLeftUpper) {
        point.transform(matrix);
    }
    for (auto& point : m_pointsLeftLower) {
        point.transform(matrix);
    }
    for (auto& point : m_pointsRightLower) {
        point.transform(matrix);
    }
    for (auto& point : m_pointsRightUpper) {
        point.transform(matrix);
    }
}

double Shape::computeDerivativeX(double x, double y) const {
    return 4 * (x*x + y*y) * x - 2 * a * a + b * b - d * d * x;
}

double Shape::computeDerivativeY(double x, double y) const {
    return 4 * (x*x + y*y) * y - 2 * b * b - a * a - d * d * y;
}

Line Shape::computeTangent(Point& point) const {
    double derivativeX = computeDerivativeX(point.x(), point.y());
    double derivativeY = computeDerivativeY(point.x(), point.y());

    // Normalize the derivative
    double magnitude = std::sqrt(derivativeX * derivativeX + derivativeY * derivativeY);
    double normalizedDerivativeX = derivativeX / magnitude;
    double normalizedDerivativeY = derivativeY / magnitude;

    // Calculate the ends of the tangent line
    double halfLength = 100 / 2;
    Point start(
        point.x() - normalizedDerivativeX * halfLength,
        point.y() - normalizedDerivativeY * halfLength
    );
    Point end(
        point.x() + normalizedDerivativeX * halfLength,
        point.y() + normalizedDerivativeY * halfLength
    );

    return Line(start, end);
}

Line Shape::computeNormal(Point& point) const {
    double derivativeX = computeDerivativeX(point.x(), point.y());
    double derivativeY = computeDerivativeY(point.x(), point.y());

    // Normalize the derivative
    double magnitude = std::sqrt(derivativeX * derivativeX + derivativeY * derivativeY);
    double normalizedDerivativeX = derivativeX / magnitude;
    double normalizedDerivativeY = derivativeY / magnitude;

    // Calculate the ends of the normal line
    double halfLength = 100 / 2;
    Point start(
        point.x() - normalizedDerivativeY * halfLength,
        point.y() + normalizedDerivativeX * halfLength
    );
    Point end(
        point.x() + normalizedDerivativeY * halfLength,
        point.y() - normalizedDerivativeX * halfLength
    );

    return Line(start, end);
}

void Shape::drawNormal(QPainter *painter, Point& point) {
    Line normal = computeNormal(point);
    painter->setPen(QPen(Qt::red, 2));
    painter->drawLine(Point::toQPoint(normal.start()), Point::toQPoint(normal.end()));
}

void Shape::drawTangent(QPainter *painter, Point& point) {
    Line tangent = computeTangent(point);
    painter->setPen(QPen(Qt::blue, 2));
    painter->drawLine(Point::toQPoint(tangent.start()), Point::toQPoint(tangent.end()));
}

void Shape::drawPoint(QPainter *painter, Point& point) {
    painter->setPen(QPen(Qt::green, 5));
    painter->drawPoint(Point::toQPoint(point));
}


