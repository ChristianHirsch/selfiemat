#ifndef EYE_H
#define EYE_H

#include <QImage>

#include <gphoto2/gphoto2-abilities-list.h>
#include <gphoto2/gphoto2-camera.h>

class Eye
{
public:
    Eye();
    ~Eye();

    bool initCamera();

    QImage takePicture();
    QImage takePreview();

protected:

    bool cameraInitialized;

    CameraAbilitiesList *al;
    CameraAbilities a;
    GPPortInfoList *il;
    GPPortInfo info;
    GPContext *cameraContext;

    CameraFilePath path;
    CameraFile *file;
    Camera *camera;

    const char *data;
    unsigned long size;

private:

    void closeCamera();
};

#endif // EYE_H
