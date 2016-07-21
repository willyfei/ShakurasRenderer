#pragma once
#include "MathAndGeometry.h"
#include "Mipmap.h"
#include "Surface.h"
#include "Resource.h"
#include <vector>
#include <string>


SHAKURAS_BEGIN;


class ObjMtl {
public:
	ObjMtl() : name("default")
		, ambient(0.2f, 0.2f, 0.2f)
		, diffuse(0.5f, 0.5f, 0.5f)
		, specular(0.7f, 0.7f, 0.7f)
		, shininess(2), alpha(1.0f)
		, is_specular(true) {}

	std::string name;

	MipmapPtr tex;
	Vector3f ambient;
	Vector3f diffuse;
	Vector3f specular;

	int shininess;
	float alpha;
	bool is_specular;

	std::string tex_name;
};


class ObjVert {
public:
	Vector3f pos;
	Vector2f uv;
	Vector3f normal;
};


class ObjMesh {
public:
	std::vector<ObjVert> verts;//[0,1,2][3,4,5]...
	ObjMtl mtl;
};


bool LoadObjMesh(const std::string& fname, std::vector<ObjMesh>& meshs, bool flip_tex_v);


SHAKURAS_END;