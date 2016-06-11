#include "screen.h"

#include <QMouseEvent>

Screen::Screen(const QString &text, QWidget *parent)
{
    setText(text);
}

Screen::~Screen()
{

}

void Screen::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton)
        return;

    pressTimer.restart();
}

void Screen::mouseReleaseEvent(QMouseEvent *ev)
{
    if(pressTimer.elapsed() > 2000)
    {
        emit contextClicked(ev->globalPos());
        return;
    }

    emit clicked();
    if(ev->pos().x() < width()/5)
        emit clickedLeft();
    else if(ev->pos().x() > width() - width()/5)
        emit clickedRight();
    else
        emit clickedCenter();
    return;
}
