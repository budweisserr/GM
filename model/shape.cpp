#include "shape.h"

void Shape::computeShape(int &gridSize, double &pixelsPerCm) {
    m_points.clear();
    m_pointsLeftLower.clear();
    m_pointsLeftUpper.clear();
    m_pointsRightLower.clear();
    m_pointsRightUpper.clear();
    m_points.resize(0);
    m_pointsLeftLower.resize(0);
    m_pointsLeftUpper.resize(0);
    m_pointsRightLower.resize(0);
    m_pointsRightUpper.resize(0);

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

Line Shape::computeTangent(Point& point) const {
    for (const auto& tangentLine : m_tangentLines) {
        if (tangentLine.start().x() == point.x() && tangentLine.start().y() == point.y()) {
            return tangentLine;
        }
    }
    return Line(Point(0, 0), Point(0, 0));
}

Line Shape::computeNormal(Point& point) const {
    for (const auto& normalLine : m_normalLines) {
        if (normalLine.start().x() == point.x() && normalLine.start().y() == point.y()) {
            return normalLine;
        }
    }
    return Line(Point(0, 0), Point(0, 0));
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

void Shape::calculateTangentAndNormalLines() {
    m_tangentLines.clear();
    m_normalLines.clear();

    for (size_t i = 0; i < m_points.size() - 1; ++i) {
        const Point& currentPoint = m_points[i];
        const Point& nextPoint = m_points[i + 1];

        double dx = nextPoint.x() - currentPoint.x();
        double dy = nextPoint.y() - currentPoint.y();
        double tangentLength = sqrt(dx * dx + dy * dy);

        double tangentUnitX = dx / tangentLength;
        double tangentUnitY = dy / tangentLength;

        double scaledTangentX = tangentUnitX * 100;
        double scaledTangentY = tangentUnitY * 100;

        double normalUnitX = -tangentUnitY;
        double normalUnitY = tangentUnitX;

        double scaledNormalX = normalUnitX * 100;
        double scaledNormalY = normalUnitY * 100;

        Point tangentStart = currentPoint;
        Point tangentEnd(
            currentPoint.x() + scaledTangentX,
            currentPoint.y() + scaledTangentY
        );

        Point normalStart = currentPoint;
        Point normalEnd(
            currentPoint.x() + scaledNormalX,
            currentPoint.y() + scaledNormalY
        );

        m_tangentLines.emplace_back(tangentStart, tangentEnd);
        m_normalLines.emplace_back(normalStart, normalEnd);
    }
}


