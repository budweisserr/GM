#pragma once
#ifndef DRAWING_H
#define DRAWING_H

#include <QWidget>
#include <QPainter>
#include <QWheelEvent>
#include <QResizeEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneWheelEvent>
#include <QPaintEvent>
#include <QTimer>
#include "grid.h"
#include "drawing_interface.h"
#include <math.h>
#include "shape.h"

class Draw final : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit Draw(QGraphicsScene *scene = nullptr);
    ~Draw() override = default;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;

    public slots:
    void resetScene();
    void setPixelsPerCm(double pixelsPerCm);
    void setGridSize(int size);

    void applyShiftToShape(double shiftX, double shiftY);
    void applyRotateToShape(double x, double y, double angle);

    void animateShape(AnimationType type);

    std::vector<Point> getPoints() const { return m_shape->getPoints(); }

    signals:
    void pixelsPerCmChanged(double pixelsPerCm);
    void animationChangedA(double value);
    void animationChangedB(double value);
    void animationChangedC(double value);
    void currentPointChanged(const QString& point);

    private slots:
    void updateAnimation();

    public slots:
    void toggleNormalVisibility(bool checked);
    void toggleTangentVisibility(bool checked);
    void togglePointVisibility(bool checked);
    void showPreviousPoint();
    void showNextPoint();
protected:
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

private:
    std::unique_ptr<CoordinateGrid> m_grid;
    std::unique_ptr<Shape> m_shape;

    void updateScene();

    int m_gridSize = 15;
    double m_pixelsPerCm = 37.795275591;

    double ** m_matrix;

    QTimer *m_animationTimer = nullptr;
    AnimationType m_animationType = AnimationType::ANIMATE_FULL;

    bool m_normalVisible = false;
    bool m_tangentVisible = false;
    bool m_pointVisible = false;
    size_t m_currentPointIndex = 0;


};



#endif //DRAWING_H
