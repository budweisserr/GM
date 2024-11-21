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

class Draw : public QObject, public QGraphicsItem {
    Q_OBJECT
public:
    Draw(QGraphicsScene *scene = nullptr);
    ~Draw() override = default;
   // signals:
    //void pixelsPerCmChanged(double pixelsPerCm);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QRectF boundingRect() const override;

    public slots:
    void setPixelsPerCm(double pixelsPerCm);
    void setGridSize(int size);
    void applyAffineTransformation(double Xx, double Xy, double Yx, double Yy, double Ox, double Oy);
    void applyProjectiveTransformation(double Xx, double Xy, double Xw, double Yx, double Yy, double Yw, double Ox, double Oy, double Ow);

protected:
    /*
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    */
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;

private:
    std::unique_ptr<CoordinateGrid> m_grid;
    Drawable *m_figure;

    void updateScene();
    int m_gridSize = 10;
    double m_pixelsPerCm = 37.795275591;

};



#endif //DRAWING_H