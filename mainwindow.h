#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class Game;
class QTimer;
class QGraphicsScene;
class Cell;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    Game * game;
    QTimer * timer;
    QGraphicsScene * scene;
    QVector< QVector<Cell*> *> * map;
    unsigned tick;

    void setScene();
    void updateScene();
    void changeSize();


private slots:
    void on_tickButton_clicked();
    void on_startButton_clicked();
    void on_saveButton_clicked();
    void on_loadButton_clicked();
    void makeTick();

    void on_sizeBox_currentIndexChanged(int);
};

#endif // MAINWINDOW_H
