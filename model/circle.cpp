#include "circle.h"

Circle::Circle(Point center, double radius, Point start, Point end, double degreeStart,
               double degreeEnd) : center(center), radius(radius), start(start), end(end), degreeStart(degreeStart),
                                   degreeEnd(degreeEnd) {
}

void Circle::computeCircle() {
    for (double i = degreeStart; i < degreeEnd; i++) {
        double degree0 = i;
        double degree1 = i + 1;
        if (i == degreeStart) {
            start = Point(computePointX(degree0), computePointY(degree0));
            Point point1 = Point(computePointX(degree1), computePointY(degree1));
            lines.emplace_back(start, point1);
        } else if (i == degreeEnd - 1) {
            Point point0 = Point(computePointX(degree0), computePointY(degree0));
            end = Point(computePointX(degree1), computePointY(degree1));
            lines.emplace_back(point0, end);
        } else {
            Point point0 = Point(computePointX(degree0), computePointY(degree0));
            Point point1 = Point(computePointX(degree1), computePointY(degree1));
            lines.emplace_back(point0, point1);
        }
    }
}

double Circle::computePointX(double degree) const{
    return center.x() + radius * cos(degree);
}

double Circle::computePointY(double degree) const{
    return center.y() + radius * sin(degree);
}

void Circle::resize(double radius){
    this->radius = radius;
    lines.clear();
    computeCircle();
}