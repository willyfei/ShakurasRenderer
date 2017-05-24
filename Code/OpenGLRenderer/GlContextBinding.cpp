#include "GlContextBinding.h"

#if defined(__ANDROID__)
/**
* ���ǵı���״̬���ݡ�
*/
struct saved_state {
	float angle;
	int32_t x;
	int32_t y;
};

/**
* ����Ӧ�ó���Ĺ���״̬��
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
* ��ʼ����ǰ��ʾ�� EGL �����ġ�
*/
static int engine_init_display(struct engine* engine) {
	//��ʼ�� OpenGL ES �� EGL

	/*
	* �˴�ָ�����������õ����ԡ�
	*���棬����ѡ�������ϴ���
	* ���ݵ�����ÿ����ɫ�� 8 ��λ�� EGLConfig
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

	/*�˴���Ӧ�ó���ѡ������������á� �ڱ�
	*ʾ���У������зǳ��򻯵�ѡ�����̣�
	*��������ѡȡ�������ǵı�׼ƥ��ĵ�һ�� EGLConfig */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID ��
	*��֤�ᱻ ANativeWindow_setBuffersGeometry() ���ܵ� EGLConfig �����ԡ�
	* ֻҪ����ѡȡ EGLConfig���Ϳ�ʹ�� EGL_NATIVE_VISUAL_ID ��ȫ����������
	* ANativeWindow �������Խ���ƥ�䡣*/
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

	//��ʼ�� GL ״̬��
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_DEPTH_TEST);

	return 0;
}

/**
* ����ʾ�еĵ�ǰ֡��
*/
static void engine_draw_frame(struct engine* engine) {
	if (engine->display == NULL) {
		//����ʾ��
		return;
	}

	//ֻʹ��һ����ɫ�����Ļ��
	glClearColor(((float)engine->state.x) / engine->width, engine->state.angle,
		((float)engine->state.y) / engine->height, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	eglSwapBuffers(engine->display, engine->surface);
}

/**
* �رյ�ǰ����ʾ������ EGL �����ġ�
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