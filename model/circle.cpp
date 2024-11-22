#include "circle.h"

Circle::Circle(Point center, double radius, Point* start, Point* end, double degreeStart,
               double degreeEnd) : start(start), end(end), center(center), radius(radius), degreeStart(degreeStart),
                                   degreeEnd(degreeEnd) {
    computeCircle();
}

void Circle::computeCircle() {
    lines.clear();
    for (double i = degreeStart; i < degreeEnd; i++) {
        double degree0 = i;
        double degree1 = i + 1;
        if (i == degreeStart) {
            start->setX(computePointX(degree0));
            start->setY(computePointY(degree0));
            Point point1 = Point(computePointX(degree1), computePointY(degree1));
            lines.emplace_back(*start, point1);
        } else if (i == degreeEnd - 1) {
            Point point0 = Point(computePointX(degree0), computePointY(degree0));
            end->setX(computePointX(degree1));
            end->setY(computePointY(degree1));
            lines.emplace_back(point0,* end);
        } else {
            Point point0 = Point(computePointX(degree0), computePointY(degree0));
            Point point1 = Point(computePointX(degree1), computePointY(degree1));
            lines.emplace_back(point0, point1);
        }
    }
}

double Circle::computePointX(double degree) const {
    return center.x() + radius * cos(degree);
}

double Circle::computePointY(double degree) const {
    return center.y() + radius * sin(degree);
}

void Circle::transform(double **matrix) {
    for (auto &line: lines)
        line.transform(matrix);
}


