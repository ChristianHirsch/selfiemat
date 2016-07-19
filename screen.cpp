#include "screen.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>

Screen::Screen(const QString &text, QWidget *parent)
{
    setText(text);
}

Screen::~Screen()
{

}

void Screen::drawSelectArrows()
{
    QPixmap pmap(width(), height());

    QPainter painter;
    painter.begin(&pmap);

    painter.fillRect(0, 0, width(), height(), Qt::black);

    QRectF rectL = QRectF(0, 0, width() / 5, height());
    QRectF rectR = QRectF(width() - width() / 5, 0, width() / 5, height());

    QPainterPath triangleL, triangleR;

    triangleL.moveTo(rectL.topRight());
    triangleL.lineTo(0, rectL.height() / 2);
    triangleL.lineTo(rectL.bottomRight());
    triangleL.lineTo(rectL.topRight());

    triangleR.moveTo(rectR.topLeft());
    triangleR.lineTo(rectR.right(), rectR.height() / 2);
    triangleR.lineTo(rectR.bottomLeft());
    triangleR.lineTo(rectR.topLeft());

    QRectF target((width() - pixmap()->width()) / 2, 0, pixmap()->width(), pixmap()->height()),
           source(0, 0, pixmap()->width(), pixmap()->height());
    painter.drawPixmap(target, *pixmap(), source);

    QColor color(0, 0, 0, 200);
    painter.fillRect(rectL, color); 
    painter.fillRect(rectR, color);

    painter.fillPath(triangleL, QBrush(QColor ("white")));
    painter.fillPath(triangleR, QBrush(QColor ("white")));

    painter.end();

    setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    setPixmap(pmap);
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
