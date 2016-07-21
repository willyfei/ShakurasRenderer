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


template<class UL, class V, class VS>
class GeometryStage {
public:
	void initialize(float w, float h) {
		width_ = w;
		height_ = h;
		vertshader_ = std::make_shared<VS>();
	}

	void setShader(std::shared_ptr<VS> vs) {
		vertshader_ = vs;
	}

	void process(StageBuffer<UL, V>& buffer) {
		//vertex sharding
		for (auto i = buffer.vertlist.begin(); i != buffer.vertlist.end(); i++) {
			vertshader_->process(buffer.uniforms, *i);
		}

		//geometry sharding
		for (size_t i = 0; i < buffer.vertlist.size();) {
			if (buffer.vertlist[i].primf == kPFTriangle) {
				i += 3;
			}
		}


		//projection transform
		for (auto i = buffer.vertlist.begin(); i != buffer.vertlist.end(); i++) {
			i->pos = buffer.projtrsf.transform(i->pos);
		}

		//cliping, ºÚªØ µœ÷
		for (size_t i = 0; i < buffer.vertlist.size();) {
			if (buffer.vertlist[i].primf == kPFTriangle) {
				if (CheckCVV(buffer.vertlist[i].pos) != 0 ||
					CheckCVV(buffer.vertlist[i + 1].pos) != 0 ||
					CheckCVV(buffer.vertlist[i + 2].pos) != 0) {
					UnstableErase(buffer.vertlist, i, 3);
				}
				else {
					i += 3;
				}
			}
		}

		//screen mapping
		for (auto i = buffer.vertlist.begin(); i != buffer.vertlist.end(); i++) {
			ScreenMapping(i->pos, width_, height_);
		}
	}

private:
	float width_, height_;
	std::shared_ptr<VS> vertshader_;
};


SHAKURAS_END;