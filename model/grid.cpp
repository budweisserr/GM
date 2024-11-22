// grid.cpp
#include "grid.h"
#include <QPainter>

void CoordinateGrid::computeGrid(int& gridSize, double& pixelsPerCm) {
    computeMainAxes(gridSize, pixelsPerCm);
    computeGridLines(gridSize, pixelsPerCm);
}

void CoordinateGrid::computeMainAxes(const int& gridSize, const double& pixelsPerCm) {
    m_xAxis = Line(Point(0, 0), Point(gridSize * pixelsPerCm, 0));
    m_yAxis = Line(Point(0, 0), Point(0, gridSize * pixelsPerCm));
}

void CoordinateGrid::computeGridLines(const int &gridSize, const double &pixelsPerCm) {
    m_gridLines.clear();
    for (int i = 1; i <= gridSize; ++i) {
        double x = i * pixelsPerCm;
        m_gridLines.emplace_back(Point(x, 0), Point(x, gridSize * pixelsPerCm));
        double y = i * pixelsPerCm;
        m_gridLines.emplace_back(Point(0, y), Point(gridSize * pixelsPerCm, y));
    }
}

void CoordinateGrid::transform(double **matrix) {
    for (auto& line : m_gridLines) {
        line.transform(matrix);
    }
    m_xAxis.transform(matrix);
    m_yAxis.transform(matrix);
}

void CoordinateGrid::draw(QPainter *painter) {
    painter->setPen(QPen(Qt::lightGray, 1, Qt::SolidLine));
    for (const auto& line : m_gridLines) {
        QPoint start = Point::toQPoint(line.start());
        QPoint end = Point::toQPoint(line.end());
        painter->drawLine(start, end);
    }

    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter->drawLine(Point::toQPoint(m_xAxis.start()), Point::toQPoint(m_xAxis.end()));
    painter->drawLine(Point::toQPoint(m_yAxis.start()), Point::toQPoint(m_yAxis.end()));
}

QRectF CoordinateGrid::boundingRect() const {
    if (m_gridLines.empty()) {
        return QRectF(0, 0, 0, 0);
    }

    // Initialize bounding box with the first line
    double minX = m_gridLines[0].start().x();
    double maxX = m_gridLines[0].start().x();
    double minY = m_gridLines[0].start().y();
    double maxY = m_gridLines[0].start().y();

    // Expand bounding box to include all grid lines
    for (const auto& line : m_gridLines) {
        minX = std::min({minX, line.start().x(), line.end().x()});
        maxX = std::max({maxX, line.start().x(), line.end().x()});
        minY = std::min({minY, line.start().y(), line.end().y()});
        maxY = std::max({maxY, line.start().y(), line.end().y()});
    }

    // Expand to include main axes
    minX = std::min({minX, m_xAxis.start().x(), m_xAxis.end().x()});
    maxX = std::max({maxX, m_xAxis.start().x(), m_xAxis.end().x()});
    minY = std::min({minY, m_yAxis.start().y(), m_yAxis.end().y()});
    maxY = std::max({maxY, m_yAxis.start().y(), m_yAxis.end().y()});

    // Return the bounding rectangle
    return QRectF(minX, minY, maxX - minX, maxY - minY);
}



