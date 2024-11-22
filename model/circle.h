#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H
#include <memory>
#include <vector>
#include "point.h"
#include "line.h"


class Circle {
public:
    Circle(Point center, double radius, Point* start, Point* end, double degreeStart, double degreeEnd);

    void computeCircle();

    double computePointX(double degree) const;

    double computePointY(double degree) const;

    void transform(double **matrix);

    std::vector<Line> getLines() const { return lines; }

    Circle operator=(const Circle& other) = delete;

    void align(Point center, double radius, Point* start, Point* end, double degreeStart, double degreeEnd) {
        setCenter(center);
        setRadius(radius);
        setStart(start);
        setEnd(end);
        setDegreeStart(degreeStart);
        setDegreeEnd(degreeEnd);
        computeCircle();
    }

private:
    Point* start;
    Point* end;
    Point center;
    double radius;
    double degreeStart;
    double degreeEnd;
    std::vector<Line> lines;

    void setCenter(Point center) { this->center = center; }
    void setRadius(double radius) { this->radius = radius; }
    void setStart(Point* start) { this->start = start; }
    void setEnd(Point* end) { this->end = end; }
    void setDegreeStart(double degreeStart) { this->degreeStart = degreeStart; }
    void setDegreeEnd(double degreeEnd) { this->degreeEnd = degreeEnd; }
};


#endif //CIRCLE_H
