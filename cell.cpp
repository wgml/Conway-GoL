#include "cell.h"
#include <QBrush>
#include <QPainter>

Cell::Cell(unsigned x, unsigned y, unsigned w, unsigned h)
    : QGraphicsRectItem(x, y, w, h)
{
    state = false;
    update();
}

Cell::Cell(unsigned x, unsigned y, unsigned w, unsigned h, bool s)
    : QGraphicsRectItem(x, y, w, h)
{
    state = s;
    update();
}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    state = !state;
    update();
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QBrush background = QBrush((state) ? Qt::black : Qt::white);
    painter->drawRect(this->boundingRect());
    painter->fillRect(this->rect(), background);
}

bool Cell::getState()
{
    return state;
}

void Cell::setState(bool s)
{
    state = s;
    update();
}
