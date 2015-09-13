#include "eye.h"

void error_func (GPContext *context, const char *format, va_list args, void *data) {
    fprintf(stderr, "*** Contexterror ***\n");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
}

void message_func (GPContext *context, const char *format, va_list args, void *data) {
    vprintf(format, args);
    printf("\n");
}

Eye::Eye()
{
    cameraInitialized = false;
}

Eye::~Eye()
{
    closeCamera();
}

QImage Eye::takePicture()
{
    int rc;
    gp_camera_capture(camera, GP_CAPTURE_IMAGE, &path, cameraContext);
    gp_file_new(&file);
    gp_camera_file_get(camera, path.folder, path.name, GP_FILE_TYPE_NORMAL, file, cameraContext);
    gp_camera_file_delete(camera, path.folder, path.name, cameraContext);

    rc = gp_file_get_data_and_size(file, &data, &size);
    if(rc != 0)
        printf("Error getting data; code %i: %s", rc, gp_result_as_string(rc));

    QImage image = QImage::fromData((uchar*) data, size);

    gp_file_unref(file);

    return image;
}

QImage Eye::takePreview()
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

    QImage image = QImage::fromData((uchar*) data, size).mirrored(true, false);

    gp_file_unref(file);

    return image;
}

bool Eye::initCamera()
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

    cameraInitialized = true;
    return true;
}

void Eye::closeCamera()
{
    printf("Closing camera ...");

    // close camera
    gp_camera_unref(camera);
    gp_context_unref(cameraContext);

    printf(" finished.\n");
}

