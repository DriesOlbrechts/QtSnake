#ifndef SNAKEHEAD_H
#define SNAKEHEAD_H

#include "food.h"
#include "snakebody.h"
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QList>
#include <QPointF>

class SnakeHead: public QObject, public QGraphicsRectItem{
   Q_OBJECT
public:    // constructors
    SnakeHead(QGraphicsItem* parent=NULL);

    //destructor
    ~SnakeHead();

    //enum
    enum Direction{
        UP,
        RIGHT,
        DOWN,
        LEFT,
        NONE,
    };

    Direction currentDir = Direction::NONE;
    // events
    void keyPressEvent(QKeyEvent* event);

    // public method
    void elongate();
    void moveBodies();
    void handleDirection();
    void changeDirection(Direction);
    int getScore();
private:
    QList<SnakeBody*> snakeBodies;
    QPointF prevPos;
    bool handlingInput = false;
    bool started = false;
    Food* food;
signals:
   void scoreChanged();
};

#endif // SNAKEHEAD_H
