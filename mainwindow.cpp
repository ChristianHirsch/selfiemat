#include "mainwindow.h"

#include "photo.h"

#include <stdio.h>

#include <QDateTime>
#include <QImage>
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
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
    scene.empty();
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
    QString fileName = "/home/foreman/"
            + QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    photo.saveImages(fileName.toStdString());
    photo.setFileName(fileName);
    photo.save(fileName.toStdString() + ".pdf");

    connect(&previewTimer, SIGNAL(timeout()), this, SLOT(updatePreview()));
}

void MainWindow::startPreview()
{
    previewTimer.start(200);
}

void MainWindow::stopPreview()
{
    previewTimer.stop();
}
