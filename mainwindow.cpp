#include "mainwindow.h"
#include <stdio.h>
#include <QImage>
#include <QVBoxLayout>
#include <QPushButton>

void error_func (GPContext *context, const char *format, va_list args, void *data) {
    fprintf(stderr, "*** Contexterror ***\n");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
}

void message_func (GPContext *context, const char *format, va_list args, void *data) {
    vprintf(format, args);
    printf("\n");
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    bool camInit = initCamera();

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
    connect(captureBtn, SIGNAL(clicked()), this, SLOT(takePicture()));
    captureBtn->setEnabled(camInit);
    layout->addWidget(captureBtn, 1, Qt::AlignBottom | Qt::AlignHCenter);

    connect(&previewTimer, SIGNAL(timeout()), this, SLOT(takePreview()));
}

MainWindow::~MainWindow()
{
    closeCamera();
}

void MainWindow::takePicture()
{
    stopPreview();

    int rc;
    gp_camera_capture(camera, GP_CAPTURE_IMAGE, &path, cameraContext);
    gp_file_new(&file);
    gp_camera_file_get(camera, path.folder, path.name, GP_FILE_TYPE_NORMAL, file, cameraContext);
    gp_camera_file_delete(camera, path.folder, path.name, cameraContext);

    rc = gp_file_get_data_and_size(file, &data, &size);
    if(rc != 0)
        printf("Error getting data; code %i: %s", rc, gp_result_as_string(rc));

    printf("Size of picture captured: %u\n", size);

    QImage image = QImage::fromData((uchar*) data, size);
    label.setPixmap(QPixmap::fromImage(image).scaled(label.width(), label.height(), Qt::KeepAspectRatio));

    gp_file_unref(file);
}

void MainWindow::takePreview()
{
    int rc;

    rc = gp_file_new(&file);
    if(rc != 0)
        printf("Error creating file; code %i: %s", rc, gp_result_as_string(rc));

    rc = gp_camera_capture_preview(camera, file, cameraContext);
    if(rc != 0)
        printf("Error capturing preview; code %i: %s", rc, gp_result_as_string(rc));

    rc = gp_file_get_data_and_size(file, &data, &size);
    if(rc != 0)
        printf("Error getting data; code %i: %s", rc, gp_result_as_string(rc));

    printf("Size of preview captured: %u\n", size);

    QImage image = QImage::fromData((uchar*) data, size).mirrored(true, false);
    label.setPixmap(QPixmap::fromImage(image).scaled(label.width(), label.height(), Qt::KeepAspectRatio));

    gp_file_unref(file);
}

void MainWindow::togglePreview()
{
    if(previewTimer.isActive())
        stopPreview();
    else
        startPreview();
}

bool MainWindow::initCamera()
{
    gp_camera_new(&camera);
    cameraContext = gp_context_new();

    //gp_context_set_error_func(gpc, error_func, NULL);
    //gp_context_set_message_func(gpc, message_func, NULL);

    printf("Camera init. Can take more than 10 seconds depending on the "
           "memory card's contents (remove card from camera to speed up).\n");

    int ret = gp_camera_init(camera, cameraContext);
    if (ret < GP_OK) {
        printf("... no camera auto detected.\n");
        gp_camera_free(camera);
        return false;
    }

    printf("... camera initialization finished successfully.\n");
    return true;
}

void MainWindow::closeCamera()
{
    printf("Closing camera ...");

    // close camera
    gp_camera_unref(camera);
    gp_context_unref(cameraContext);

    printf(" finished.\n");
}

void MainWindow::startPreview()
{
    previewTimer.start(100);
}

void MainWindow::stopPreview()
{
    previewTimer.stop();
}
