#ifndef FIGURE_H
#define FIGURE_H

#include <QWidget>
#include <vector>
#include "circle.h"
#include "line.h"
#include "drawing_interface.h"

class Figure final : public Drawable {
public:
    void draw(QPainter *painter) override;

    ~Figure() override = default;

    void computeFigure(double &pixelsPerCm);

public slots:
    void transform(double **matrix);

private:
    std::unique_ptr<Point> A = std::make_unique<Point>(0, 0), B = std::make_unique<Point>(0, 0), C = std::make_unique<Point>(0, 0),
                      D = std::make_unique<Point>(0, 0), E = std::make_unique<Point>(0, 0), F = std::make_unique<Point>(0, 0),
                      G = std::make_unique<Point>(0, 0), H = std::make_unique<Point>(0, 0), I = std::make_unique<Point>(0, 0),
                      J = std::make_unique<Point>(0, 0), K = std::make_unique<Point>(0, 0), L = std::make_unique<Point>(0, 0),
                      M = std::make_unique<Point>(0, 0), O = std::make_unique<Point>(0, 0), P = std::make_unique<Point>(0, 0),
                      Q = std::make_unique<Point>(0, 0), R = std::make_unique<Point>(0, 0), S = std::make_unique<Point>(0, 0),
                      T = std::make_unique<Point>(0, 0); // points
    Circle BC = Circle(0, 0, 0, 0, 0, 0), FG = Circle(0, 0, 0, 0, 0, 0), IJ = Circle(0, 0, 0, 0, 0, 0), KL =
                    Circle(0, 0, 0, 0, 0, 0), SR = Circle(0, 0, 0, 0, 0, 0), QT = Circle(0, 0, 0, 0, 0, 0);
    // center circle points
};


#endif //FIGURE_H
