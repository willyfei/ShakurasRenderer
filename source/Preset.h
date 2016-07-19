#pragma once
#include "MathAndGeometry.h"
#include "Texture.h"
#include "VaryingList.h"
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
		Vector2f texcoord;
		Vector4f normal;
	};

	typedef std::tuple<
		Varying<Vector2f, kTBAll>,//纹理坐标
		Varying<Vector3f, kTBAll>,//法向
		Varying<Vector3f, kTBAll>,//光源方向(light_pos - pos)
		Varying<Vector3f, kTBAll>//相机方向(eye_pos - pos)
	> VaryingList;

	typedef Vertex<AttribList, VaryingList> Vertex;

	typedef GsFragment<VaryingList> Fragment;

	typedef StageBuffer<UniformList, Vertex> StageBuffer;

	class VertexShader {
	public:
		void process(const UniformList& u, Vertex& v) {
			std::get<0>(v.varyings).value = v.attribs.texcoord;

			Vector4f norm = u.mvtrsf.transform(v.attribs.normal);
			std::get<1>(v.varyings).value.set(norm.x, norm.y, norm.z);

			Vector3f light_pos = u.light_pos;
			std::get<2>(v.varyings).value.set(light_pos.x - v.pos.x, light_pos.y - v.pos.y, light_pos.z - v.pos.z);

			Vector3f eye_pos = u.eye_pos;
			std::get<3>(v.varyings).value.set(eye_pos.x - v.pos.x, eye_pos.y - v.pos.y, eye_pos.z - v.pos.z);

			v.pos = u.mvtrsf.transform(v.pos);
		}
	};

	class FragmentShader {
	public:
		void process(const UniformList& u, Fragment& f) {
			Vector3f norm = std::get<1>(f.varyings).value;
			Vector3f light_dir = std::get<2>(f.varyings).value;
			Vector3f eye_dir = std::get<3>(f.varyings).value;
			Normalize(norm);
			Normalize(light_dir);
			Normalize(eye_dir);

			float illum_diffuse = Clamp(DotProduct(light_dir, norm), 0.0f, 1.0f);
			float illum_specular = Clamp(DotProduct(Reflect(light_dir, norm), eye_dir), 0.0f, 1.0f);
			Vector3f illum = u.ambient + u.diffuse * illum_diffuse + u.specular * illum_specular;

			Vector2f uv = std::get<0>(f.varyings).value;
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


SHAKURAS_END;