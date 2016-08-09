#pragma once
#include "GlRendererDll.h"
#include "Core\Utility.h"
#include <Windows.h>


SHAKURAS_BEGIN;


class GLRENDERER_DLL GlContextBinding {
public:
	GlContextBinding(HDC hDC, HGLRC hRC);
	~GlContextBinding();
};


SHAKURAS_END;