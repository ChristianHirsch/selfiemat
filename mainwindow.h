#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "eye.h"

#include <vector>

#include <QWidget>
#include <QLabel>
#include <QTimer>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void updatePreview();
    void togglePreview();

    void startScene();
    void takeScenePicture();
    void endScene();

protected:
    Eye eye;
    QLabel label;

    QTimer previewTimer;
    QTimer sceneTimer;

private:
    std::vector<QImage> scene;

    void startPreview();
    void stopPreview();
};

#endif // MAINWINDOW_H
