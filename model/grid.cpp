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




