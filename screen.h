#ifndef SCREEN_H
#define SCREEN_H

#include <QObject>
#include <QLabel>

class Screen : public QLabel
{
    Q_OBJECT

public:
    explicit Screen( const QString& text="", QWidget* parent=0 );
    ~Screen();

signals:
    void clicked();
    void clickedLeft();
    void clickedRight();
    void clickedCenter();

protected:
    void mousePressEvent(QMouseEvent *ev);
};

#endif // SCREEN_H
