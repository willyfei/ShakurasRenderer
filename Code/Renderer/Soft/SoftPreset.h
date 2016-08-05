#pragma once
#include "SoftMipmap.h"
#include "SoftVertex.h"
#include "SoftFragment.h"
#include "SoftDrawCall.h"
#include "SoftGeometryStage.h"
#include "SoftRasterizerStage.h"
#include "SoftRenderStage.h"
#include "../MathAndGeometry.h"


SHAKURAS_BEGIN;


//一层mipmap纹理
//Phong着色
namespace phong {

	struct UniformList {
		SoftMipmapPtr texture;
		Vector3f ambient;
		Vector3f diffuse;
		Vector3f specular;
		Matrix44f model_trsf;
		Matrix44f mv_trsf;
		Vector3f light_pos;
		Vector3f eye_pos;
	};

	struct AttribList {
		Vector2f uv;
		Vector3f normal;
	};

	struct VaryingList {
		Vector2f uv;
		Vector3f normal;
		Vector3f light_dir;
		Vector3f eye_dir;
	};

	inline VaryingList operator+(const VaryingList& v1, const VaryingList& v2) {
		VaryingList v3;
		v3.uv = v1.uv + v2.uv;
		v3.normal = v1.normal + v2.normal;
		v3.light_dir = v1.light_dir + v2.light_dir;
		v3.eye_dir = v1.eye_dir + v2.eye_dir;
		return v3;
	}

	inline VaryingList operator-(const VaryingList& v1, const VaryingList& v2) {
		VaryingList v3;
		v3.uv = v1.uv - v2.uv;
		v3.normal = v1.normal - v2.normal;
		v3.light_dir = v1.light_dir - v2.light_dir;
		v3.eye_dir = v1.eye_dir - v2.eye_dir;
		return v3;
	}

	inline VaryingList operator*(const VaryingList& v1, float t) {
		VaryingList v3;
		v3.uv = v1.uv * t;
		v3.normal = v1.normal * t;
		v3.light_dir = v1.light_dir * t;
		v3.eye_dir = v1.eye_dir * t;
		return v3;
	}

	inline VaryingList operator/(const VaryingList& v1, float d) {
		VaryingList v3;
		float t = 1.0f / d;
		v3.uv = v1.uv * t;
		v3.normal = v1.normal * t;
		v3.light_dir = v1.light_dir * t;
		v3.eye_dir = v1.eye_dir * t;
		return v3;
	}

	typedef shakuras::SoftVertex<AttribList, VaryingList> Vertex;

	typedef shakuras::SoftFragment<VaryingList> Fragment;

	typedef shakuras::SoftDrawCall<UniformList, Vertex> DrawCall;

	typedef shakuras::SoftPrimitiveList<Vertex> PrimitiveList;

	class VertexShader {
	public:
		void process(const UniformList& u, Vertex& v) {
			v.varyings.uv = v.attribs.uv;

			v.varyings.normal = u.model_trsf.transform(v.attribs.normal.xyzw(0.0f)).xyz();

			v.varyings.light_dir = u.light_pos - v.pos.xyz();

			v.varyings.eye_dir = u.eye_pos - v.pos.xyz();

			v.pos = u.mv_trsf.transform(v.pos);
		}
	};

	class FragmentShader {
	public:
		void process(const UniformList& u, SoftSampler& sampler, Fragment& f) {
			Vector3f norm = f.varyings.normal;
			Vector3f light_dir = f.varyings.light_dir;
			Vector3f eye_dir = f.varyings.eye_dir;
			Normalize3(norm);
			Normalize3(light_dir);
			Normalize3(eye_dir);

			float illum_diffuse = Clamp(DotProduct3(light_dir, norm), 0.0f, 1.0f);
			float illum_specular = Clamp(DotProduct3(Reflect(light_dir, norm), eye_dir), 0.0f, 1.0f);
			Vector3f illum = u.ambient + u.diffuse * illum_diffuse + u.specular * illum_specular;

			Vector2f uv = f.varyings.uv;
			Vector3f tc(1.0f, 1.0f, 1.0f);//默认白色
			if (u.texture) {
				tc = sampler.mipmapTrilinear(uv.x, uv.y, *u.texture, RepeatAddr);
			}
			Vector3f c(tc.x * illum.x, tc.y * illum.y, tc.z * illum.z);

			Clamp(c.x, 0.0f, 1.0f);
			Clamp(c.y, 0.0f, 1.0f);
			Clamp(c.z, 0.0f, 1.0f);

			f.c.set(c.x, c.y, c.z, 1.0f);
		}
	};

	typedef SoftGeometryStage<UniformList, Vertex, VertexShader> GeomStage;

	typedef SoftRasterizerStage<UniformList, Vertex, Fragment, FragmentShader> RasStage;

	typedef SoftRenderStage<DrawCall, GeomStage, RasStage> RenderStage;
}


template<>
inline Vector2f TexCoord<phong::VaryingList>(const phong::VaryingList& v) {
	return v.uv;
}


SHAKURAS_END;