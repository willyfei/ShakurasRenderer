#pragma once
#include "MathAndGeometry.h"


SHAKURAS_BEGIN;


inline int CheckCVV(const Vector4f& v) {
	int check = 0;
	if (v.z < 0.0f) check |= 1;
	if (v.z > v.w) check |= 2;
	if (v.x < -v.w) check |= 4;
	if (v.x > v.w) check |= 8;
	if (v.y < -v.w) check |= 16;
	if (v.y > v.w) check |= 32;
	return check;
}


inline void ScreenMapping(Vector4f& v, float width, float height) {
	float w = v.w;
	Vector4f r;
	float rhw = 1.0f / v.w;
	r.x = (v.x * rhw + 1.0f) * width * 0.5f;
	r.y = (1.0f - v.y * rhw) * height * 0.5f;
	r.z = v.z * rhw;
	r.w = w;
	v = r;
}


template<class UniformList, class Vert, class VertShader>
class GsGeometryStage {
public:
	void initialize(float w, float h) {
		width_ = w;
		height_ = h;
		vertshader_ = std::make_shared<VertShader>();
	}

	void setShader(std::shared_ptr<VertShader> vs) {
		vertshader_ = vs;
	}

	void process(GsStageBuffer<UniformList, Vert>& buffer) {
		//vertex sharding, 内部含有model view transform
		for (auto i = buffer.vertlist.begin(); i != buffer.vertlist.end(); i++) {
			vertshader_->process(buffer.uniforms, *i);
		}

		//projection transform
		for (auto i = buffer.vertlist.begin(); i != buffer.vertlist.end(); i++) {
			i->pos = buffer.projtrsf.transform(i->pos);
		}

		//cliping, 简化实现
		size_t i = 0;
		while (i + 2 < buffer.itris.size()) {
			if (CheckCVV(buffer.vertlist[buffer.itris[i]].pos) != 0 ||
				CheckCVV(buffer.vertlist[buffer.itris[i + 1]].pos) != 0 ||
				CheckCVV(buffer.vertlist[buffer.itris[i + 2]].pos) != 0) {
				int len = (int)buffer.itris.size();
				std::swap(buffer.itris[i], buffer.itris[len - 3]);
				std::swap(buffer.itris[i + 1], buffer.itris[len - 2]);
				std::swap(buffer.itris[i + 2], buffer.itris[len - 1]);
				buffer.itris.pop_back();
				buffer.itris.pop_back();
				buffer.itris.pop_back();
			}
			else {
				i += 3;
			}
		}

		//screen mapping
		for (auto i = buffer.vertlist.begin(); i != buffer.vertlist.end(); i++) {
			ScreenMapping(i->pos, width_, height_);
		}
	}

private:
	float width_, height_;
	std::shared_ptr<VertShader> vertshader_;
};


SHAKURAS_END;