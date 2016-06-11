#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "eye.h"
#include "scene.h"
#include "screen.h"

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

    void loadScene();
    void selectAndShowNextScene();
    void selectAndShowPrevScene();

    void showImage(const QImage &_image);

    void openContextMenu(const QPoint &_pos);

protected:
    Eye eye;
    Screen screen;

    QTimer previewTimer;
    QTimer sceneTimer;
    QTimer camInitTimer;

private:
    QPushButton *previewBtn;
    QAction *selectPrinterAction, *loadSceneAction,
      *setWorkDirectoryPathAction, *setFileBaseNameAction;

    void initActions();

    void startPreview();
    void stopPreview();

    void contextMenuEvent(QContextMenuEvent *_event);
};

#endif // MAINWINDOW_H
