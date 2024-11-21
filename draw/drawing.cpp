#include "drawing.h"
#include <iostream>

#define log(x) std::cout << x << std::endl;

Draw::Draw(QGraphicsScene* scene)
    : m_grid(new CoordinateGrid())
      //m_figure(new Drawable()),
      {
    if (scene) {
        scene->addItem(this);
    }
    updateScene();
}

void Draw::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    painter->fillRect(boundingRect(), Qt::white);

    painter->scale(1, -1);
    painter->translate(widget->width(), -widget->height());
    m_grid->draw(painter);
    //m_figure->draw(painter);
}


QRectF Draw::boundingRect() const {
    //dynamically change the size of the bounding rectangle with grid area
    return QRectF(0, 0, 10000, 10000);
}

void Draw::wheelEvent(QGraphicsSceneWheelEvent* event) {
    constexpr double MIN_PIXELS_PER_CM = 1.0;
    constexpr double MAX_PIXELS_PER_CM = 200.0;

    if (event->delta() > 0) {
        if (m_pixelsPerCm < MAX_PIXELS_PER_CM) {
            m_pixelsPerCm *= 1.2; // Smoother zoom
        }
    } else {
        if (m_pixelsPerCm > MIN_PIXELS_PER_CM) {
            m_pixelsPerCm /= 1.2;
        }
    }
    updateScene();
    event->accept();
}
/*
void Draw::paintEvent(QPaintEvent* event) {
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(event->rect(), Qt::white);

    painter.scale(1, -1);
    painter.translate(width() / 2, -height() / 2);
    m_grid->draw(&painter);
    //m_figure->draw(&painter);
}

void Draw::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    updateScene();
}

void Draw::wheelEvent(QWheelEvent* event) {
    constexpr double MIN_PIXELS_PER_CM = 1.0;
    constexpr double MAX_PIXELS_PER_CM = 200.0;

    if (event->angleDelta().y() > 0) {
        if (m_pixelsPerCm < MAX_PIXELS_PER_CM) {
            m_pixelsPerCm *= 1.2; // Smoother zoom
        }
    } else {
        if (m_pixelsPerCm > MIN_PIXELS_PER_CM) {
            m_pixelsPerCm /= 1.2;
        }
    }
    updateScene();
    event->accept();
}
*/

void Draw::updateScene() {
    m_grid->computeGrid(m_gridSize, m_pixelsPerCm);
    update();
}

void Draw::setGridSize(int size) {
    m_gridSize = size;
    updateScene();
}

void Draw::setPixelsPerCm(double pixelsPerCm) {
    m_pixelsPerCm = pixelsPerCm;
    //emit pixelsPerCmChanged(pixelsPerCm);
    updateScene();
}

void Draw::applyAffineTransformation(double Xx, double Xy, double Yx, double Yy, double Ox, double Oy) {
    double **matrix = new double*[3];
    for (int i = 0; i < 3; ++i) {
        matrix[i] = new double[3];
    }
    matrix[0][0] = Xx;
    matrix[0][1] = Xy;
    matrix[0][2] = 0;
    matrix[1][0] = Yx;
    matrix[1][1] = Yy;
    matrix[1][2] = 0;
    matrix[2][0] = Ox * m_pixelsPerCm;
    matrix[2][1] = Oy * m_pixelsPerCm;
    matrix[2][2] = 1;



    m_grid->transform(matrix);
    update();
}

void Draw::applyProjectiveTransformation(double Xx, double Xy, double Xw, double Yx, double Yy, double Yw, double Ox, double Oy, double Ow) {
    double **matrix = new double*[3];
    for (int i = 0; i < 3; ++i) {
        matrix[i] = new double[3];
    }

    matrix[0][0] = Xx * Xw * m_pixelsPerCm;
    matrix[0][1] = Xy * Xw * m_pixelsPerCm;
    matrix[0][2] = Xw;
    matrix[1][0] = Yx * Yw * m_pixelsPerCm;
    matrix[1][1] = Yy * Yw * m_pixelsPerCm;
    matrix[1][2] = Yw;
    matrix[2][0] = Ox * Ow * m_pixelsPerCm;
    matrix[2][1] = Oy * Ow * m_pixelsPerCm;
    matrix[2][2] = Ow;

    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            log(matrix[i][j]);
        }
    }

    log ("-----------------");
    m_grid->transform(matrix);
    update();
}




