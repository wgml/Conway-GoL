#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include "cell.h"
#include <QTimer>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    game = NULL;
    ui->statusBar->showMessage(QString::fromUtf8("Załadowano"));
    tick = 0;
    scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFrameStyle(1);
    unsigned sizeX = (ui->sizeBox->currentIndex() + 1) * 20;
    unsigned sizeY = (ui->sizeBox->currentIndex() + 1) * 10;
    map = new QVector< QVector<Cell*> *>(sizeY);
    for(unsigned i = 0; i < sizeY; i++)
        (*map)[i] = new QVector<Cell*>(sizeX);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(makeTick()));

    setScene();
}

MainWindow::~MainWindow()
{
    delete ui;
    if(game != NULL)
        delete game;
    if(scene != NULL)
        delete scene;
}

void MainWindow::on_startButton_clicked()
{
    if(timer->isActive())
    {
         //konczenie
        timer->stop();
        ui->speedBox->setEnabled(true);
        ui->speedSlider->setEnabled(true);
        ui->tickButton->setEnabled(true);
        ui->sizeBox->setEnabled(true);
        ui->loadButton->setEnabled(true);
        ui->saveButton->setEnabled(true);
        ui->graphicsView->setEnabled(true);
    }
    else
    {
        timer->start(1000/ui->speedBox->value());
        ui->speedBox->setEnabled(false);
        ui->speedSlider->setEnabled(false);
        ui->tickButton->setEnabled(false);
        ui->sizeBox->setEnabled(false);
        ui->loadButton->setEnabled(false);
        ui->saveButton->setEnabled(false);
        ui->graphicsView->setEnabled(false);
    }

}

void MainWindow::on_tickButton_clicked()
{
    makeTick();
}

void MainWindow::changeSize()
{
    if(game != NULL)
        delete game;
    game = new Game((ui->sizeBox->currentIndex()+1)*20, (ui->sizeBox->currentIndex()+1)*10);

    map = new QVector< QVector<Cell*> *>((ui->sizeBox->currentIndex()+1)*10);
    for(int i = 0; i < (ui->sizeBox->currentIndex()+1)*10; i++)
        (*map)[i] = new QVector<Cell*>((ui->sizeBox->currentIndex()+1)*20);
    tick = 0;
    setScene();
}

void MainWindow::makeTick()
{
   if(game == NULL)
   {
        //tworzenie gry w razie potrzeby

        game = new Game((ui->sizeBox->currentIndex()+1)*20, (ui->sizeBox->currentIndex()+1)*10);
        for(unsigned i = 0; i < game->getHeight(); i++)
            for(unsigned j = 0; j < game->getWidth(); j++)
                game->setState(j, i, map->at(i)->at(j)->getState());
        tick = 0;
   }
   /*
    sprawdzanie, czy stan mapy w grze zgadza sie ze stanem mapy w map
    */
   for(unsigned i = 0; i < game->getHeight(); i++)
       for(unsigned j = 0; j < game->getWidth(); j++)
           if(game->getState(j, i) != map->at(i)->at(j)->getState())
               game->setState(j, i, map->at(i)->at(j)->getState());

   if(game->isPlayable())
   {
       game->tick();
       tick++;
       ui->statusBar->showMessage(QString("Tick: %1").arg(tick));
       updateScene();
   }
   else
   {
       ui->statusBar->showMessage(QString("Symulacja zakonczona po %1 tickach.").arg(tick));
       if(timer && timer->isActive())
           ui->startButton->click();
   }
}

void MainWindow::setScene(void)
{
    if(scene != NULL)
        delete scene;


    unsigned sizeX = (ui->sizeBox->currentIndex() + 1) * 20;
    unsigned sizeY = (ui->sizeBox->currentIndex() + 1) * 10;

    ui->graphicsView->setGeometry(QRect(-800, -400, 800, 400));
    unsigned cellX = ui->graphicsView->geometry().width() / sizeX;
    unsigned cellY = ui->graphicsView->geometry().height() / sizeY;
    ui->statusBar->showMessage(QString("%1 %2, %3 %4").arg(sizeX).arg(sizeY).arg(cellX).arg(cellY));
    scene = new QGraphicsScene(-ui->graphicsView->geometry().width()/2, -ui->graphicsView->geometry().height()/2,
                               ui->graphicsView->geometry().width()+10, ui->graphicsView->geometry().height());
    ui->graphicsView->setScene(scene);
    Cell * el;
    for(unsigned i = 0; i < sizeY; i++)
    {
        for(unsigned j = 0; j < sizeX; j++)
        {
            el = new Cell(cellX * j, cellY * i, cellX, cellY);
            scene->addItem(el);
            (*map->at(i))[j] = el;
        }
    }
}

void MainWindow::updateScene(void)
{
    if(!game || !scene || !map)
        ui->statusBar->showMessage("Cos nie wyszlo");
    else
        for(unsigned i = 0; i < game->getHeight(); i++)
            for(unsigned j = 0; j < game->getWidth(); j++)
                map->at(i)->at(j)->setState(game->getState(j, i));
}

void MainWindow::on_sizeBox_currentIndexChanged(int)
{
    changeSize();
}

void MainWindow::on_saveButton_clicked()
{
    if(tick == 0)
    {
        QMessageBox msg;
        msg.setText("0. tick");
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName();
        QFile saveFile(fileName);
        saveFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
        QTextStream txt(&saveFile);

        txt << (ui->sizeBox->currentIndex());
        for(unsigned i = 0; i < game->getHeight(); i++)
            for(unsigned j = 0; j < game->getWidth(); j++)
                txt << " " << (game->getState(j, i) ? 1 : 0);
        saveFile.close();
    }
}

void MainWindow::on_loadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName();
    if(fileName == "")
        return;
    QFile loadFile(fileName);
    loadFile.open(QIODevice::ReadOnly);
    QTextStream txt(&loadFile);
    QString tmp;

    QMessageBox msg;//just in case...
    msg.setText("Niepoprawny plik");
    msg.setStandardButtons(QMessageBox::Ok);

    txt >> tmp;
    if(tmp.toUInt() > 3)
    {
        msg.exec();
        return;
    }

    ui->sizeBox->setCurrentIndex(tmp.toInt());
    ui->sizeBox->update();

    setScene();

    for(int i = 0; i < (ui->sizeBox->currentIndex() + 1) * 10; i++)
    {
        for(int j = 0; j < (ui->sizeBox->currentIndex() + 1) * 20; j++)
        {
            txt >> tmp;
            if(tmp.toUInt() != 0 && tmp.toUInt() != 1)
            {
                msg.exec();
                return;
            }
            map->at(i)->at(j)->setState(((tmp.toInt() == 1) ? true : false));
        }
    }
    loadFile.close();
}
