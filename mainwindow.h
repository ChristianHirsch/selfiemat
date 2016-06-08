#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "eye.h"
#include "scene.h"

#include <vector>

#include <QWidget>
#include <QLabel>
#include <QPushButton>
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
    void findAndInitCamera();

    void startScene();
    void takeScenePicture();
    void endScene();

protected:
    Eye eye;
    QLabel label;

    QTimer previewTimer;
    QTimer sceneTimer;
    QTimer camInitTimer;

private:
    Scene scene;
    QPushButton *previewBtn, *captureBtn;
    QAction *selectPrinterAction, *loadSceneAction,
      *setWorkDirectoryPathAction, *setFileBaseNameAction;

    void initActions();

    void startPreview();
    void stopPreview();

    void contextMenuEvent(QContextMenuEvent *_event);
};

#endif // MAINWINDOW_H
