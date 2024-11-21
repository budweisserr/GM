#ifndef FIGURE_H
#define FIGURE_H

#include <QWidget>
#include <vector>
#include "line.h"
#include "drawing_interface.h"

class Figure : public Drawable {
public:
    void draw(QPainter* painter) override;
    explicit Figure() = default;
    ~Figure() = default;
    void computeFigure(double& pixelsPerCm);

    public slots:
    void transform(double **matrix);

};



#endif //FIGURE_H
