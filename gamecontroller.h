#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "snakehead.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFile>

#include <QWidget>

class GameController : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GameController(QWidget *parent = nullptr);
    ~GameController();
    void start();
    QGraphicsScene* scene;
    QColor color = Qt::cyan;
    QString filePath;

    int score = 0;
    bool newhigh = false;
    int highScore = 0;
private:
    QTimer *timer;
    SnakeHead* snakehead;
signals:
   void gameLost();
   void scoreSaved();
private slots:
   void handleLost();
};

#endif // GAMECONTROLLER_H
