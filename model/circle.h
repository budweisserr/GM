#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H
#include <vector>
#include "point.h"
#include "line.h"


class Circle {
public:
    Circle(Point center, double radius, Point start, Point end double degreeStart, double degreeEnd);

    void computeCircle();

    double computePointX(double degree) const;

    double computePointY(double degree) const;

    void resize(double radius);

private:
    Point center;
    double radius;
    Point start;
    Point end;
    double degreeStart;
    double degreeEnd;
    std::vector<Line> lines;
};


#endif //CIRCLE_H
