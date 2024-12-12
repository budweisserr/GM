// grid.cpp
#include "grid.h"
#include <QPainter>

void CoordinateGrid::computeGrid(int& gridSize, double& pixelsPerCm) {
    computeMainAxes(gridSize, pixelsPerCm);
    computeGridLines(gridSize, pixelsPerCm);
}

void CoordinateGrid::computeMainAxes(const int& gridSize, const double& pixelsPerCm) {
    m_xAxis = Line(Point(0, -gridSize * pixelsPerCm), Point(0, gridSize * pixelsPerCm));
    m_yAxis = Line(Point(-gridSize * pixelsPerCm, 0), Point(gridSize * pixelsPerCm, 0));
}

void CoordinateGrid::computeGridLines(const int &gridSize, const double &pixelsPerCm) {
    m_gridLines.clear();
    for (int i = -gridSize; i <= gridSize; ++i) {
        if (i == 0) {
            continue;
        }
        double x = i * pixelsPerCm;
        m_gridLines.emplace_back(Point(x, -gridSize * pixelsPerCm), Point(x, gridSize * pixelsPerCm));
        double y = i * pixelsPerCm;
        m_gridLines.emplace_back(Point(-gridSize * pixelsPerCm, y), Point(gridSize * pixelsPerCm, y));
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




