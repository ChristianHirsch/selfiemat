#ifndef __FOCUS_H
#define __FOCUS_H
#include <gphoto2/gphoto2-camera.h>

extern int camera_auto_focus (Camera *list, GPContext *context, int onoff);
extern int camera_eosviewfinder (Camera *list, GPContext *context, int onoff);
extern int camera_manual_focus (Camera *list, int tgt, GPContext *context);
#endif
