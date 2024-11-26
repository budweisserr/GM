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
#include "grid.h"
#include "drawing_interface.h"
#include "figure.h"

class Draw final : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    explicit Draw(QGraphicsScene *scene = nullptr);
    ~Draw() override = default;
   // signals:
    //void pixelsPerCmChanged(double pixelsPerCm);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;

    public slots:
    void resetScene();
    void setPixelsPerCm(double pixelsPerCm);
    void setGridSize(int size);
    void applyAffineTransformation(double Xx, double Xy, double Yx, double Yy, double Ox, double Oy);
    void applyProjectiveTransformation(double Xx, double Xy, double Xw, double Yx, double Yy, double Yw, double Ox, double Oy, double Ow);

    void applyShiftToFigure(double shiftX, double shiftY);
    void applyRotateToFigure(double x, double y, double angle);
protected:
    /*
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    */
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

private:
    std::unique_ptr<CoordinateGrid> m_grid;
    std::unique_ptr<Figure> m_figure;

    void updateScene();
    int m_gridSize = 15;
    double m_pixelsPerCm = 37.795275591;

};



#endif //DRAWING_H
