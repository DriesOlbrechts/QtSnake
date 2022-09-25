#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "gamecontroller.h"
GameController *game;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPushButton* button = ui->colorPicker;
    QString resource = ":/media/color-picker.png";
    QPixmap pixIcon(resource);
    button->setIcon(QIcon(pixIcon));
    setButtonColor(button, Qt::green);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_colorPicker_clicked()
{
   QColor color = QColorDialog::getColor();
   setButtonColor(ui->colorPicker, color);
}


void MainWindow::setButtonColor(QPushButton* button, QColor color){
   QPalette palette = button->palette();
   palette.setColor(QPalette::Button, color);
   button->setAutoFillBackground(true);
   button->setPalette(palette);
   button->update();
}

void MainWindow::on_startGame_clicked()
{
   game = new GameController();
   game->show();
   game->raise();
   game->setFocus();
   game->start();
}

