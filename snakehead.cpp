#include "snakehead.h"
#include "food.h"
#include "gamecontroller.h"
#include "snakebody.h"
#include <QBrush>
#include <QDebug>
#include <QApplication>

extern GameController *game;

SnakeHead::SnakeHead(QGraphicsItem *parent) {

  // draw
  setRect(0, 0, 50, 50);
  QBrush brush;
  brush.setStyle(Qt::SolidPattern);
  brush.setColor(game->color.darker(200));
  setBrush(brush);

  // make focusable
  setFlag(QGraphicsItem::ItemIsFocusable, true);

  SnakeBody *b1 = new SnakeBody();
  b1->setPos(0, 50);
  game->scene->addItem(b1);
  snakeBodies.prepend(b1);
}

SnakeHead::~SnakeHead(){
    for(SnakeBody* body : snakeBodies){
        delete body;
    }
    delete food;
}


int SnakeHead::getScore(){
    return game->score;
}
void SnakeHead::keyPressEvent(QKeyEvent *event) {

  if(handlingInput) return;
  handlingInput = true;
  // move up
  if (event->key() == Qt::Key_Up && this->currentDir != Direction::DOWN) {
      changeDirection(Direction::UP);
     }

  // move down
  if (event->key() == Qt::Key_Down && this->currentDir != Direction::UP) {
      changeDirection(Direction::DOWN);
  }


  // move right
  if (event->key() == Qt::Key_Right && this->currentDir != Direction::LEFT) {
      changeDirection(Direction::RIGHT);
  }

  // move left
  if (event->key() == Qt::Key_Left && this->currentDir != Direction::RIGHT) {
      changeDirection(Direction::LEFT);
  }

}

void SnakeHead::elongate() {
  // add new SnakeBody to list
  SnakeBody *body = new SnakeBody();
  snakeBodies.prepend(body);

  // properly position the body
  body->setPos(-200, -200); // TODO
  game->scene->addItem(body);
}

void SnakeHead::moveBodies() {
    if(!this->started){
        this->started = true;
    }
  // traverses through list of bodies and moves them properly

  // move all bodies from back to front
  if (snakeBodies.size() > 0) {
    for (size_t i = 0, n = snakeBodies.size() - 1; i < n; ++i) {
      snakeBodies[i]->setPos(snakeBodies[i + 1]->pos());
    }

    // move front body to previous position of head
    snakeBodies.last()->setPos(prevPos);
  }
}
void SnakeHead::handleDirection() {
  prevPos = pos();
  int xPos, yPos;
  switch (currentDir) {
  case Direction::UP:
    xPos = x();
    yPos = y() - boundingRect().height();
    break;
  case Direction::RIGHT:
    xPos = x() + boundingRect().width();
    yPos = y();
    break;
  case Direction::DOWN:
    xPos = x();
    yPos = y() + boundingRect().height();
    break;
  case Direction::LEFT:
    xPos = x() - boundingRect().width();
    yPos = y();
    break;
  case Direction::NONE:
    xPos = x();
    yPos = y();
  default:
      break;
  }
  QPointF newPos(xPos,yPos);
  setPos(newPos);
  if(currentDir != Direction::NONE) moveBodies();

  // elongate if collides with Fruit
  QList<QGraphicsItem *> cItems = collidingItems();
  for (size_t i = 0, n = cItems.size(); i < n; ++i) {
    QGraphicsItem &temp = *cItems[i];
    if (typeid(temp) == typeid(Food)) {
      // fruit found in collision list
      elongate();
      this->food = dynamic_cast<Food*>(cItems[i]);
      game->score++;
      emit scoreChanged();
      if(this->food->isVisible()){
        this->food->setRandomPos();
      }
    }
    else if (typeid(temp) == typeid(SnakeBody) && this->started){
         emit game->gameLost();
         return;
    }
  }

  QPointF centerHead = mapToScene(boundingRect().center());
  qreal rightBorder = game->scene->sceneRect().right();
  qreal leftBorder = game->scene->sceneRect().left();
  qreal topBorder = game->scene->sceneRect().top();
  qreal botBorder = game->scene->sceneRect().bottom();

  // border collision logic
  if (centerHead.x() > rightBorder || centerHead.x() < leftBorder ||
      centerHead.y() > botBorder || centerHead.y() < topBorder) {
    switch (currentDir) {
    case Direction::UP:
      xPos = x();
      yPos = botBorder - boundingRect().height();
      break;
    case Direction::RIGHT:
      xPos = leftBorder;
      yPos = y();
      break;
    case Direction::DOWN:
      xPos = x();
      yPos = topBorder;
      break;
    case Direction::LEFT:
      xPos = rightBorder - boundingRect().width();
      yPos = y();
      break;
    case Direction::NONE:
      xPos = x();
      yPos = y();
    default:
      break;
    }
    QPointF opposite(xPos,yPos);
    setPos(opposite);
    if (currentDir != Direction::NONE)
      moveBodies();
  }

  handlingInput = false;
}

void SnakeHead::changeDirection(Direction direction){
   this->currentDir = direction;
}
