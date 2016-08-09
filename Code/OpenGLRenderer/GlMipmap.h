#pragma once
#include "GlRendererDll.h"
#include "Core\Utility.h"
#include <string>


SHAKURAS_BEGIN;


class GLRENDERER_DLL GlMipmap {
public:
	GlMipmap();
	~GlMipmap();

public:
	void load(void* bits, int width, int height);

public:
	unsigned int tex_id_;
};


SHAKURAS_SHARED_PTR(GlMipmap);


GLRENDERER_DLL GlMipmapPtr CreateGlMipmap(void* bits, int width, int height);


SHAKURAS_END;