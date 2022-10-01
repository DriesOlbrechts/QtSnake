#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QPalette>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_colorPicker_clicked();


    void on_startGame_clicked();

    void on_selectFile_clicked();
    void gameLost();

private:
    Ui::MainWindow *ui;
    void setButtonColor(QPushButton*, QColor);
};
#endif // MAINWINDOW_H
