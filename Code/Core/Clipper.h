#pragma once
#include "MathAndGeometry.h"
#include "Primitive.h"
#include <algorithm>
#include <array>


SHAKURAS_BEGIN;


inline int CheckCVV(const Vector4f& v) {
	int check = 0;
	if (v.z > v.w) check |= 1;
	if (v.z < 0.0f) check |= 2;
	if (v.y > v.w) check |= 4;
	if (v.y < -v.w) check |= 8;
	if (v.x > v.w) check |= 16;
	if (v.x < -v.w) check |= 32;
	return check;
}


inline bool IsFront(const Vector4f& pos0, const Vector4f& pos1, const Vector4f& pos2) {
	Vector2f pv_2d[3] =
	{
		Vector2f(pos0.x, pos0.y) / pos0.w,
		Vector2f(pos1.x, pos1.y) / pos1.w,
		Vector2f(pos2.x, pos2.y) / pos2.w,
	};

	float const area = CrossProduct2(pv_2d[2] - pv_2d[0], pv_2d[1] - pv_2d[0]);
	return area > 0.0f;
}


template<class V>
inline V SignedDistanceLerp(const V& v1, const V& v2, float d1, float d2) {
	if (d1 < d2) {
		return v2 + (v1 - v2) * d2 / (d2 - d1);
	}
	else {
		return v1 + (v2 - v1) * d1 / (d1 - d2);
	}
}


template<class V>
class Clipper {
public:
	Clipper(PrimitiveList<V>& prims) {
		iprims_ = &prims;
	}

public:
	void process() {
		oprims_.clear();

		computeOrientate();
		
		for (size_t i = 0; i != iprims_->tris_.size(); i++) {
			clipTriangle(i);
		}

		iprims_->swap(oprims_);
	}

private:
	// 0 : (, near)
	// 1 : [near, far]
	// 2 : (far, )
	inline short orientate(const Vector4f& pos, float& neard, float& fard) {
		static const Vector4f near_plane = { 0.0f, 0.0f, 1.0f, 0.0f };
		static const Vector4f far_plane = { 0.0f, 0.0f, -1.0f, 1.0f };

		neard = DotProduct4(near_plane, pos);
		fard = DotProduct4(far_plane, pos);
		if (neard < 0) {
			return 0;
		}
		else if (fard < 0) {
			return 2;
		}
		return 1;
	}

	void computeOrientate() {
		oris_.clear();
		neards_.clear();
		fards_.clear();
		oris_.resize(iprims_->verts_.size(), -1);
		neards_.resize(iprims_->verts_.size());
		fards_.resize(iprims_->verts_.size());

		for (size_t i = 0; i != iprims_->verts_.size(); i++) {
			float neard = 0.0f, fard = 0.0f;
			oris_[i] = orientate(iprims_->verts_[i].pos, neard, fard);
			neards_[i] = neard;
			fards_[i] = fard;
		}
	}

	void clipTriangle(size_t i) {
		const size_t i1 = iprims_->tris_[i][0];
		const size_t i2 = iprims_->tris_[i][1];
		const size_t i3 = iprims_->tris_[i][2];

		const V& v1 = iprims_->verts_[i1];
		const V& v2 = iprims_->verts_[i2];
		const V& v3 = iprims_->verts_[i3];

		const short o1 = oris_[i1];
		const short o2 = oris_[i2];
		const short o3xyhX = oris_[i3];

		std::array<size_t, 3> tri_index = { i1, i2, i3 };
		std::array<const V*, 3> tri_pvert = { &v1, &v2, &v3 };
		
		//先剔除背面
		if (!IsFront(v1.pos, v2.pos, v3.pos)) {
			return;
		}

		//所有顶点可能分布在三个区域：
		// 0 : (, near)
		// 1 : [near, far]
		// 2 : (far, )
		//oris_记录了这个标记

		std::array<int, 3> counter = { 0, 0, 0 };
		counter[oris_[i1]]++;
		counter[oris_[i2]]++;
		counter[oris_[i3]]++;

		int max_count = *std::max_element(counter.begin(), counter.end());

		//S-1 三个点在同一个区域
		//	S-1.1 三个点属于(, near)，剔除
		//	S-1.2 三个点属于[near, far]，输出
		//	S-1.3 三个点属于(far, )，剔除
		//S-2 两个点在同一个区域
		//	S-2.1 一个点属于[near, far]，对另两个点插值，生成三角形，输出
		//	S-2.2 两个点属于[near, far]，对另一个点插值成两个点，生成梯形，拆分成两个三角形，输出
		//	S-2.3 没有点属于[near, far]，插值生成四个点，生成梯形，拆分成两个三角形，输出
		//S-3 每个点都属于不同的区域
		//	S-3.1 插值生成四个点，生成五边形，拆分成三个三角形，输出

		if (max_count == 3) {
			if (counter[1] == 3) {
				oprims_.addTriangle(v1, v2, v3);
			}
		}
		else if (max_count == 2) {
			if (counter[1] == 1) {

			}
		}
	}

private:
	PrimitiveList<V>* iprims_;
	std::vector<short> oris_;
	std::vector<float> neards_;
	std::vector<float> fards_;
	PrimitiveList<V> oprims_;
};


SHAKURAS_END