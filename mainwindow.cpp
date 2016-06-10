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

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    layout->addWidget(&screen);
    screen.setMinimumSize(800, 600);

    previewBtn = new QPushButton;
    previewBtn->setText("Preview");
    connect(previewBtn, SIGNAL(clicked()), this, SLOT(togglePreview()));
    previewBtn->setEnabled(false);
    layout->addWidget(previewBtn, 1, Qt::AlignBottom | Qt::AlignHCenter);

    connect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    connect(&camInitTimer, SIGNAL(timeout()), this, SLOT(findAndInitCamera()));
    camInitTimer.start(1000);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    initActions();

    QDir workDir(Common::getWorkDirectoryPath());
    if(!workDir.mkpath(Common::getWorkDirectoryPath()))
        printf("Error creating working directory \"%s\"", workDir.absolutePath().toStdString().c_str());
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

    connect(&screen, SIGNAL(clickedCenter()), this, SLOT(startScene()));
    connect(&screen, SIGNAL(clickedRight()), this, SLOT(selectAndShowNextScene()));
    connect(&screen, SIGNAL(clickedLeft()), this, SLOT(selectAndShowPrevScene()));

    previewBtn->setEnabled(true);

    camInitTimer.stop();
}

void MainWindow::startScene()
{
    disconnect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    Common::getScene()->clear();
    sceneTimer.start(1500);
    connect(&sceneTimer, SIGNAL(timeout()), this, SLOT(takeScenePicture()));
}

void MainWindow::takeScenePicture()
{
    if(Common::getScene()->imagesToAdd() <= 0)
        return endScene();

    QImage image = eye.takePicture();
    screen.setPixmap(QPixmap::fromImage(image).scaled(screen.width(), screen.height(), Qt::KeepAspectRatio));
    Common::getScene()->addImage(image);
}

void MainWindow::endScene()
{
    sceneTimer.stop();
    disconnect(&sceneTimer, SIGNAL(timeout()), this, SLOT(takeScenePicture()));

    Common::getScene()->paint();
    showImage(Common::getScene()->getSceneImage());

    Photo photo;
    photo.setScene(Common::getScene());
    photo.print();

    connect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
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
    previewTimer.start(200);
}

void MainWindow::stopPreview()
{
    previewTimer.stop();
}

void MainWindow::contextMenuEvent(QContextMenuEvent *_event)
{
    QMenu menu(this);

    menu.addAction(loadSceneAction);
    menu.addAction(selectPrinterAction);
    menu.addAction(setWorkDirectoryPathAction);
    menu.addAction(setFileBaseNameAction);
    menu.exec(_event->globalPos());
}
