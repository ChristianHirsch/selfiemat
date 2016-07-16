#include "mainwindow.h"

#include "common.h"
#include "photo.h"

#include <stdio.h>

#include <QAction>
#include <QContextMenuEvent>
#include <QDateTime>
#include <QDir>
#include <QMenu>
#include <QImage>
#include <QVBoxLayout>
#include <QPushButton>

#define TIME_PREVIEW 200

MainWindow::MainWindow(QWidget *parent) : QWidget(parent),
    idle("icons/press.png"),
    smile("icons/smile.png")
{
    setMinimumSize(480, 320);

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    screen.setMinimumSize(240, 160);
    layout->addWidget(&screen);

    previewBtn = new QPushButton;
    previewBtn->setText("Preview");
    connect(previewBtn, SIGNAL(clicked()), this, SLOT(togglePreview()));
    previewBtn->setEnabled(false);
//    layout->addWidget(previewBtn, 1, Qt::AlignBottom | Qt::AlignHCenter);

    connect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    connect(&camInitTimer, SIGNAL(timeout()), this, SLOT(findAndInitCamera()));
    camInitTimer.start(1000);

    connect(&screen, SIGNAL(contextClicked(const QPoint &)), this, SLOT(openContextMenu(const QPoint &)));

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    initActions();

    QDir workDir(Common::getWorkDirectoryPath());
    if(!workDir.mkpath(Common::getWorkDirectoryPath()))
        printf("Error creating working directory \"%s\"", workDir.absolutePath().toStdString().c_str());

    stateTimer.setSingleShot(true);
}

MainWindow::~MainWindow()
{
    while(Common::getSceneCount() > 0)
        Common::scenePopBack();
}

void MainWindow::updatePreview()
{
    QImage image = eye.takePreview();
    screen.setPixmap(QPixmap::fromImage(image).scaled(screen.width(), screen.height(), Qt::KeepAspectRatio));
}

void MainWindow::togglePreview()
{
    if(previewTimer.isActive())
        stopPreview();
    else
        startPreview();
}

void MainWindow::findAndInitCamera()
{
    if(!eye.initCamera())
        return;

    camInitTimer.stop();
    stateTimer.stop();

    connect(&stateTimer, SIGNAL(timeout()), this, SLOT(stateChange()));

    state = IDLE;
    stateChange();
}

void MainWindow::takeScenePicture()
{
    QImage image = eye.takePicture();
    screen.setPixmap(QPixmap::fromImage(image).scaled(screen.width(), screen.height(), Qt::KeepAspectRatio));
    Common::getScene()->addImage(image);

    if(Common::getScene()->imagesToAdd() <= 0)
        state = PRINT;
    else
        state = TAKE_PIC_PREVIEW;
}

void MainWindow::loadScene()
{
    Scene *newScene = Common::loadSceneFromFile();
    showImage(newScene->getPreviewImage());
}

void MainWindow::selectAndShowNextScene()
{
    Scene *scene = Common::getNextScene();
    showImage(scene->getPreviewImage());
}

void MainWindow::selectAndShowPrevScene()
{
    Scene *scene = Common::getPrevScene();
    showImage(scene->getPreviewImage());
}

void MainWindow::showImage(const QImage &_image)
{
    QPixmap pixmap = QPixmap::fromImage(_image).scaled(screen.width(), screen.height(), Qt::KeepAspectRatio);
    screen.setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
    screen.setPixmap(pixmap);
}

void MainWindow::openContextMenu(const QPoint &_pos)
{
    QMenu menu(this);

    menu.addAction(loadSceneAction);
    menu.addAction(selectPrinterAction);
    menu.addAction(setWorkDirectoryPathAction);
    menu.addAction(setFileBaseNameAction);
    menu.exec(_pos);
}

void MainWindow::initActions()
{
    selectPrinterAction = new QAction(tr("Select &Printer"), this);
    connect(selectPrinterAction, &QAction::triggered, Photo::selectPrinter);

    setWorkDirectoryPathAction = new QAction(tr("Select &Working Directory"), this);
    connect(setWorkDirectoryPathAction, &QAction::triggered, Common::setWorkDirectoryPathWithDialog);

    setFileBaseNameAction = new QAction(tr("Select &File base name"), this);
    connect(setFileBaseNameAction, &QAction::triggered, Common::setFileBaseNameWithDialog);

    loadSceneAction = new QAction(tr("Load &Scene"), this);
    connect(loadSceneAction, SIGNAL(triggered(bool)), this, SLOT(loadScene()));
}

void MainWindow::startPreview()
{
    previewTimer.start(TIME_PREVIEW);
}

void MainWindow::stopPreview()
{
    previewTimer.stop();
}

void MainWindow::stateChange()
{
    switch(state)
    {
        case IDLE:
            printf("idle state\n");

            showImage(idle);
            Common::getScene()->clear();
            connect(&screen, SIGNAL(clickedCenter()), this, SLOT(stateChange()));

            state = SELECT_SCENE;
            break;

        case SELECT_SCENE:
            printf("select scene state\n");

            showImage(Common::getScene()->getPreviewImage());
            connect(&screen, SIGNAL(clickedRight()), this, SLOT(selectAndShowNextScene()));
            connect(&screen, SIGNAL(clickedLeft()), this, SLOT(selectAndShowPrevScene()));

            state = TAKE_PIC_PREVIEW;
            break;

        case TAKE_PREVIEW:
            break;

        case TAKE_PIC_PREVIEW:
            printf("take pic preview state\n");

            disconnect(&screen, SIGNAL(clickedRight()), this, SLOT(selectAndShowNextScene()));
            disconnect(&screen, SIGNAL(clickedLeft()), this, SLOT(selectAndShowPrevScene()));
            disconnect(&screen, SIGNAL(clickedCenter()), this, SLOT(stateChange()));

            startPreview();
            stateTimer.start(3000);
            state = SMILE;
            break;

        case SMILE:
            printf("take picture state\n");
            stopPreview();

            showImage(smile);

            state = TAKE_PIC;
            stateTimer.start(1000);
            break;

        case TAKE_PIC:
            printf("take picture state\n");

            takeScenePicture();
            stateTimer.start(1000);
            break;

        case PRINT:
            printf("print state\n");

            Common::getScene()->paint();
            showImage(Common::getScene()->getSceneImage());

            Photo photo;
            photo.setScene(Common::getScene());
            photo.print();

            state = IDLE;
            stateTimer.start(5000);
            break;
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *_event)
{
    openContextMenu(_event->globalPos());
}
