#include "gamecontroller.h"
#include "snakehead.h"
#include "food.h"

GameController::GameController(QWidget *parent)
    : QGraphicsView(parent)
{
    this->setBackgroundBrush(Qt::green);
    scene = new QGraphicsScene(0,0,800,600);
    setScene(scene);
}


void GameController::start(){
    SnakeHead* snakeHead = new SnakeHead();
    scene->addItem(snakeHead);
    snakeHead->setFocus();

    Food* f = new Food();
    f->setPos(300,300);
    scene->addItem(f);
}
