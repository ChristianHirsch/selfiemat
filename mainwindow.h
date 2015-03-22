#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>

#include <gphoto2/gphoto2-abilities-list.h>
#include <gphoto2/gphoto2-camera.h>

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void takePicture();

protected:
    CameraAbilitiesList *al;
    CameraAbilities a;
    GPPortInfoList *il;
    GPPortInfo info;
    GPContext *cameraContext;

    CameraFilePath path;
    CameraFile *file;
    Camera *camera;

    const char *data;
    unsigned long size;

    QLabel label;

private:
    bool initCamera();
    void closeCamera();
};

#endif // MAINWINDOW_H