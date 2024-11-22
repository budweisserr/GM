#include "figure.h"

void Figure::computeFigure(double &pixelsPerCm) {
    A->setX(2 * pixelsPerCm);
    A->setY(11 * pixelsPerCm);

    D->setX(10 * pixelsPerCm);
    D->setY(8 * pixelsPerCm);

    E->setX(10 * pixelsPerCm);
    E->setY(4 * pixelsPerCm);

    H->setX(2 * pixelsPerCm);
    H->setY(1 * pixelsPerCm);

    M->setX(5 * pixelsPerCm);
    M->setY(6 * pixelsPerCm);

    O->setX(9 * pixelsPerCm);
    O->setY(8 * pixelsPerCm);

    P->setX(9 * pixelsPerCm);
    P->setY(4 * pixelsPerCm);

    Point center1 = Point(7 * pixelsPerCm,6 * pixelsPerCm);
    Point center2 = Point(10 * pixelsPerCm, 10 * pixelsPerCm);
    Point center3 = Point(10 * pixelsPerCm, 2 * pixelsPerCm);

    BC.align(center2, 1 * pixelsPerCm, C.get(), B.get(), -45, 90);
    FG.align(center3 , 1 * pixelsPerCm, G.get(), F.get(), -90, 45);

    IJ.align(center1, 4 * pixelsPerCm, J.get(), I.get(), 70, 140);
    KL.align(center1, 3 * pixelsPerCm, K.get(), L.get(), 70, 140);
    SR.align(center1, 3 * pixelsPerCm, R.get(), S.get(), 220, 290);
    QT.align(center1, 4 * pixelsPerCm, Q.get(), T.get(), 220, 290);
}

void Figure::draw(QPainter *painter) {
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));

    painter->drawLine(Point::toQPoint(*A), Point::toQPoint(*B));
    painter->drawLine(Point::toQPoint(*C), Point::toQPoint(*D));
    painter->drawLine(Point::toQPoint(*D), Point::toQPoint(*E));
    painter->drawLine(Point::toQPoint(*E), Point::toQPoint(*F));
    painter->drawLine(Point::toQPoint(*G), Point::toQPoint(*H));
    painter->drawLine(Point::toQPoint(*H), Point::toQPoint(*A));
    painter->drawLine(Point::toQPoint(*M), Point::toQPoint(*O));
    painter->drawLine(Point::toQPoint(*O), Point::toQPoint(*P));
    painter->drawLine(Point::toQPoint(*P), Point::toQPoint(*M));
//
    painter->drawLine(Point::toQPoint(*I), Point::toQPoint(*L));
    painter->drawLine(Point::toQPoint(*J), Point::toQPoint(*K));
    painter->drawLine(Point::toQPoint(*Q), Point::toQPoint(*R));
    painter->drawLine(Point::toQPoint(*S), Point::toQPoint(*T));

    for (const auto& line : BC.getLines()) {
        painter->drawLine(Point::toQPoint(line.start()), Point::toQPoint(line.end()));
    }

    for (const auto& line : FG.getLines()) {
        painter->drawLine(Point::toQPoint(line.start()), Point::toQPoint(line.end()));
    }

    for (const auto& line : IJ.getLines()) {
        painter->drawLine(Point::toQPoint(line.start()), Point::toQPoint(line.end()));
    }

    for (const auto& line : KL.getLines()) {
        painter->drawLine(Point::toQPoint(line.start()), Point::toQPoint(line.end()));
    }

    for (const auto& line : SR.getLines()) {
        painter->drawLine(Point::toQPoint(line.start()), Point::toQPoint(line.end()));
    }

    for (const auto& line : QT.getLines()) {
        painter->drawLine(Point::toQPoint(line.start()), Point::toQPoint(line.end()));
    }

}

void Figure::transform(double **matrix) {
    A->transform(matrix);
    B->transform(matrix);
    C->transform(matrix);
    D->transform(matrix);
    E->transform(matrix);
    F->transform(matrix);
    G->transform(matrix);
    H->transform(matrix);
    I->transform(matrix);
    J->transform(matrix);
    K->transform(matrix);
    L->transform(matrix);
    M->transform(matrix);
    O->transform(matrix);
    P->transform(matrix);
    Q->transform(matrix);
    R->transform(matrix);
    S->transform(matrix);
    T->transform(matrix);

    BC.transform(matrix);
    FG.transform(matrix);
    IJ.transform(matrix);
    KL.transform(matrix);
    SR.transform(matrix);
    QT.transform(matrix);
}



