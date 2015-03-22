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

    QPushButton *btn = new QPushButton;
    btn->setText("Cheese!");
    connect(btn, SIGNAL(clicked()), this, SLOT(takePicture()));
    btn->setEnabled(camInit);
    layout->addWidget(btn, 1, Qt::AlignBottom | Qt::AlignHCenter);
}

MainWindow::~MainWindow()
{
    closeCamera();
}

void MainWindow::takePicture()
{
    gp_camera_capture(camera, GP_CAPTURE_IMAGE, &path, cameraContext);
    gp_file_new(&file);
    gp_camera_file_get(camera, path.folder, path.name, GP_FILE_TYPE_NORMAL, file, cameraContext);
    gp_camera_file_delete(camera, path.folder, path.name, cameraContext);
    gp_file_get_data_and_size(file, &data, &size);

    printf("Size of picture captured: %u\n", size);

    QImage image = QImage::fromData((uchar*) data, size);
    label.setPixmap(QPixmap::fromImage(image).scaled(label.width(), label.height(), Qt::KeepAspectRatio));
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

    printf(" finished.");
}
