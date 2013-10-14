#include "picturelabel.h"

PictureLabel::PictureLabel(QWidget *parent) :
    QLabel(parent)
{
}

void PictureLabel::mousePressEvent(QMouseEvent *ev)
{
    Rectangle = QRect(ev->pos(),ev->pos());

    TrackingRectangle = true;
}

void PictureLabel::mouseMoveEvent(QMouseEvent *ev)
{
    Rectangle.moveBottomRight(ev->pos());

    DrawRectangle = true;
}

void PictureLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    if(!DrawRectangle)
    {
        Rectangle.setTopLeft(QPoint(0,0));
        Rectangle.setBottomRight(QPoint(0,0));
    }

    TrackingRectangle = false;
    DrawRectangle = false;
}
