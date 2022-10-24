#ifndef CMOUSEEVENT_H
#define CMOUSEEVENT_H

#include <QMouseEvent>
#include "qpoint.h"
#include "qwidget.h"

class CMouseEvent
{
public:
    CMouseEvent() = default;
    ~CMouseEvent() = default;
public:
   void mousePressEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   QPoint mousePoint;
   bool isMouseDown = false;
};

#endif // CMOUSEEVENT_H
