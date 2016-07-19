#pragma once
#include "Utility.h"
#include <tuple>


SHAKURAS_BEGIN;


enum TraversalBehavior {
	kTBNone = 0,
	kTBLerp = 1U << 0,//线性插值
	kTBPerspect = 1U << 1,//透视
	kTBAll = kTBLerp | kTBPerspect
};


template<typename T, uint16_t B>
class Varying {
public:
	T value;

	inline void perspectMul(float t) {
		if (B & kTBPerspect) value = value * t;
	}
	inline void lerpMul(float t) {
		if (B & kTBLerp) value = value * t;
	}
	inline void lerpPlus(const Varying<T, B>& v2) {
		if (B & kTBLerp) value = value + v2.value;
	}
	inline void lerpSub(const Varying<T, B>& v2) {
		if (B & kTBLerp) value = value - v2.value;
	}
};


//std::tuple<GsVarying<T0, B0>, GsVarying<T1, B1> ...>视作VaryingList


template<class VaryingList>
struct IteratorCaller {
	template<int Index>
	static inline void PerspectMul(VaryingList& v1, float t) {
		std::get<Index>(v1).perspectMul(t);
		PerspectMul<Index + 1>(v1, t);
	}

	template<int Index>
	static inline void LerpMul(VaryingList& v1, float t) {
		std::get<Index>(v1).lerpMul(t);
		LerpMul<Index + 1>(v1, t);
	}

	template<int Index>
	static inline void LerpPlus(VaryingList& v1, const VaryingList& v2) {
		std::get<Index>(v1).lerpPlus(std::get<Index>(v2));
		LerpPlus<Index + 1>(v1, v2);
	}

	template<int Index>
	static inline void LerpSub(VaryingList& v1, const VaryingList& v2) {
		std::get<Index>(v1).lerpSub(std::get<Index>(v2));
		LerpSub<Index + 1>(v1, v2);
	}
	
	template<> 
	static inline void PerspectMul<std::tuple_size<VaryingList>::value>(VaryingList& v1, float t) {}
	template<> 
	static inline void LerpMul<std::tuple_size<VaryingList>::value>(VaryingList& v1, float t) {}
	template<> 
	static inline void LerpPlus<std::tuple_size<VaryingList>::value>(VaryingList& v1, const VaryingList& v2) {}
	template<>
	static inline void LerpSub<std::tuple_size<VaryingList>::value>(VaryingList& v1, const VaryingList& v2) {}
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