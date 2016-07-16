#pragma once
#include "Utility.h"
#include <tuple>


SHAKURAS_BEGIN;


enum GsTraversalBehavior {
	kTBNone = 0,
	kTBLerp = 1U << 0,//���Բ�ֵ
	kTBPerspect = 1U << 1,//͸��
	kTBAll = kTBLerp | kTBPerspect
};


class GSVNil {};


template<typename T, uint16_t B>
class GsVarying {
public:
	T value;

	inline void perspectMul(float t) {
		if (B & kTBPerspect) value = value * t;
	}
	inline void lerpMul(float t) {
		if (B & kTBLerp) value = value * t;
	}
	inline void lerpPlus(const GsVarying<T, B>& v2) {
		if (B & kTBLerp) value = value + v2.value;
	}
	inline void lerpSub(const GsVarying<T, B>& v2) {
		if (B & kTBLerp) value = value - v2.value;
	}
};


template<class VaryingList>
struct IteratorCaller {
	template<int Index>
	static void PerspectMul(VaryingList& v1, float t) {
		std::get<Index>(v1).perspectMul(t);
		PerspectMul<Index + 1>(v1, t);
	}

	template<int Index>
	static void LerpMul(VaryingList& v1, float t) {
		std::get<Index>(v1).lerpMul(t);
		LerpMul<Index + 1>(v1, t);
	}

	template<int Index>
	static void LerpPlus(VaryingList& v1, const VaryingList& v2) {
		std::get<Index>(v1).lerpPlus(std::get<Index>(v2));
		LerpPlus<Index + 1>(v1, v2);
	}

	template<int Index>
	static void LerpSub(VaryingList& v1, const VaryingList& v2) {
		std::get<Index>(v1).lerpSub(std::get<Index>(v2));
		LerpSub<Index + 1>(v1, v2);
	}
	
	template<> 
	static void PerspectMul<std::tuple_size<VaryingList>::value>(VaryingList& v1, float t) {}
	template<> 
	static void LerpMul<std::tuple_size<VaryingList>::value>(VaryingList& v1, float t) {}
	template<> 
	static void LerpPlus<std::tuple_size<VaryingList>::value>(VaryingList& v1, const VaryingList& v2) {}
	template<>
	static void LerpSub<std::tuple_size<VaryingList>::value>(VaryingList& v1, const VaryingList& v2) {}
};


template<class VaryingList>
void PerspectMul(VaryingList& v1, float t) {
	IteratorCaller<VaryingList>::PerspectMul<0>(v1, t);
}

template<class VaryingList>
void LerpMul(VaryingList& v1, float t) {
	IteratorCaller<VaryingList>::LerpMul<0>(v1, t);
}

template<class VaryingList>
void LerpPlus(VaryingList& v1, const VaryingList& v2) {
	IteratorCaller<VaryingList>::LerpPlus<0>(v1, v2);
}

template<class VaryingList>
void LerpSub(VaryingList& v1, const VaryingList& v2) {
	IteratorCaller<VaryingList>::LerpSub<0>(v1, v2);
}


SHAKURAS_END;