#include "gamecontroller.h"
#include "food.h"
#include "snakehead.h"
#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QGridLayout>
#include <QStandardPaths>
#include <QTimer>
#include <iostream>
GameController::GameController(QWidget *parent) : QGraphicsView(parent) {
  this->setBackgroundBrush(Qt::green);
  scene = new QGraphicsScene(0, 0, 800, 600);
  fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

  setScene(scene);
}
GameController::~GameController() { delete snakehead; }
void GameController::keyPressEvent(QKeyEvent *event) {

  if (this->snakehead->handlingInput)
    return;
  this->snakehead->handlingInput = true;
  // move up
  if ((event->key() == Qt::Key_Up || event->key() == Qt::Key_W ||
       event->key() == Qt::Key_K) &&
      this->snakehead->currentDir != this->snakehead->Direction::DOWN) {
    this->snakehead->changeDirection(this->snakehead->Direction::UP);
  }

  // move down
  if ((event->key() == Qt::Key_Down || event->key() == Qt::Key_S ||
       event->key() == Qt::Key_J) &&
      this->snakehead->currentDir != this->snakehead->Direction::UP) {
    this->snakehead->changeDirection(this->snakehead->Direction::DOWN);
  }

  // move right
  if ((event->key() == Qt::Key_Right || event->key() == Qt::Key_D ||
       event->key() == Qt::Key_L) &&
      this->snakehead->currentDir != this->snakehead->Direction::LEFT) {
    this->snakehead->changeDirection(this->snakehead->Direction::RIGHT);
  }

  // move left
  if ((event->key() == Qt::Key_Left || event->key() == Qt::Key_A ||
       event->key() == Qt::Key_H) &&
      this->snakehead->currentDir != this->snakehead->Direction::RIGHT) {
    this->snakehead->changeDirection(this->snakehead->Direction::LEFT);
  }
}

void GameController::start() {
  SnakeHead *snakeHead = new SnakeHead();
  scene->addItem(snakeHead);
  snakeHead->setFocus();

  Food *f = new Food();
  f->setRandomPos();
  scene->addItem(f);

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, [=] {
      snakeHead->handleDirection();
  });

  connect(snakeHead, &SnakeHead::scoreChanged, this,
          [=] { timer->start(1000 / (5 + (snakeHead->getScore()) / 5)); });

  connect(this, &GameController::gameLost, this, &GameController::handleLost);

  this->snakehead = snakeHead;
  timer->start(1000 / (5 + (snakeHead->getScore()) / 3));
}
void GameController::handleLost() {
  timer->stop();
  int topScore = 0;
  QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
  if (!dir.exists())
    dir.mkpath(".");
  QFile scoreFile(this->filePath);

  if (scoreFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
    QTextStream score(&scoreFile);
    QString line = score.readLine();
    if (line != "") {
      topScore = line.toInt();
      this->highScore = topScore;
    }
    if (this->score > topScore) {
      scoreFile.resize(0);
      scoreFile.write(QString::number(this->score).toUtf8());
      this->newhigh = true;
      this->highScore = this->score;
    }

    scoreFile.close();
  }
  emit scoreSaved();
}
