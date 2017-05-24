#include "GlContextBinding.h"

#if defined(__ANDROID__)
/**
* 我们的保存状态数据。
*/
struct saved_state {
	float angle;
	int32_t x;
	int32_t y;
};

/**
* 我们应用程序的共享状态。
*/
struct engine {
	struct android_app* app;

	ASensorManager* sensorManager;
	const ASensor* accelerometerSensor;
	ASensorEventQueue* sensorEventQueue;

	int animating;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	struct saved_state state;
};

/**
* 初始化当前显示的 EGL 上下文。
*/
static int engine_init_display(struct engine* engine) {
	//初始化 OpenGL ES 和 EGL

	/*
	* 此处指定了所需配置的属性。
	*下面，我们选择与屏上窗口
	* 兼容的至少每个颜色有 8 个位的 EGLConfig
	*/
	const EGLint attribs[] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_NONE
	};
	EGLint w, h, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/*此处，应用程序选择了所需的配置。 在本
	*示例中，我们有非常简化的选择流程，
	*其中我们选取了与我们的标准匹配的第一个 EGLConfig */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID 是
	*保证会被 ANativeWindow_setBuffersGeometry() 接受的 EGLConfig 的属性。
	* 只要我们选取 EGLConfig，就可使用 EGL_NATIVE_VISUAL_ID 安全地重新配置
	* ANativeWindow 缓冲区以进行匹配。*/
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	context = eglCreateContext(display, config, NULL, NULL);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGW("Unable to eglMakeCurrent");
		return -1;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;
	engine->state.angle = 0;

	//初始化 GL 状态。
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);

	return 0;
}

/**
* 仅显示中的当前帧。
*/
static void engine_draw_frame(struct engine* engine) {
	if (engine->display == NULL) {
		//无显示。
		return;
	}

	//只使用一种颜色填充屏幕。
	glClearColor(((float)engine->state.x) / engine->width, engine->state.angle,
		((float)engine->state.y) / engine->height, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	eglSwapBuffers(engine->display, engine->surface);
}

/**
* 关闭当前与显示关联的 EGL 上下文。
*/
static void engine_term_display(struct engine* engine) {
	if (engine->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE) {
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->animating = 0;
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}
#endif

SHAKURAS_BEGIN;

#if !defined(__ANDROID__)
GlContextBinding::GlContextBinding(HDC hDC, HGLRC hRC)
{
	BOOL res = wglMakeCurrent(hDC, hRC);
}


GlContextBinding::~GlContextBinding()
{
	wglMakeCurrent(NULL, NULL);
}
#else
GlContextBinding::GlContextBinding(struct android_app* app)
{
	struct engine* engine = (struct engine*)app->userData;
	app_ = app;
	engine_init_display(engine);
}

GlContextBinding::~GlContextBinding()
{
	struct engine* engine = (struct engine*)app_->userData;
	engine_term_display(engine);
}
#endif
SHAKURAS_END;