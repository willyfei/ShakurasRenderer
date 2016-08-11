#pragma once
#include "SoftPrimitiveList.h"
#include "Core/MathAndGeometry.h"
#include "Core/Profiler.h"
#include <algorithm>
#include <array>
#include <map>


SHAKURAS_BEGIN;


/*inline int CheckCVV(const Vector4f& v) {
	int check = 0;
	if (v.z > v.w) check |= 1;
	if (v.z < 0.0f) check |= 2;
	if (v.y > v.w) check |= 4;
	if (v.y < -v.w) check |= 8;
	if (v.x > v.w) check |= 16;
	if (v.x < -v.w) check |= 32;
	return check;
}*/


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
	SoftClipper() {
		iprims_ = nullptr;
		profiler_ = nullptr;
		refuse_back_ = true;
	}

public:
	void reset(SoftPrimitiveList<V>& prims, Profiler& profiler, bool refuse_back) {
		iprims_ = &prims;
		profiler_ = &profiler;
		refuse_back_ = refuse_back;

		oris_.clear();
		neards_.clear();
		fards_.clear();

		lerps_.clear();
		lerpdict_.clear();

		tridict_.clear();

		overts_.clear();
		otris_.clear();
	}

	void process() {
		if (!iprims_ || !profiler_) {
			return;
		}
		
		computeOrientate();

		computeLerpVertex();

		computeClipedTriangle();

		iprims_->verts_.swap(overts_);
		iprims_->tris_.swap(otris_);
	}

private:
	// 0 : (, near)
	// 1 : [near, far]
	// 2 : (far, )
	static const short kTooNear = 0;
	static const short kOK = 1;
	static const short kTooFar = 2;

	inline short orientate(const Vector4f& pos, float& neard, float& fard) {
		static const Vector4f near_plane = { 0.0f, 0.0f, 1.0f, 0.0f };
		static const Vector4f far_plane = { 0.0f, 0.0f, -1.0f, 1.0f };

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
		oris_.resize(iprims_->verts_.size(), -1);
		neards_.resize(iprims_->verts_.size());
		fards_.resize(iprims_->verts_.size());
		
		auto calc_orient = [&](size_t i) {
			float neard = 0.0f, fard = 0.0f;
			oris_[i] = orientate(iprims_->verts_[i].pos, neard, fard);
			neards_[i] = neard;
			fards_[i] = fard;
		};

		Concurrency::parallel_for(size_t(0), iprims_->verts_.size(), calc_orient);
	}

	void allocLerpVertex(size_t i1, size_t i2, short o1, short o2) {
		if (o1 == o2) {
			return;
		}

		if (o1 == kOK || o2 == kOK) {
			//插值一个点
			short oo = (o1 == kOK ? o2 : o1);

			auto mm = std::minmax(i1, i2);

			lerps_.push_back(mm);
			lerpdict_[mm.first][mm.second][oo] = overts_.size();
			overts_.push_back(V());
		}
		else {
			//插值两个点
			auto mm = std::minmax(i1, i2);

			lerps_.push_back(mm);
			lerpdict_[mm.first][mm.second][kTooNear] = overts_.size();
			overts_.push_back(V());
			lerpdict_[mm.first][mm.second][kTooFar] = overts_.size();
			overts_.push_back(V());
		}
	}

	size_t lerpIndex(size_t i1, size_t i2, short flag) {
		auto mm = std::minmax(i1, i2);
		return lerpdict_[mm.first][mm.second][flag];
	}

	void computeLerpVertex() {
		overts_ = iprims_->verts_;

		//分配空间
		for (size_t i = 0; i != iprims_->tris_.size(); i++) {
			const size_t i1 = iprims_->tris_[i][0];
			const size_t i2 = iprims_->tris_[i][1];
			const size_t i3 = iprims_->tris_[i][2];

			const short o1 = oris_[i1];
			const short o2 = oris_[i2];
			const short o3 = oris_[i3];

			allocLerpVertex(i1, i2, o1, o2);
			allocLerpVertex(i1, i3, o1, o3);
			allocLerpVertex(i2, i3, o2, o3);
		}

		//插值
		auto calc_lerp = [&](const std::pair<size_t, size_t>& mm) {
			size_t i1 = mm.first;
			size_t i2 = mm.second;

			std::map<short, size_t>& dict = lerpdict_[i1][i2];
			if (dict.size() == 1) {
				short oo = dict.begin()->first;
				size_t i3 = dict.begin()->second;
				const std::vector<float>* pds = (oo == kTooNear ? &neards_ : &fards_);

				overts_[i3] = SignedDistanceLerp(overts_[i1], overts_[i2], (*pds)[i1], (*pds)[i2]);
			}
			else if (dict.size() == 2) {
				size_t i3 = dict[kTooNear];
				size_t i4 = dict[kTooFar];

				overts_[i3] = SignedDistanceLerp(overts_[i1], overts_[i2], neards_[i1], neards_[i2]);
				overts_[i4] = SignedDistanceLerp(overts_[i1], overts_[i2], fards_[i1], fards_[i2]);
			}
		};

		Concurrency::parallel_for_each(lerps_.begin(), lerps_.end(), calc_lerp);
	}

	void allocTriangle(size_t i) {
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

		if (max_count == 3) {
			//S-1.1
			if (counter[kOK] == 3) {
				tridict_[i].push_back(otris_.size());
				otris_.push_back({ 0, 0, 0 });
			}

			//S-1.2
			//S-1.3
		}
		else if (max_count == 2) {
			//S-2.1
			if (counter[kOK] == 1) {
				tridict_[i].push_back(otris_.size());
				otris_.push_back({ 0, 0, 0 });
			}
			//S-2.2
			else if (counter[kOK] == 2) {
				tridict_[i].push_back(otris_.size());
				otris_.push_back({ 0, 0, 0 });

				tridict_[i].push_back(otris_.size());
				otris_.push_back({ 0, 0, 0 });
			}
			//S-2.3
			else if (counter[kOK] == 0) {
				tridict_[i].push_back(otris_.size());
				otris_.push_back({ 0, 0, 0 });

				tridict_[i].push_back(otris_.size());
				otris_.push_back({ 0, 0, 0 });
			}
		}
		else if (max_count == 1) {
			//S - 3.1
			tridict_[i].push_back(otris_.size());
			otris_.push_back({ 0, 0, 0 });

			tridict_[i].push_back(otris_.size());
			otris_.push_back({ 0, 0, 0 });

			tridict_[i].push_back(otris_.size());
			otris_.push_back({ 0, 0, 0 });
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

		const std::vector<size_t>& dict = tridict_[i];
		
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

		if (max_count == 3) {
			//S-1.1
			if (counter[kOK] == 3) {
				otris_[dict[0]] = { i1, i2, i3 };
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

				//取合适的平面距离映射
				short oo = (o1 != kOK ? o1 : (o2 != kOK ? o2 : o3));

				//插值
				size_t lerp_v2 = lerpIndex(tri_index[0], tri_index[1], oo);
				size_t lerp_v3 = lerpIndex(tri_index[0], tri_index[2], oo);

				//输出
				otris_[dict[0]] = { tri_index[0], lerp_v2, lerp_v3 };
			}
			//S-2.2
			else if (counter[kOK] == 2) {
				//将非kOK的顶点旋转到首部
				int rot = (o1 != kOK ? 0 : (o2 != kOK ? 1 : 2));
				std::rotate(tri_index.begin(), tri_index.begin() + rot, tri_index.end());

				//取合适的平面距离映射
				short oo = (o1 != kOK ? o1 : (o2 != kOK ? o2 : o3));

				//插值
				size_t lerp_v2 = lerpIndex(tri_index[0], tri_index[1], oo);
				size_t lerp_v3 = lerpIndex(tri_index[0], tri_index[2], oo);

				//梯形为 [lerp_v2, v2, v3, lerp_v3]
				//三角形为 [lerp_v2, v2, v3] [lerp_v2, v3, lerp_v3]

				//输出
				otris_[dict[0]] = { lerp_v2, tri_index[1], tri_index[2] };
				otris_[dict[1]] = { lerp_v2, tri_index[2], lerp_v3 };
			}
			//S-2.3
			else if (counter[kOK] == 0) {
				//将自己位于一个区域的顶点旋转到首部
				int rot = (o1 == o2 ? 2 : (o1 == o3 ? 1 : 0));
				std::rotate(tri_index.begin(), tri_index.begin() + rot, tri_index.end());

				//分别取平面距离映射
				short o_one = (o1 == o2 ? o3 : (o1 == o3 ? o2 : o1));
				short o_two = (o_one == kTooFar ? kTooNear : kTooFar);

				//插值
				size_t lerp_v2_one = lerpIndex(tri_index[0], tri_index[1], o_one);
				size_t lerp_v3_one = lerpIndex(tri_index[0], tri_index[2], o_one);
				size_t lerp_v2_two = lerpIndex(tri_index[0], tri_index[1], o_two);
				size_t lerp_v3_two = lerpIndex(tri_index[0], tri_index[2], o_two);

				//梯形为 [lerp_v2_one, lerp_v2_two, lerp_v3_two, lerp_v3_one]
				//三角形为 [lerp_v2_one, lerp_v2_two, lerp_v3_two] [lerp_v2_one, lerp_v3_two, lerp_v3_one]

				//输出
				otris_[dict[0]] = { lerp_v2_one, lerp_v2_two, lerp_v3_two };
				otris_[dict[1]] = { lerp_v2_one, lerp_v3_two, lerp_v3_one };
			}
		}
		else if (max_count == 1) {
			//S - 3.1
			//将kOK的顶点旋转到首部
			int rot = (o1 == kOK ? 0 : (o2 == kOK ? 1 : 2));
			std::rotate(tri_index.begin(), tri_index.begin() + rot, tri_index.end());
			
			//方位标识也要旋转
			std::array<short, 3> tri_o = { o1, o2, o3 };
			std::rotate(tri_o.begin(), tri_o.begin() + rot, tri_o.end());

			//分别取平面距离映射
			short o2 = tri_o[1];
			short o3 = tri_o[2];

			//插值
			size_t lerp_v01 = lerpIndex(tri_index[0], tri_index[1], o2);
			size_t lerp_v02 = lerpIndex(tri_index[0], tri_index[2], o3);
			size_t lerp_v12_1 = lerpIndex(tri_index[1], tri_index[2], o2);
			size_t lerp_v12_2 = lerpIndex(tri_index[1], tri_index[2], o3);

			//五边形为 [v1, lerp_v01, lerp_v12_1, lerp_v12_2, lerp_v02]
			//三角形为 [v1, lerp_v01, lerp_v12_1] [v1, lerp_v12_1, lerp_v12_2] [v1, lerp_v12_2, lerp_v02]

			//输出
			otris_[dict[0]] = { tri_index[0], lerp_v01, lerp_v12_1 };
			otris_[dict[1]] = { tri_index[0], lerp_v12_1, lerp_v12_2 };
			otris_[dict[2]] = { tri_index[0], lerp_v12_2, lerp_v02 };
		}
	}

	void computeClipedTriangle() {
		tridict_.resize(iprims_->tris_.size());

		for (size_t i = 0; i != iprims_->tris_.size(); i++) {
			allocTriangle(i);
		}

		auto clip_tri = [&](size_t i) {
			clipTriangle(i);
		};

		Concurrency::parallel_for(size_t(0), iprims_->tris_.size(), clip_tri);
	}

private:
	SoftPrimitiveList<V>* iprims_;
	Profiler* profiler_;
	bool refuse_back_;

	std::vector<short> oris_;
	std::vector<float> neards_;
	std::vector<float> fards_;

	std::vector<std::pair<size_t, size_t> > lerps_;
	std::map<size_t, std::map<size_t, std::map<short, size_t> > > lerpdict_;//[v1, [v2, [flag, vlerp]]]

	std::vector<std::vector<size_t> > tridict_;//[tri_index, [cliped_tri_index]]

	std::vector<V> overts_;
	std::vector<std::array<size_t, 3> > otris_;
};


SHAKURAS_END