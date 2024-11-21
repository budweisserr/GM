// grid.h
#pragma once
#include <QWidget>
#include <vector>
#include "line.h"
#include "drawing_interface.h"

class CoordinateGrid : public Drawable {
public:
    void draw(QPainter* painter) override;
    explicit CoordinateGrid() = default;
    ~CoordinateGrid() = default;
    void computeGrid(int& gridSize, double& pixelsPerCm);

    double getGridArea(double pixelsPerCm) const;
    double calculateArea(double& pixelsPerCm) const;

    public slots:
    void transform(double **matrix);
protected:
    /*
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    */

private:

    void computeMainAxes(const int& gridSize, const double& pixelsPerCm);
    void computeGridLines(const int& gridSize, const double& pixelsPerCm);

    std::vector<Line> m_gridLines;
    Line m_xAxis = Line(Point(-10, 0), Point(10, 0));
    Line m_yAxis = Line(Point(0, -10), Point(0, 10));
};