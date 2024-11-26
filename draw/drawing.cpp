#include "drawing.h"


Draw::Draw(QGraphicsScene* scene)
    : m_grid(new CoordinateGrid()),
      m_figure(new Figure())
      {
    if (scene) {
        scene->addItem(this);
    }

    m_matrix = new double*[3];
    for (int i = 0; i < 3; ++i) {
        m_matrix[i] = new double[3];
    }

    updateScene();
}

void Draw::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);

    painter->fillRect(boundingRect(), Qt::white);

    painter->scale(1, -1);
    painter->translate(widget->width() / 3, -widget->height());

    m_grid->draw(painter);
    m_figure->draw(painter);
}


QRectF Draw::boundingRect() const {
    return QRectF(0, 0, 10000, 10000);
}

void Draw::wheelEvent(QGraphicsSceneWheelEvent* event) {
    if (event->delta() > 0) {
        if (constexpr double MAX_PIXELS_PER_CM = 200.0; m_pixelsPerCm < MAX_PIXELS_PER_CM) {
            m_pixelsPerCm *= 1.2; // Smoother zoom
        }
    } else {
        if (constexpr double MIN_PIXELS_PER_CM = 1.0; m_pixelsPerCm > MIN_PIXELS_PER_CM) {
            m_pixelsPerCm /= 1.2;
        }
    }
    updateScene();
    emit pixelsPerCmChanged(m_pixelsPerCm);
    event->accept();
}

void Draw::updateScene() {
    m_grid->computeGrid(m_gridSize, m_pixelsPerCm);
    m_figure->computeFigure(m_pixelsPerCm);
    update();
}

void Draw::setGridSize(int size) {
    m_gridSize = size;
    updateScene();
}

void Draw::setPixelsPerCm(double pixelsPerCm) {
    m_pixelsPerCm = pixelsPerCm;
    updateScene();
}

void Draw::resetScene() {
    updateScene();
}

void Draw::applyAffineTransformation(double Xx, double Xy, double Yx, double Yy, double Ox, double Oy) {
    m_matrix[0][0] = Xx;
    m_matrix[0][1] = Xy;
    m_matrix[0][2] = 0;
    m_matrix[1][0] = Yx;
    m_matrix[1][1] = Yy;
    m_matrix[1][2] = 0;
    m_matrix[2][0] = Ox * m_pixelsPerCm;
    m_matrix[2][1] = Oy * m_pixelsPerCm;
    m_matrix[2][2] = 1;

    m_grid->transform(m_matrix);
    m_figure->transform(m_matrix);
    update();
}

void Draw::applyProjectiveTransformation(double Xx, double Xy, double Xw, double Yx, double Yy, double Yw, double Ox, double Oy, double Ow) {
    m_matrix[0][0] = Xx * Xw * m_pixelsPerCm;
    m_matrix[0][1] = Xy * Xw * m_pixelsPerCm;
    m_matrix[0][2] = Xw;
    m_matrix[1][0] = Yx * Yw * m_pixelsPerCm;
    m_matrix[1][1] = Yy * Yw * m_pixelsPerCm;
    m_matrix[1][2] = Yw;
    m_matrix[2][0] = Ox * Ow * m_pixelsPerCm;
    m_matrix[2][1] = Oy * Ow * m_pixelsPerCm;
    m_matrix[2][2] = Ow;

    m_grid->transform(m_matrix);
    m_figure->transform(m_matrix);
    update();
}

void Draw::applyShiftToFigure(double shiftX, double shiftY) {
    m_matrix[0][0] = 1;
    m_matrix[0][1] = 0;
    m_matrix[0][2] = 0;
    m_matrix[1][0] = 0;
    m_matrix[1][1] = 1;
    m_matrix[1][2] = 0;
    m_matrix[2][0] = shiftX * m_pixelsPerCm;
    m_matrix[2][1] = shiftY * m_pixelsPerCm;
    m_matrix[2][2] = 1;

    m_figure->transform(m_matrix);
    update();
}

void Draw::applyRotateToFigure(double x, double y, double angle) {
    double rad = angle * M_PI / 180.0;
    m_matrix[0][0] = cos(rad);
    m_matrix[0][1] = sin(rad);
    m_matrix[0][2] = 0;
    m_matrix[1][0] = -sin(rad);
    m_matrix[1][1] = cos(rad);
    m_matrix[1][2] = 0;
    m_matrix[2][0] = -x * m_pixelsPerCm * (cos(rad) - 1) + y * m_pixelsPerCm * sin(rad);
    m_matrix[2][1] = -x * m_pixelsPerCm * sin(rad) - y * m_pixelsPerCm * (cos(rad) - 1);
    m_matrix[2][2] = 1;

    m_figure->transform(m_matrix);
    update();
}

void Draw::applySymmetryToFigure(double x, double y) {
    m_matrix[0][0] = -1;
    m_matrix[0][1] = 0;
    m_matrix[0][2] = 0;
    m_matrix[1][0] = 0;
    m_matrix[1][1] = -1;
    m_matrix[1][2] = 0;
    m_matrix[2][0] = 2 * x * m_pixelsPerCm;
    m_matrix[2][1] = 2 * y * m_pixelsPerCm;
    m_matrix[2][2] = 1;

    m_figure->transform(m_matrix);
    update();
}







