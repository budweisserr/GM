#ifndef LINE_H
#define LINE_H

#include <qmath.h>

#include "point.h"

class Line {
public:
    Line(const Point& start, const Point& end) : m_start(start), m_end(end) {}
    Point start() const { return m_start; }
    Point end() const { return m_end; }

    void transform(double** matrix) {
        m_start.transform(matrix);
        m_end.transform(matrix);
    }
private:
    Point m_start, m_end;
};

#endif