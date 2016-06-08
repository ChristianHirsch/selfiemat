#include "mainwindow.h"

#include "photo.h"

#include <stdio.h>

#include <QAction>
#include <QContextMenuEvent>
#include <QDateTime>
#include <QMenu>
#include <QImage>
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent),
    scene(1480,1000)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    layout->addWidget(&label);
    label.setMinimumSize(800, 600);

    previewBtn = new QPushButton;
    previewBtn->setText("Preview");
    connect(previewBtn, SIGNAL(clicked()), this, SLOT(togglePreview()));
    previewBtn->setEnabled(false);
    layout->addWidget(previewBtn, 1, Qt::AlignBottom | Qt::AlignHCenter);

    captureBtn = new QPushButton;
    captureBtn->setText("Cheese!");
    connect(captureBtn, SIGNAL(clicked()), this, SLOT(startScene()));
    captureBtn->setEnabled(false);
    layout->addWidget(captureBtn, 1, Qt::AlignBottom | Qt::AlignHCenter);

    connect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    connect(&camInitTimer, SIGNAL(timeout()), this, SLOT(findAndInitCamera()));
    camInitTimer.start(1000);

    QPalette pal(palette());
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    scene.initialize();
    initActions();
}

MainWindow::~MainWindow()
{
}

void MainWindow::updatePreview()
{
    QImage image = eye.takePreview();
    label.setPixmap(QPixmap::fromImage(image).scaled(label.width(), label.height(), Qt::KeepAspectRatio));
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

    previewBtn->setEnabled(true);
    captureBtn->setEnabled(true);

    camInitTimer.stop();
}

void MainWindow::startScene()
{
    disconnect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    scene.clear();
    sceneTimer.start(1500);
    connect(&sceneTimer, SIGNAL(timeout()), this, SLOT(takeScenePicture()));
}

void MainWindow::takeScenePicture()
{
    if(scene.imagesToAdd() <= 0)
        return endScene();

    QImage image = eye.takePicture();
    label.setPixmap(QPixmap::fromImage(image).scaled(label.width(), label.height(), Qt::KeepAspectRatio));
    scene.addImage(image);
}

void MainWindow::endScene()
{
    sceneTimer.stop();
    disconnect(&sceneTimer, SIGNAL(timeout()), this, SLOT(takeScenePicture()));

    scene.paint();
    label.setPixmap(QPixmap::fromImage(scene.getSceneImage()).scaled(label.width(), label.height(), Qt::KeepAspectRatio));

    Photo photo;
    photo.setScene(scene);
    photo.print();

    connect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
}

void MainWindow::initActions()
{
    selectPrinterAction = new QAction(tr("Select &Printer"), this);
    connect(selectPrinterAction, &QAction::triggered, Photo::selectPrinter);
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

    //menu.addAction(loadSceneAction);
    menu.addAction(selectPrinterAction);
    menu.exec(_event->globalPos());
    //menu.exec(_event->globalPos());
}
