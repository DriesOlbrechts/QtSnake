#include "food.h"
#include <QBrush>
Food::Food(QGraphicsItem *parent)
{
    // draw
    setRect(0,0,45,45);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::red);
    setBrush(brush);
}
