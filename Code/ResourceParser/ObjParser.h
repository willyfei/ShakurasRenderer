#pragma once
#include "ResUtility.h"
#include "Renderer/MathAndGeometry.h"
#include "Renderer/Soft/Mipmap.h"
#include "Renderer/Soft/Surface.h"
#include <vector>
#include <string>


#pragma warning(push) 
#pragma warning(disable:4251)


class RESPARSER_DLL ObjMtl {
public:
	ObjMtl() : name("default")
		, ambient(0.2f, 0.2f, 0.2f)
		, diffuse(0.5f, 0.5f, 0.5f)
		, specular(0.7f, 0.7f, 0.7f)
		, shininess(2), alpha(1.0f)
		, is_specular(true) {}

	std::string name;

	shakuras::MipmapPtr tex;
	shakuras::Vector3f ambient;
	shakuras::Vector3f diffuse;
	shakuras::Vector3f specular;

	int shininess;
	float alpha;
	bool is_specular;

	std::string tex_name;
};


class RESPARSER_DLL ObjVert {
public:
	shakuras::Vector3f pos;
	shakuras::Vector2f uv;
	shakuras::Vector3f normal;
};


class RESPARSER_DLL ObjMesh {
public:
	std::vector<ObjVert> verts;
	std::vector<size_t> tris;//[0,1,2][3,4,5]...
	ObjMtl mtl;
};


RESPARSER_DLL bool LoadObjMesh(const std::string& fname, std::vector<ObjMesh>& meshs, bool flip_tex_v);


#pragma warning(pop)