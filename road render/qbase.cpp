#include "qbase.h"

QT_BEGIN_NAMESPACE

/*!
  \internal
*/
QBezier QBezier::fromPoints(const QPointF &p1, const QPointF &p2,
                            const QPointF &p3, const QPointF &p4)
{
    QBezier b;
    b.x1 = p1.x();
    b.y1 = p1.y();
    b.x2 = p2.x();
    b.y2 = p2.y();
    b.x3 = p3.x();
    b.y3 = p3.y();
    b.x4 = p4.x();
    b.y4 = p4.y();
    return b;
}


QT_END_NAMESPACE
