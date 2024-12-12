#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include <QWidget>
#include <vector>
#include "math.h"
#include "line.h"
#include "drawing_interface.h"

enum class AnimationType {
    ANIMATE_A,
    ANIMATE_B,
    ANIMATE_C,
    ANIMATE_FULL
};

class Shape final : public Drawable {
public:
    void draw(QPainter *painter) override;

    ~Shape() override = default;

    void computeShape(int &gridSize, double &pixelsPerCm);

    double getA() const { return a; }
    double getB() const { return b; }
    double getD() const { return d; }

    void setA(double value) { a = value; }
    void setB(double value) { b = value; }
    void setD(double value) { d = value; }

    void drawNormal(QPainter *painter, Point& point);
    void drawTangent(QPainter *painter, Point& point);
    void drawPoint(QPainter *painter, Point& point);

    std::vector<Point> getPoints() const { return m_points; }

public slots:
    void transform(double **matrix);

private:
    std::vector<Point> m_pointsLeftUpper, m_pointsRightUpper, m_pointsLeftLower, m_pointsRightLower;
    std::vector<Point> m_points;
    double computeDerivativeX(double x, double y) const;
    double computeDerivativeY(double x, double y) const;
    Line computeNormal(Point& point) const;
    Line computeTangent(Point& point) const;
    double a = 4;
    double b = 2.8;
    double d = -1;
    double step = 0.01;
    double epsilon = 0.01;
};


#endif //SHAPE_H
