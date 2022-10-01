#include "gamecontroller.h"
#include "snakehead.h"
#include "food.h"
#include <QGridLayout>
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

GameController::GameController(QWidget *parent)
    : QGraphicsView(parent)
{
    this->setBackgroundBrush(Qt::green);
    scene = new QGraphicsScene(0,0,800,600);
    fitInView(scene->sceneRect(), Qt::KeepAspectRatio);

    setScene(scene);
}
GameController::~GameController() {
    delete snakehead;
}

void GameController::start(){
    SnakeHead* snakeHead = new SnakeHead();
    scene->addItem(snakeHead);
    snakeHead->setFocus();

    Food* f = new Food();
    f->setRandomPos();
    scene->addItem(f);


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]{
        snakeHead->handleDirection();
    } );
    connect(snakeHead, &SnakeHead::scoreChanged, this, [=]{
        timer->start(1000 / (5 + (snakeHead->getScore())/5));
    });
    connect(this, &GameController::gameLost, this, &GameController::handleLost);
    this->snakehead = snakeHead;
    timer->start(1000/ (5 + (snakeHead->getScore())/3));
}
void GameController::handleLost(){
  timer->stop();
  int topScore = 0;
  QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
  if(!dir.exists()) dir.mkpath(".");
  QFile scoreFile(this->filePath);

  if (scoreFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
    QTextStream score(&scoreFile);
    QString line = score.readLine();
    if(line != "") {
        topScore = line.toInt();
        this->highScore = topScore;
    }
    if(this->score > topScore){
        scoreFile.resize(0);
        scoreFile.write(QString::number(this->score).toUtf8());
        this->newhigh = true;
        this->highScore = this->score;
    }

    scoreFile.close();
  }
  emit scoreSaved();
}
