#ifndef SCREEN_H
#define SCREEN_H

#include <QObject>
#include <QLabel>
#include <QTime>

class Screen : public QLabel
{
    Q_OBJECT

public:
    explicit Screen( const QString& text="", QWidget* parent=0 );
    ~Screen();

    void drawSelectArrows();

signals:
    void clicked();
    void clickedLeft();
    void clickedRight();
    void clickedCenter();
    void contextClicked(const QPoint _pos);

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

private:
    QTime pressTimer;
};

#endif // SCREEN_H
