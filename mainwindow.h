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
#include <QHBoxLayout>
#include <QVBoxLayout>

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

    void takeScenePicture();

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
    QTimer stateTimer;

private:
    QPushButton *previewBtn;
    QLabel *copyCountLabel;
    QAction *selectPrinterAction, *loadSceneAction,
      *setWorkDirectoryPathAction, *setFileBaseNameAction, *quitApplicationAction;
    QWidget printWidget;
    QWidget stdWidget;

    QHBoxLayout mainLayout;
    QVBoxLayout printLayout;
    QHBoxLayout stdLayout;

    QImage idle, loading, noPrinter, noCamera, wait, smile, printing, countdown[3];

    int copyCount;

    void createStdLayout();
    void createPrintLayout();
    void createSimplePrintLayout();
    void initActions();

    void showPrintWidget();
    void showStdWidget();

    void startPreview();
    void stopPreview();

    void setCopyCount(int _cnt);

    void contextMenuEvent(QContextMenuEvent *_event);

    enum State
    {
        IDLE = 0,
        SELECT_SCENE,
        TAKE_PREVIEW,
        TAKE_PIC_PREVIEW,
        SMILE,
        TAKE_PIC,
        PAUSE_PIC,
        PRINT,
        SELECT_COPY_COUNT,
        PRINTING,

        ERROR_CAMERA,
        ERROR_PRINTER,
    } state;

private slots:
    void stateChange();
    void setCopyCountAndChangeState(int _cnt);
    void setNoCopyAndChangeState(void);
    void setOneCopyAndChangeState(void);
    void setTwoCopiesAndChangeState(void);
    void setFourCopiesAndChangeState(void);

    void incCopyCount();
    void decCopyCount();
};

#endif // MAINWINDOW_H
