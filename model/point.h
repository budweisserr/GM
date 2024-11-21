#pragma once
#ifndef POINT_H
#define POINT_H

#include <QPoint>

class Point {
public:
    Point(double x = 0, double y = 0) : m_x(x), m_y(y) {}
    double x() const { return m_x; }
    double y() const { return m_y; }

    void transform (double **matrix) {
        double vx = m_x * matrix[0][0] + m_y * matrix[1][0] + matrix[2][0];
        double vy = m_x * matrix[0][1] + m_y * matrix[1][1] + matrix[2][1];
        double w = m_x * matrix[0][2] + m_y * matrix[1][2] + matrix[2][2];

        m_x = vx / w;
        m_y = vy / w;
    }


    static QPoint toQPoint(const Point& point) {
        return QPoint(point.x(), point.y());
    }
    static Point fromQPoint(const QPoint& point) {
        return Point(point.x(), point.y());
    }
private:
    double m_x, m_y;
};

#endif // POINT_H