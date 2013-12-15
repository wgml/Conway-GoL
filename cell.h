#ifndef CELL_H
#define CELL_H

#include <QGraphicsRectItem>

class Cell : public QGraphicsRectItem
{
public:
    Cell(unsigned, unsigned, unsigned, unsigned);
    Cell(unsigned, unsigned, unsigned, unsigned, bool);
    void setState(bool);
    bool getState();

private:
    bool state;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private slots:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CELL_H
