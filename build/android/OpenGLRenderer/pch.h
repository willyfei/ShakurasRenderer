#if defined(__ANDROID__)
#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <math.h>
#include <android/log.h>
#include <android/sensor.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include "android_native_app_glue.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "OpenGLRenderer", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "OpenGLRenderer", __VA_ARGS__))

#else
#include <windows.h>
#include "gl\glew.h"
#include "gl\wglew.h"
#endif
