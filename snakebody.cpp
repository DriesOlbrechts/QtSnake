#include "snakebody.h"
#include <QBrush>
#include "gamecontroller.h"

extern GameController *game;
SnakeBody::SnakeBody(QGraphicsItem *parent){
    // draw
    setRect(0,0,50,50);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(game->color);
    setBrush(brush);
}
