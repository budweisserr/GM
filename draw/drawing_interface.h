#pragma once
#ifndef DRAWING_INTERFACE_H
#define DRAWING_INTERFACE_H

#include <QPainter>

class Drawable {
    //pure virtual class
public:
    virtual ~Drawable() = default;

    virtual void draw(QPainter* painter) = 0;
};



#endif //DRAWING_INTERFACE_H
