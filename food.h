#ifndef FOOD_H
#define FOOD_H

#include <QGraphicsEllipseItem>

class Food: public QGraphicsEllipseItem{
public:
    Food(QGraphicsItem* parent=NULL);
    void setRandomPos();
};
#endif // FOOD_H
