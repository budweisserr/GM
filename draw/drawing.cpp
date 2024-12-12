#include "drawing.h"

Draw::Draw(QGraphicsScene* scene)
    : m_grid(new CoordinateGrid()),
      m_shape(new Shape())
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
    painter->translate(widget->width() / 2, -widget->height() / 2);

    m_grid->draw(painter);
    m_shape->draw(painter);

    if (m_normalVisible && !m_shape->getPoints().empty()) {
        m_shape->drawNormal(painter, m_shape->getPoints()[m_currentPointIndex]);
    }

    if (m_tangentVisible && !m_shape->getPoints().empty()) {
        m_shape->drawTangent(painter, m_shape->getPoints()[m_currentPointIndex]);
    }

    if (m_pointVisible && !m_shape->getPoints().empty()) {
        m_shape->drawPoint(painter, m_shape->getPoints()[m_currentPointIndex]);
    }
        
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
    m_shape->computeShape(m_gridSize, m_pixelsPerCm);
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
    m_shape->setA(4);
    m_shape->setB(2.8);
    m_shape->setD(-1);
    updateScene();
}

void Draw::applyShiftToShape(double shiftX, double shiftY) {
    m_matrix[0][0] = 1;
    m_matrix[0][1] = 0;
    m_matrix[0][2] = 0;
    m_matrix[1][0] = 0;
    m_matrix[1][1] = 1;
    m_matrix[1][2] = 0;
    m_matrix[2][0] = shiftX * m_pixelsPerCm;
    m_matrix[2][1] = shiftY * m_pixelsPerCm;
    m_matrix[2][2] = 1;

    m_shape->transform(m_matrix);
    update();
}

void Draw::applyRotateToShape(double x, double y, double angle) {
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

    m_shape->transform(m_matrix);
    update();
}

void Draw::animateShape(AnimationType type) {
    m_animationType = type;
    if (!m_animationTimer) {
        m_animationTimer = new QTimer(this);
        connect(m_animationTimer, &QTimer::timeout, this, &Draw::updateAnimation);
        m_animationTimer->start(1);
    } else {
        m_animationTimer->stop();
        delete m_animationTimer;
        m_animationTimer = nullptr;
    }
}

void Draw::updateAnimation() {
    static bool direction = true; // true for increasing, false for decreasing
    double m_animationStep = 0.1;
    double m_animationTarget = 10;
    double m_animationMin = -10;

    switch (m_animationType) {
        case AnimationType::ANIMATE_A:
            if (direction) {
                m_shape->setA(m_shape->getA() + m_animationStep);
                if (m_shape->getA() >= m_animationTarget) direction = false;
            } else {
                m_shape->setA(m_shape->getA() - m_animationStep);
                if (m_shape->getA() <= m_animationMin) direction = true;
            }
            emit animationChangedA(m_shape->getA());
            break;
        case AnimationType::ANIMATE_B:
            if (direction) {
                m_shape->setB(m_shape->getB() + m_animationStep);
                if (m_shape->getB() >= m_animationTarget) direction = false;
            } else {
                m_shape->setB(m_shape->getB() - m_animationStep);
                if (m_shape->getB() <= m_animationMin) direction = true;
            }
            emit animationChangedB(m_shape->getB());
            break;
        case AnimationType::ANIMATE_C:
            if (direction) {
                m_shape->setD(m_shape->getD() + m_animationStep);
                if (m_shape->getD() >= m_animationTarget) direction = false;
            } else {
                m_shape->setD(m_shape->getD() - m_animationStep);
                if (m_shape->getD() <= m_animationMin) direction = true;
            }
            emit animationChangedC(m_shape->getD());
            break;
        case AnimationType::ANIMATE_FULL:
            if (direction) {
                m_shape->setA(m_shape->getA() + m_animationStep);
                m_shape->setB(m_shape->getB() + m_animationStep / 2);
                m_shape->setD(m_shape->getD() + m_animationStep / 3);
                if (m_shape->getA() >= m_animationTarget || m_shape->getB() >= m_animationTarget || m_shape->getD() >= m_animationTarget) direction = false;
            } else {
                m_shape->setA(m_shape->getA() - m_animationStep);
                m_shape->setB(m_shape->getB() - m_animationStep / 2);
                m_shape->setD(m_shape->getD() - m_animationStep / 3);
                if (m_shape->getA() <= m_animationMin || m_shape->getB() <= m_animationMin || m_shape->getD() <= m_animationMin) direction = true;
            }
            emit animationChangedA(m_shape->getA());
            emit animationChangedB(m_shape->getB());
            emit animationChangedC(m_shape->getD());
            break;
    }
    m_shape->computeShape(m_gridSize, m_pixelsPerCm);
    updateScene();
}

void Draw::toggleNormalVisibility(bool checked) {
    m_normalVisible = checked;
    update();
}

void Draw::toggleTangentVisibility(bool checked) {
    m_tangentVisible = checked;
    update();
}

void Draw::togglePointVisibility(bool checked) {
    m_pointVisible = checked;
    update();
}

void Draw::showPreviousPoint() {
    if (!m_shape->getPoints().empty()) {
        m_currentPointIndex = (m_currentPointIndex - 10 + m_shape->getPoints().size()) % m_shape->getPoints().size();
        update();
    }
    QString point = QString("%1, %2").arg(m_shape->getPoints().at(m_currentPointIndex).x() / m_pixelsPerCm).arg(m_shape->getPoints().at(m_currentPointIndex).y() / m_pixelsPerCm);
    emit currentPointChanged(point);
}

void Draw::showNextPoint() {
    if (!m_shape->getPoints().empty()) {
        m_currentPointIndex = (m_currentPointIndex + 10) % m_shape->getPoints().size();
        update();
    }
    QString point = QString("%1, %2").arg(m_shape->getPoints().at(m_currentPointIndex).x() / m_pixelsPerCm).arg(m_shape->getPoints().at(m_currentPointIndex).y() / m_pixelsPerCm);
    emit currentPointChanged(point);
}






