#pragma once
#include "MathAndGeometry.h"
#include "Texture.h"
#include "Vertex.h"
#include "Fragment.h"
#include "StageBuffer.h"
#include "GeometryStage.h"
#include "RasterizerStage.h"
#include "GraphicPipeline.h"


SHAKURAS_BEGIN;


namespace preset_std {

	struct UniformList {
		SurfacePtr texture;
		Matrix44f mvtrsf;
		Vector3f ambient;
		Vector3f diffuse;
		Vector3f specular;
		Vector3f light_pos;
		Vector3f eye_pos;
	};

	struct AttribList {
		Vector2f uv;
		Vector4f normal;
	};

	struct VaryingList {
		Vector2f uv;
		Vector3f normal;
		Vector3f light_dir;
		Vector3f eye_dir;
	};

	typedef shakuras::Vertex<AttribList, VaryingList> Vertex;

	typedef shakuras::Fragment<VaryingList> Fragment;

	typedef shakuras::StageBuffer<UniformList, Vertex> StageBuffer;

	class VertexShader {
	public:
		void process(const UniformList& u, Vertex& v) {
			v.varyings.uv = v.attribs.uv;

			Vector4f norm = u.mvtrsf.transform(v.attribs.normal);
			v.varyings.normal.set(norm.x, norm.y, norm.z);

			Vector3f light_pos = u.light_pos;
			v.varyings.light_dir.set(light_pos.x - v.pos.x, light_pos.y - v.pos.y, light_pos.z - v.pos.z);

			Vector3f eye_pos = u.eye_pos;
			v.varyings.eye_dir.set(eye_pos.x - v.pos.x, eye_pos.y - v.pos.y, eye_pos.z - v.pos.z);

			v.pos = u.mvtrsf.transform(v.pos);
		}
	};

	class FragmentShader {
	public:
		void process(const UniformList& u, Fragment& f) {
			Vector3f norm = f.varyings.normal;
			Vector3f light_dir = f.varyings.light_dir;
			Vector3f eye_dir = f.varyings.eye_dir;
			Normalize(norm);
			Normalize(light_dir);
			Normalize(eye_dir);

			float illum_diffuse = Clamp(DotProduct(light_dir, norm), 0.0f, 1.0f);
			float illum_specular = Clamp(DotProduct(Reflect(light_dir, norm), eye_dir), 0.0f, 1.0f);
			Vector3f illum = u.ambient + u.diffuse * illum_diffuse + u.specular * illum_specular;

			Vector2f uv = f.varyings.uv;
			Vector3f tc = BilinearSample(uv.x, uv.y, *u.texture);
			Vector3f c(tc.x * illum.x, tc.y * illum.y, tc.z * illum.z);

			Clamp(c.x, 0.0f, 1.0f);
			Clamp(c.y, 0.0f, 1.0f);
			Clamp(c.z, 0.0f, 1.0f);

			f.c.set(c.x, c.y, c.z, 1.0f);
		}
	};

	typedef GeometryStage<UniformList, Vertex, VertexShader> GeomStage;

	typedef RasterizerStage<UniformList, Vertex, Fragment, FragmentShader> RasStage;
}


template<>
void TravPlus<preset_std::VaryingList>(preset_std::VaryingList& v1, const preset_std::VaryingList& v2, int oper) {
	v1.uv = v1.uv + v2.uv;
	v1.normal = v1.normal + v2.normal;
	v1.light_dir = v1.light_dir + v2.light_dir;
	v1.eye_dir = v1.eye_dir + v2.eye_dir;
}

template<>
void TravSub<preset_std::VaryingList>(preset_std::VaryingList& v1, const preset_std::VaryingList& v2, int oper) {
	v1.uv = v1.uv - v2.uv;
	v1.normal = v1.normal - v2.normal;
	v1.light_dir = v1.light_dir - v2.light_dir;
	v1.eye_dir = v1.eye_dir - v2.eye_dir;
}

template<>
void TravMul<preset_std::VaryingList>(preset_std::VaryingList& v1, float t, int oper) {
	v1.uv = v1.uv * t;
	v1.normal = v1.normal * t;
	v1.light_dir = v1.light_dir * t;
	v1.eye_dir = v1.eye_dir * t;
}

template<>
void TravDiv<preset_std::VaryingList>(preset_std::VaryingList& v1, float d, int oper) {
	float t = 1.0f / d;
	v1.uv = v1.uv * t;
	v1.normal = v1.normal * t;
	v1.light_dir = v1.light_dir * t;
	v1.eye_dir = v1.eye_dir * t;
}


SHAKURAS_END;