#include "cmouseevent.h"

CMouseEvent::CMouseEvent(QObject *parent)
{
    this->widget = (QP)parent;
}

void CMouseEvent::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMouseDown = true;
        mousePoint = event->globalPos();
    }
}

void CMouseEvent::mouseReleaseEvent(QMouseEvent *event)
{
    isMouseDown = false;
}

void CMouseEvent::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint delta = event->globalPos() - mousePoint;
    if(isMouseDown == true)
        widget.move(widget.x() + delta.x(), widget.y() + delta.y());
    else
        widget.move(widget.x()+delta.x(), widget.y()+delta.y());
       mousePoint = event->globalPos();
}




