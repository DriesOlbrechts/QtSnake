#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QGraphicsView>
#include <QGraphicsScene>

#include <QWidget>

class GameController : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GameController(QWidget *parent = nullptr);
    void start();
    QGraphicsScene* scene;

signals:

};

#endif // GAMECONTROLLER_H
