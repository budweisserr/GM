#include "circle.h"

Circle::Circle(Point center, double radius, Point* start, Point* end, double degreeStart,
               double degreeEnd) : start(start), end(end), center(center), radius(radius), degreeStart(degreeStart),
                                   degreeEnd(degreeEnd) {
}

void Circle::computeCircle() {
    lines.clear();

    double step = 1.0;
    double radStart = degreeStart * M_PI / 180.0;
    double radEnd = degreeEnd * M_PI / 180.0;

    double rad0 = radStart;
    double rad1 = rad0 + step * M_PI / 180.0;

    Point point0(computePointX(rad0), computePointY(rad0));
    start->setX(point0.x());
    start->setY(point0.y());

    while (rad0 < radEnd) {
        rad1 = std::min(radEnd, rad0 + step * M_PI / 180.0); // Avoid overshooting

        Point point1(computePointX(rad1), computePointY(rad1));
        lines.emplace_back(point0, point1);

        point0 = point1;
        rad0 = rad1;
    }

    end->setX(point0.x());
    end->setY(point0.y());
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


