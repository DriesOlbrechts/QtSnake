#include "food.h"
#include "gamecontroller.h"
#include <QApplication>
#include <QBrush>
#include <QDebug>
#include <QRandomGenerator>
extern GameController *game;
Food::Food(QGraphicsItem *parent) {
  // draw
  setRect(0, 0, 50, 50);
  QBrush brush;
  brush.setStyle(Qt::SolidPattern);
  brush.setColor(Qt::red);
  setBrush(brush);
}

void Food::setRandomPos() {
  this->hide();
  int maxX = game->viewport()->width();
  int maxY = game->viewport()->height();
  bool valid = false;
  int count = 0;
  int randomX, randomY;
  while (!valid) {
    // TODO: better way of figuring out when no spots
    // available
    if (count >= 200) {
        emit game->gameLost();
    }
    randomX = QRandomGenerator::global()->bounded(50, maxX - 50);
    randomY = QRandomGenerator::global()->bounded(50, maxY - 50);
    randomX = randomX + (50 -  (randomX % 50 ?: 50));
    randomY = randomY + (50 -  (randomY % 50 ?: 50));

    this->setPos(randomX, randomY);
    QList<QGraphicsItem *> cItems = collidingItems();
    if (cItems.length() == 0) {
      valid = true;
    }
    count++;
  }
  this->show();
}
