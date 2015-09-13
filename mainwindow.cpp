#include "mainwindow.h"

#include "photo.h"

#include <stdio.h>

#include <QImage>
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    bool camInit = eye.initCamera();

    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    layout->addWidget(&label);
    label.setMinimumSize(800, 600);

    QPushButton *previewBtn = new QPushButton;
    previewBtn->setText("Preview");
    connect(previewBtn, SIGNAL(clicked()), this, SLOT(togglePreview()));
    previewBtn->setEnabled(camInit);
    layout->addWidget(previewBtn, 1, Qt::AlignBottom | Qt::AlignHCenter);

    QPushButton *captureBtn = new QPushButton;
    captureBtn->setText("Cheese!");
    connect(captureBtn, SIGNAL(clicked()), this, SLOT(startScene()));
    captureBtn->setEnabled(camInit);
    layout->addWidget(captureBtn, 1, Qt::AlignBottom | Qt::AlignHCenter);

    connect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
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

void MainWindow::startScene()
{
    disconnect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
    sceneTimer.start(1500);
    connect(&sceneTimer, SIGNAL(timeout()), this, SLOT(takeScenePicture()));
}

void MainWindow::takeScenePicture()
{
    QImage image = eye.takePicture();
    label.setPixmap(QPixmap::fromImage(image).scaled(label.width(), label.height(), Qt::KeepAspectRatio));
    scene.push_back(image);

    if(scene.size() > 3)
        endScene();
}

void MainWindow::endScene()
{
    sceneTimer.stop();
    disconnect(&sceneTimer, SIGNAL(timeout()), this, SLOT(takeScenePicture()));

    Photo photo;
    photo.addImage(scene);
    photo.save("/tmp/photo.pdf");

    connect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
}

void MainWindow::startPreview()
{
    previewTimer.start(100);
}

void MainWindow::stopPreview()
{
    previewTimer.stop();
}
