// grid.cpp
#include "grid.h"
#include <QPainter>

void CoordinateGrid::computeGrid(int& gridSize, double& pixelsPerCm) {
    computeMainAxes(gridSize, pixelsPerCm);
    computeGridLines(gridSize, pixelsPerCm);
}

void CoordinateGrid::computeMainAxes(const int& gridSize, const double& pixelsPerCm) {
    m_xAxis = Line(Point(0, 0), Point(gridSize * pixelsPerCm, 0));
    m_yAxis = Line(Point(0, 0), Point(0, gridSize * pixelsPerCm));
}

void CoordinateGrid::computeGridLines(const int &gridSize, const double &pixelsPerCm) {
    m_gridLines.clear();
    for (int i = 1; i <= gridSize; ++i) {
        double x = i * pixelsPerCm;
        m_gridLines.emplace_back(Point(x, 0), Point(x, gridSize * pixelsPerCm));
        double y = i * pixelsPerCm;
        m_gridLines.emplace_back(Point(0, y), Point(gridSize * pixelsPerCm, y));
    }
}

void CoordinateGrid::transform(double **matrix) {
    for (auto& line : m_gridLines) {
        line.transform(matrix);
    }
    m_xAxis.transform(matrix);
    m_yAxis.transform(matrix);
}

/*
void CoordinateGrid::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fill background
    painter.fillRect(event->rect(), Qt::white);

    // Set up coordinate system
    m_origin = QPoint(width() / 2, height() / 2);

    double pixelSpacing = m_gridSpacingCm * m_pixelsPerCm;

    painter.setPen(QPen(Qt::lightGray, 1, Qt::SolidLine));
    for (const auto& line : m_transformedGridLines) {
        QPoint start = Point::toQPoint(line.start());
        QPoint end = Point::toQPoint(line.end());
        painter.drawLine(start, end);
    }

    // Render main axes
    renderMainAxes();
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter.drawLine(Point::toQPoint(m_xAxis.start()), Point::toQPoint(m_xAxis.end()));
    painter.drawLine(Point::toQPoint(m_yAxis.start()), Point::toQPoint(m_yAxis.end()));

    // Draw current scale
    QString scaleText = QString("Scale: 1 cm = %1 px").arg(m_pixelsPerCm, 0, 'f', 1);
    painter.drawText(10, height() - 10, scaleText);
}

void CoordinateGrid::wheelEvent(QWheelEvent* event) {
    if (event->angleDelta().y() > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
    event->accept();
}

void CoordinateGrid::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    update();
}

void CoordinateGrid::applyAffineTransformation(double Xx, double Xy, double Yx, double Yy, double Ox, double Oy) {
    auto** matrix = new double*[3];
    for (int i = 0; i < 3; ++i) {
        matrix[i] = new double[3];
    }

    matrix[0][0] = Xx / m_pixelsPerCm;
    matrix[0][1] = Xy / m_pixelsPerCm;
    matrix[0][2] = 0;
    matrix[1][0] = Yx / m_pixelsPerCm;
    matrix[1][1] = Yy / m_pixelsPerCm;
    matrix[1][2] = 0;
    matrix[2][0] = Ox;
    matrix[2][1] = Oy;
    matrix[2][2] = 1;

    for (auto& line : m_transformedGridLines) {
        line.transform(matrix);
    }

    //update main axes
    m_xAxis.transform(matrix);
    m_yAxis.transform(matrix);

    for (int i = 0; i < 3; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;

    update();
}

void CoordinateGrid::applyProjectiveTransformation(double Xx, double Xy, double Xw, double Yx, double Yy, double Yw,
                                                   double Ox, double Oy, double Ow) {
    return;
}
*/

void CoordinateGrid::draw(QPainter *painter) {
    painter->setPen(QPen(Qt::lightGray, 1, Qt::SolidLine));
    for (const auto& line : m_gridLines) {
        QPoint start = Point::toQPoint(line.start());
        QPoint end = Point::toQPoint(line.end());
        painter->drawLine(start, end);
    }

    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter->drawLine(Point::toQPoint(m_xAxis.start()), Point::toQPoint(m_xAxis.end()));
    painter->drawLine(Point::toQPoint(m_yAxis.start()), Point::toQPoint(m_yAxis.end()));
}

double CoordinateGrid::calculateArea(double& pixelsPerCm) const {
    if (m_gridLines.empty()) {
        return 0.0;
    }

    // Find min and max coordinates to calculate grid dimensions
    double minX = m_gridLines[0].start().x(), maxX = m_gridLines[0].start().x();
    double minY = m_gridLines[0].start().y(), maxY = m_gridLines[0].start().y();

    for (const auto& point : m_gridLines) {
        if (point.start().x() < minX) {
            minX = point.start().x();
        }
        if (point.start().x() > maxX) {
            maxX = point.start().x();
        }
        if (point.start().y() < minY) {
            minY = point.start().y();
        }
        if (point.start().y() > maxY) {
            maxY = point.start().y();
        }
    }

    // Calculate width and height
    double width = maxX - minX;
    double height = maxY - minY;

    // Calculate area in square pixels
    double areaInPixels = width * height;

    // Convert to square centimeters (if needed)
    // assuming m_pixelsPerCm is accessible or passed as parameter
    double areaInCm2 = areaInPixels / (pixelsPerCm * pixelsPerCm);

    return areaInCm2;
}

double CoordinateGrid::getGridArea(double pixelsPerCm) const {
    return calculateArea(pixelsPerCm);
}



