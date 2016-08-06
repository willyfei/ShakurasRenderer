#pragma once
#include "SoftPrimitiveList.h"
#include "Core/MathAndGeometry.h"
#include "Core/Profiler.h"
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


inline bool IsCounterClockwise(const Vector4f& pos0, const Vector4f& pos1, const Vector4f& pos2) {
	Vector2f pv_2d[3] =
	{
		{ pos0.x / pos0.w, pos0.y / pos0.w },
		{ pos1.x / pos1.w, pos1.y / pos1.w },
		{ pos2.x / pos2.w, pos2.y / pos2.w }
	};

	float area = CrossProduct2(pv_2d[2] - pv_2d[0], pv_2d[1] - pv_2d[0]);
	return area > 0.0f;
}


template<class V>
inline V SignedDistanceLerp(const V& v1, const V& v2, float d1, float d2) {
	V r;

	if (d1 < d2) {
		r = v2 + (v1 - v2) * (d2 / (d2 - d1));
	}
	else {
		r = v1 + (v2 - v1) * (d1 / (d1 - d2));
	}

	return r;
}


template<class V>
class SoftClipper {
public:
	SoftClipper(SoftPrimitiveList<V>& prims, Profiler& profiler, bool refuse_back) {
		iprims_ = &prims;
		profiler_ = &profiler;
		refuse_back_ = refuse_back;
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
	static const short kTooNear = 0;
	static const short kOK = 1;
	static const short kTooFar = 2;

	inline short orientate(const Vector4f& pos, float& neard, float& fard) {
		const Vector4f near_plane = { 0.0f, 0.0f, 1.0f, 0.0f };
		const Vector4f far_plane = { 0.0f, 0.0f, -1.0f, 1.0f };

		neard = DotProduct4(near_plane, pos);
		fard = DotProduct4(far_plane, pos);
		if (neard < 0) {
			return kTooNear;
		}
		else if (fard < 0) {
			return kTooFar;
		}
		return kOK;
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
		const short o3 = oris_[i3];
		
		//剔除背面
		if (refuse_back_ && !IsCounterClockwise(v1.pos, v2.pos, v3.pos)) {
			return;
		}

		//所有顶点可能分布在三个区域：
		// 0 : (, near)
		// 1 : [near, far]
		// 2 : (far, )
		//oris_记录了这个标记

		std::array<char, 3> counter = { 0, 0, 0 };
		counter[o1]++;
		counter[o2]++;
		counter[o3]++;

		int max_count = *std::max_element(counter.begin(), counter.end());

		//S-1 三个点在同一个区域
		//	S-1.1 三个点属于(, near)，剔除
		//	S-1.2 三个点属于[near, far]，输出
		//	S-1.3 三个点属于(far, )，剔除
		//S-2 两个点在同一个区域
		//	S-2.1 一个点属于[near, far]，对另两个点插值，生成三角形，输出
		//	S-2.2 两个点属于[near, far]，对另一个点插值得到两个点，生成梯形，拆分成两个三角形，输出
		//	S-2.3 没有点属于[near, far]，插值得到四个点，生成梯形，拆分成两个三角形，输出
		//S-3 每个点都属于不同的区域
		//	S-3.1 插值得到四个点，生成五边形，拆分成三个三角形，输出

		std::array<size_t, 3> tri_index = { i1, i2, i3 };
		std::array<const V*, 3> tri_pvert = { &v1, &v2, &v3 };

		if (max_count == 3) {
			//S-1.1
			if (counter[kOK] == 3) {
				outputTriangle(v1, v2, v3);
			}

			//S-1.2
			//S-1.3
		}
		else if (max_count == 2) {
			//S-2.1
			if (counter[kOK] == 1) {
				//将kOK的顶点旋转到首部
				int rot = (o1 == kOK ? 0 : (o2 == kOK ? 1 : 2));
				std::rotate(tri_index.begin(), tri_index.begin() + rot, tri_index.end());
				std::rotate(tri_pvert.begin(), tri_pvert.begin() + rot, tri_pvert.end());

				//取合适的平面距离映射
				short oo = (o1 != kOK ? o1 : (o2 != kOK ? o2 : o3));
				const std::vector<float>* pds = (oo == kTooNear ? &neards_ : &fards_);

				//插值
				V lerp_v2 = SignedDistanceLerp(*tri_pvert[0], *tri_pvert[1], (*pds)[tri_index[0]], (*pds)[tri_index[1]]);
				V lerp_v3 = SignedDistanceLerp(*tri_pvert[0], *tri_pvert[2], (*pds)[tri_index[0]], (*pds)[tri_index[2]]);

				//输出
				outputTriangle(*tri_pvert[0], lerp_v2, lerp_v3);
			}
			//S-2.2
			else if (counter[kOK] == 2) {
				//将非kOK的顶点旋转到首部
				int rot = (o1 != kOK ? 0 : (o2 != kOK ? 1 : 2));
				std::rotate(tri_index.begin(), tri_index.begin() + rot, tri_index.end());
				std::rotate(tri_pvert.begin(), tri_pvert.begin() + rot, tri_pvert.end());

				//取合适的平面距离映射
				short oo = (o1 != kOK ? o1 : (o2 != kOK ? o2 : o3));
				const std::vector<float>* pds = (oo == kTooNear ? &neards_ : &fards_);

				//插值
				V lerp_v2 = SignedDistanceLerp(*tri_pvert[0], *tri_pvert[1], (*pds)[tri_index[0]], (*pds)[tri_index[1]]);
				V lerp_v3 = SignedDistanceLerp(*tri_pvert[0], *tri_pvert[2], (*pds)[tri_index[0]], (*pds)[tri_index[2]]);

				//梯形为 [lerp_v2, v2, v3, lerp_v3]
				//三角形为 [lerp_v2, v2, v3] [lerp_v2, v3, lerp_v3]

				//输出
				outputTriangle(lerp_v2, *tri_pvert[1], *tri_pvert[2]);
				outputTriangle(lerp_v2, *tri_pvert[2], lerp_v3);
			}
			//S-2.3
			else if (counter[kOK] == 0) {
				//将自己位于一个区域的顶点旋转到首部
				int rot = (o1 == o2 ? 2 : (o1 == o3 ? 1 : 0));
				std::rotate(tri_index.begin(), tri_index.begin() + rot, tri_index.end());
				std::rotate(tri_pvert.begin(), tri_pvert.begin() + rot, tri_pvert.end());

				//分别取平面距离映射
				short o_one = (o1 == o2 ? o3 : (o1 == o3 ? o2 : o1));
				short o_two = (o_one == kTooFar ? kTooNear : kTooFar);
				const std::vector<float>* pds_one = (o_one == kTooNear ? &neards_ : &fards_);
				const std::vector<float>* pds_two = (o_two == kTooNear ? &neards_ : &fards_);

				//插值
				V lerp_v2_one = SignedDistanceLerp(*tri_pvert[0], *tri_pvert[1], (*pds_one)[tri_index[0]], (*pds_one)[tri_index[1]]);
				V lerp_v3_one = SignedDistanceLerp(*tri_pvert[0], *tri_pvert[2], (*pds_one)[tri_index[0]], (*pds_one)[tri_index[2]]);
				V lerp_v2_two = SignedDistanceLerp(*tri_pvert[0], *tri_pvert[1], (*pds_two)[tri_index[0]], (*pds_two)[tri_index[1]]);
				V lerp_v3_two = SignedDistanceLerp(*tri_pvert[0], *tri_pvert[2], (*pds_two)[tri_index[0]], (*pds_two)[tri_index[2]]);

				//梯形为 [lerp_v2_one, lerp_v2_two, lerp_v3_two, lerp_v3_one]
				//三角形为 [lerp_v2_one, lerp_v2_two, lerp_v3_two] [lerp_v2_one, lerp_v3_two, lerp_v3_one]

				//输出
				outputTriangle(lerp_v2_one, lerp_v2_two, lerp_v3_two);
				outputTriangle(lerp_v2_one, lerp_v3_two, lerp_v3_one);
			}
		}
		else if (max_count == 1) {
			//S - 3.1
			//将kOK的顶点旋转到首部
			int rot = (o1 == kOK ? 0 : (o2 == kOK ? 1 : 2));
			std::rotate(tri_index.begin(), tri_index.begin() + rot, tri_index.end());
			std::rotate(tri_pvert.begin(), tri_pvert.begin() + rot, tri_pvert.end());
			
			//方位标识也要旋转
			std::array<short, 3> tri_o = { o1, o2, o3 };
			std::rotate(tri_o.begin(), tri_o.begin() + rot, tri_o.end());

			//分别取平面距离映射
			const std::vector<float>* pds2 = (tri_o[1] == kTooNear ? &neards_ : &fards_);
			const std::vector<float>* pds3 = (tri_o[2] == kTooNear ? &neards_ : &fards_);

			//插值
			V lerp_v01 = SignedDistanceLerp(*tri_pvert[0], *tri_pvert[1], (*pds2)[tri_index[0]], (*pds2)[tri_index[1]]);
			V lerp_v02 = SignedDistanceLerp(*tri_pvert[0], *tri_pvert[2], (*pds3)[tri_index[0]], (*pds3)[tri_index[2]]);
			V lerp_v12_1 = SignedDistanceLerp(*tri_pvert[1], *tri_pvert[2], (*pds2)[tri_index[1]], (*pds2)[tri_index[2]]);
			V lerp_v12_2 = SignedDistanceLerp(*tri_pvert[1], *tri_pvert[2], (*pds3)[tri_index[1]], (*pds3)[tri_index[2]]);

			//五边形为 [v1, lerp_v01, lerp_v12_1, lerp_v12_2, lerp_v02]
			//三角形为 [v1, lerp_v01, lerp_v12_1] [v1, lerp_v12_1, lerp_v12_2] [v1, lerp_v12_2, lerp_v02]

			//输出
			outputTriangle(v1, lerp_v01, lerp_v12_1);
			outputTriangle(v1, lerp_v12_1, lerp_v12_2);
			outputTriangle(v1, lerp_v12_2, lerp_v02);
		}
	}

	inline void outputTriangle(const V& v1, const V& v2, const V& v3) {
		oprims_.addTriangle(v1, v2, v3);
	}

private:
	SoftPrimitiveList<V>* iprims_;
	std::vector<short> oris_;
	std::vector<float> neards_;
	std::vector<float> fards_;
	SoftPrimitiveList<V> oprims_;
	Profiler* profiler_;
	bool refuse_back_;
};


SHAKURAS_END