#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <android/log.h>

#include <math.h>
#include "android_native_app_glue.h"

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES3/gl3.h>