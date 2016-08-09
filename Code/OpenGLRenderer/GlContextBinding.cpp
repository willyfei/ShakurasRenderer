#include "GlContextBinding.h"


SHAKURAS_BEGIN;


GlContextBinding::GlContextBinding(HDC hDC, HGLRC hRC)
{
	BOOL res = wglMakeCurrent(hDC, hRC);
}


GlContextBinding::~GlContextBinding()
{
	wglMakeCurrent(NULL, NULL);
}


SHAKURAS_END;