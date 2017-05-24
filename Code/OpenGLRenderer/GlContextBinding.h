#pragma once
#include "GlRendererDll.h"
#include "Core\Utility.h"

SHAKURAS_BEGIN;

#if !defined(__ANDROID__)
class GLRENDERER_DLL GlContextBinding {
public:
	GlContextBinding(HDC hDC, HGLRC hRC);
	~GlContextBinding();
};
#else
class GLRENDERER_DLL GlContextBinding
{
public:
	GlContextBinding(struct android_app* app);
	~GlContextBinding();
	struct android_app* app_;
};
#endif

SHAKURAS_END;