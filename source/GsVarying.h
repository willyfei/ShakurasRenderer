#pragma once
#include "Utility.h"
#include <type_traits>


SHAKURAS_BEGIN;


enum GsTraversalBehavior {
	kTBNone = 0,
	kTBLerp = 1U << 0,//线性插值
	kTBPerspect = 1U << 1,//透视
	kTBAll = kTBLerp | kTBPerspect
};


class GSVNil {};


template<typename T0 = GSVNil, uint16_t B0 = kTBNone
	, typename T1 = GSVNil, uint16_t B1 = kTBNone
	, typename T2 = GSVNil, uint16_t B2 = kTBNone
	, typename T3 = GSVNil, uint16_t B3 = kTBNone
	, typename T4 = GSVNil, uint16_t B4 = kTBNone
	, typename T5 = GSVNil, uint16_t B5 = kTBNone
	, typename T6 = GSVNil, uint16_t B6 = kTBNone
	, typename T7 = GSVNil, uint16_t B7 = kTBNone
	, typename T8 = GSVNil, uint16_t B8 = kTBNone
	, typename T9 = GSVNil, uint16_t B9 = kTBNone
	, typename T10 = GSVNil, uint16_t B10 = kTBNone>
class GsVarying {
};


template<typename T0, uint16_t B0>
class GsVarying<T0, B0> {
public:
	typedef GsVarying<T0, B0> self_t;
	static const int length = 1;

public:
	template<int Index> void perspectMul(float t) {}
	template<int Index> void lerpMul(float t) {}
	template<int Index> void lerpPlus(const self_t& v2) {}
	template<int Index> void lerpSub(const self_t& v2) {}

	typedef T0 argument_type0;
	static const uint16_t behavior0 = B0;
	argument_type0 value0;
	template<> inline void perspectMul<0>(float t) {
		if (behavior0 & kTBPerspect) value0 = value0 * t;
	}
	template<> inline void lerpMul<0>(float t) {
		if (behavior0 & kTBLerp) value0 = value0 * t;\
	}
	template<> inline void lerpPlus<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 + v2.value0;
	}
	template<> inline void lerpSub<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 - v2.value0;
	}
};


template<typename T0, uint16_t B0
	, typename T1, uint16_t B1>
class GsVarying<T0, B0, T1, B1> {
public:
	typedef GsVarying<T0, B0, T1, B1> self_t;
	static const int length = 2;

public:
	template<int Index> void perspectMul(float t) {}
	template<int Index> void lerpMul(float t) {}
	template<int Index> void lerpPlus(const self_t& v2) {}
	template<int Index> void lerpSub(const self_t& v2) {}

	typedef T0 argument_type0;
	static const uint16_t behavior0 = B0;
	argument_type0 value0;
	template<> inline void perspectMul<0>(float t) {
		if (behavior0 & kTBPerspect) value0 = value0 * t;
	}
	template<> inline void lerpMul<0>(float t) {
		if (behavior0 & kTBLerp) value0 = value0 * t;\
	}
	template<> inline void lerpPlus<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 + v2.value0;
	}
	template<> inline void lerpSub<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 - v2.value0;
	}

	typedef T1 argument_type1;
	static const uint16_t behavior1 = B1;
	argument_type1 value1;
	template<> inline void perspectMul<1>(float t) {
		if (behavior1 & kTBPerspect) value1 = value1 * t;
	}
	template<> inline void lerpMul<1>(float t) {
		if (behavior1 & kTBLerp) value1 = value1 * t;\
	}
	template<> inline void lerpPlus<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 + v2.value1;
	}
	template<> inline void lerpSub<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 - v2.value1;
	}
};


template<typename T0, uint16_t B0
	, typename T1, uint16_t B1
	, typename T2, uint16_t B2>
class GsVarying<T0, B0, T1, B1, T2, B2> {
public:
	typedef GsVarying<T0, B0, T1, B1, T2, B2> self_t;
	static const int length = 3;

public:
	template<int Index> void perspectMul(float t) {}
	template<int Index> void lerpMul(float t) {}
	template<int Index> void lerpPlus(const self_t& v2) {}
	template<int Index> void lerpSub(const self_t& v2) {}

	typedef T0 argument_type0;
	static const uint16_t behavior0 = B0;
	argument_type0 value0;
	template<> inline void perspectMul<0>(float t) {
		if (behavior0 & kTBPerspect) value0 = value0 * t;
	}
	template<> inline void lerpMul<0>(float t) {
		if (behavior0 & kTBLerp) value0 = value0 * t;\
	}
	template<> inline void lerpPlus<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 + v2.value0;
	}
	template<> inline void lerpSub<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 - v2.value0;
	}

	typedef T1 argument_type1;
	static const uint16_t behavior1 = B1;
	argument_type1 value1;
	template<> inline void perspectMul<1>(float t) {
		if (behavior1 & kTBPerspect) value1 = value1 * t;
	}
	template<> inline void lerpMul<1>(float t) {
		if (behavior1 & kTBLerp) value1 = value1 * t;\
	}
	template<> inline void lerpPlus<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 + v2.value1;
	}
	template<> inline void lerpSub<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 - v2.value1;
	}

	typedef T2 argument_type2;
	static const uint16_t behavior2 = B2;
	argument_type2 value2;
	template<> inline void perspectMul<2>(float t) {
		if (behavior2 & kTBPerspect) value2 = value2 * t;
	}
	template<> inline void lerpMul<2>(float t) {
		if (behavior2 & kTBLerp) value2 = value2 * t;\
	}
	template<> inline void lerpPlus<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 + v2.value2;
	}
	template<> inline void lerpSub<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 - v2.value2;
	}
};

template<typename T0, uint16_t B0
	, typename T1, uint16_t B1
	, typename T2, uint16_t B2
	, typename T3, uint16_t B3>
class GsVarying<T0, B0, T1, B1, T2, B2, T3, B3> {
public:
	typedef GsVarying<T0, B0, T1, B1, T2, B2, T3, B3> self_t;
	static const int length = 4;

public:
	template<int Index> void perspectMul(float t) {}
	template<int Index> void lerpMul(float t) {}
	template<int Index> void lerpPlus(const self_t& v2) {}
	template<int Index> void lerpSub(const self_t& v2) {}

	typedef T0 argument_type0;
	static const uint16_t behavior0 = B0;
	argument_type0 value0;
	template<> inline void perspectMul<0>(float t) {
		if (behavior0 & kTBPerspect) value0 = value0 * t;
	}
	template<> inline void lerpMul<0>(float t) {
		if (behavior0 & kTBLerp) value0 = value0 * t;\
	}
	template<> inline void lerpPlus<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 + v2.value0;
	}
	template<> inline void lerpSub<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 - v2.value0;
	}

	typedef T1 argument_type1;
	static const uint16_t behavior1 = B1;
	argument_type1 value1;
	template<> inline void perspectMul<1>(float t) {
		if (behavior1 & kTBPerspect) value1 = value1 * t;
	}
	template<> inline void lerpMul<1>(float t) {
		if (behavior1 & kTBLerp) value1 = value1 * t;\
	}
	template<> inline void lerpPlus<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 + v2.value1;
	}
	template<> inline void lerpSub<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 - v2.value1;
	}

	typedef T2 argument_type2;
	static const uint16_t behavior2 = B2;
	argument_type2 value2;
	template<> inline void perspectMul<2>(float t) {
		if (behavior2 & kTBPerspect) value2 = value2 * t;
	}
	template<> inline void lerpMul<2>(float t) {
		if (behavior2 & kTBLerp) value2 = value2 * t;\
	}
	template<> inline void lerpPlus<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 + v2.value2;
	}
	template<> inline void lerpSub<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 - v2.value2;
	}

	typedef T3 argument_type3;
	static const uint16_t behavior3 = B3;
	argument_type3 value3;
	template<> inline void perspectMul<3>(float t) {
		if (behavior3 & kTBPerspect) value3 = value3 * t;
	}
	template<> inline void lerpMul<3>(float t) {
		if (behavior3 & kTBLerp) value3 = value3 * t;\
	}
	template<> inline void lerpPlus<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 + v2.value3;
	}
	template<> inline void lerpSub<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 - v2.value3;
	}
};

template<typename T0, uint16_t B0
	, typename T1, uint16_t B1
	, typename T2, uint16_t B2
	, typename T3, uint16_t B3
	, typename T4, uint16_t B4>
class GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4> {
public:
	typedef GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4> self_t;
	static const int length = 5;

public:
	template<int Index> void perspectMul(float t) {}
	template<int Index> void lerpMul(float t) {}
	template<int Index> void lerpPlus(const self_t& v2) {}
	template<int Index> void lerpSub(const self_t& v2) {}

	typedef T0 argument_type0;
	static const uint16_t behavior0 = B0;
	argument_type0 value0;
	template<> inline void perspectMul<0>(float t) {
		if (behavior0 & kTBPerspect) value0 = value0 * t;
	}
	template<> inline void lerpMul<0>(float t) {
		if (behavior0 & kTBLerp) value0 = value0 * t;\
	}
	template<> inline void lerpPlus<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 + v2.value0;
	}
	template<> inline void lerpSub<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 - v2.value0;
	}

	typedef T1 argument_type1;
	static const uint16_t behavior1 = B1;
	argument_type1 value1;
	template<> inline void perspectMul<1>(float t) {
		if (behavior1 & kTBPerspect) value1 = value1 * t;
	}
	template<> inline void lerpMul<1>(float t) {
		if (behavior1 & kTBLerp) value1 = value1 * t;\
	}
	template<> inline void lerpPlus<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 + v2.value1;
	}
	template<> inline void lerpSub<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 - v2.value1;
	}

	typedef T2 argument_type2;
	static const uint16_t behavior2 = B2;
	argument_type2 value2;
	template<> inline void perspectMul<2>(float t) {
		if (behavior2 & kTBPerspect) value2 = value2 * t;
	}
	template<> inline void lerpMul<2>(float t) {
		if (behavior2 & kTBLerp) value2 = value2 * t;\
	}
	template<> inline void lerpPlus<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 + v2.value2;
	}
	template<> inline void lerpSub<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 - v2.value2;
	}

	typedef T3 argument_type3;
	static const uint16_t behavior3 = B3;
	argument_type3 value3;
	template<> inline void perspectMul<3>(float t) {
		if (behavior3 & kTBPerspect) value3 = value3 * t;
	}
	template<> inline void lerpMul<3>(float t) {
		if (behavior3 & kTBLerp) value3 = value3 * t;\
	}
	template<> inline void lerpPlus<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 + v2.value3;
	}
	template<> inline void lerpSub<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 - v2.value3;
	}

	typedef T4 argument_type4;
	static const uint16_t behavior4 = B4;
	argument_type4 value4;
	template<> inline void perspectMul<4>(float t) {
		if (behavior4 & kTBPerspect) value4 = value4 * t;
	}
	template<> inline void lerpMul<4>(float t) {
		if (behavior4 & kTBLerp) value4 = value4 * t;\
	}
	template<> inline void lerpPlus<4>(const self_t& v2) {
		if (behavior4 & kTBLerp) value4 = value4 + v2.value4;
	}
	template<> inline void lerpSub<4>(const self_t& v2) {
		if (behavior4 & kTBLerp) value4 = value4 - v2.value4;
	}
};

template<typename T0, uint16_t B0
	, typename T1, uint16_t B1
	, typename T2, uint16_t B2
	, typename T3, uint16_t B3
	, typename T4, uint16_t B4
	, typename T5, uint16_t B5>
class GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4, T5, B5> {
public:
	typedef GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4, T5, B5> self_t;
	static const int length = 6;

public:
	template<int Index> void perspectMul(float t) {}
	template<int Index> void lerpMul(float t) {}
	template<int Index> void lerpPlus(const self_t& v2) {}
	template<int Index> void lerpSub(const self_t& v2) {}

	typedef T0 argument_type0;
	static const uint16_t behavior0 = B0;
	argument_type0 value0;
	template<> inline void perspectMul<0>(float t) {
		if (behavior0 & kTBPerspect) value0 = value0 * t;
	}
	template<> inline void lerpMul<0>(float t) {
		if (behavior0 & kTBLerp) value0 = value0 * t;\
	}
	template<> inline void lerpPlus<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 + v2.value0;
	}
	template<> inline void lerpSub<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 - v2.value0;
	}

	typedef T1 argument_type1;
	static const uint16_t behavior1 = B1;
	argument_type1 value1;
	template<> inline void perspectMul<1>(float t) {
		if (behavior1 & kTBPerspect) value1 = value1 * t;
	}
	template<> inline void lerpMul<1>(float t) {
		if (behavior1 & kTBLerp) value1 = value1 * t;\
	}
	template<> inline void lerpPlus<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 + v2.value1;
	}
	template<> inline void lerpSub<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 - v2.value1;
	}

	typedef T2 argument_type2;
	static const uint16_t behavior2 = B2;
	argument_type2 value2;
	template<> inline void perspectMul<2>(float t) {
		if (behavior2 & kTBPerspect) value2 = value2 * t;
	}
	template<> inline void lerpMul<2>(float t) {
		if (behavior2 & kTBLerp) value2 = value2 * t;\
	}
	template<> inline void lerpPlus<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 + v2.value2;
	}
	template<> inline void lerpSub<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 - v2.value2;
	}

	typedef T3 argument_type3;
	static const uint16_t behavior3 = B3;
	argument_type3 value3;
	template<> inline void perspectMul<3>(float t) {
		if (behavior3 & kTBPerspect) value3 = value3 * t;
	}
	template<> inline void lerpMul<3>(float t) {
		if (behavior3 & kTBLerp) value3 = value3 * t;\
	}
	template<> inline void lerpPlus<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 + v2.value3;
	}
	template<> inline void lerpSub<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 - v2.value3;
	}

	typedef T4 argument_type4;
	static const uint16_t behavior4 = B4;
	argument_type4 value4;
	template<> inline void perspectMul<4>(float t) {
		if (behavior4 & kTBPerspect) value4 = value4 * t;
	}
	template<> inline void lerpMul<4>(float t) {
		if (behavior4 & kTBLerp) value4 = value4 * t;\
	}
	template<> inline void lerpPlus<4>(const self_t& v2) {
		if (behavior4 & kTBLerp) value4 = value4 + v2.value4;
	}
	template<> inline void lerpSub<4>(const self_t& v2) {
		if (behavior4 & kTBLerp) value4 = value4 - v2.value4;
	}
	
	typedef T5 argument_type5;
	static const uint16_t behavior5 = B5;
	argument_type5 value5;
	template<> inline void perspectMul<5>(float t) {
		if (behavior5 & kTBPerspect) value5 = value5 * t;
	}
	template<> inline void lerpMul<5>(float t) {
		if (behavior5 & kTBLerp) value5 = value5 * t;\
	}
	template<> inline void lerpPlus<5>(const self_t& v2) {
		if (behavior5 & kTBLerp) value5 = value5 + v2.value5;
	}
	template<> inline void lerpSub<5>(const self_t& v2) {
		if (behavior5 & kTBLerp) value5 = value5 - v2.value5;
	}
};

template<typename T0, uint16_t B0
	, typename T1, uint16_t B1
	, typename T2, uint16_t B2
	, typename T3, uint16_t B3
	, typename T4, uint16_t B4
	, typename T5, uint16_t B5
	, typename T6, uint16_t B6>
class GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4, T5, B5, T6, B6> {
public:
	typedef GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4, T5, B5, T6, B6> self_t;
	static const int length = 7;

public:
	template<int Index> void perspectMul(float t) {}
	template<int Index> void lerpMul(float t) {}
	template<int Index> void lerpPlus(const self_t& v2) {}
	template<int Index> void lerpSub(const self_t& v2) {}

	typedef T0 argument_type0;
	static const uint16_t behavior0 = B0;
	argument_type0 value0;
	template<> inline void perspectMul<0>(float t) {
		if (behavior0 & kTBPerspect) value0 = value0 * t;
	}
	template<> inline void lerpMul<0>(float t) {
		if (behavior0 & kTBLerp) value0 = value0 * t;\
	}
	template<> inline void lerpPlus<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 + v2.value0;
	}
	template<> inline void lerpSub<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 - v2.value0;
	}

	typedef T1 argument_type1;
	static const uint16_t behavior1 = B1;
	argument_type1 value1;
	template<> inline void perspectMul<1>(float t) {
		if (behavior1 & kTBPerspect) value1 = value1 * t;
	}
	template<> inline void lerpMul<1>(float t) {
		if (behavior1 & kTBLerp) value1 = value1 * t;\
	}
	template<> inline void lerpPlus<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 + v2.value1;
	}
	template<> inline void lerpSub<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 - v2.value1;
	}

	typedef T2 argument_type2;
	static const uint16_t behavior2 = B2;
	argument_type2 value2;
	template<> inline void perspectMul<2>(float t) {
		if (behavior2 & kTBPerspect) value2 = value2 * t;
	}
	template<> inline void lerpMul<2>(float t) {
		if (behavior2 & kTBLerp) value2 = value2 * t;\
	}
	template<> inline void lerpPlus<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 + v2.value2;
	}
	template<> inline void lerpSub<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 - v2.value2;
	}
	
	typedef T3 argument_type3;
	static const uint16_t behavior3 = B3;
	argument_type3 value3;
	template<> inline void perspectMul<3>(float t) {
		if (behavior3 & kTBPerspect) value3 = value3 * t;
	}
	template<> inline void lerpMul<3>(float t) {
		if (behavior3 & kTBLerp) value3 = value3 * t;\
	}
	template<> inline void lerpPlus<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 + v2.value3;
	}
	template<> inline void lerpSub<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 - v2.value3;
	}

	typedef T4 argument_type4;
	static const uint16_t behavior4 = B4;
	argument_type4 value4;
	template<> inline void perspectMul<4>(float t) {
		if (behavior4 & kTBPerspect) value4 = value4 * t;
	}
	template<> inline void lerpMul<4>(float t) {
		if (behavior4 & kTBLerp) value4 = value4 * t;\
	}
	template<> inline void lerpPlus<4>(const self_t& v2) {
		if (behavior4 & kTBLerp) value4 = value4 + v2.value4;
	}
	template<> inline void lerpSub<4>(const self_t& v2) {
		if (behavior4 & kTBLerp) value4 = value4 - v2.value4;
	}

	typedef T5 argument_type5;
	static const uint16_t behavior5 = B5;
	argument_type5 value5;
	template<> inline void perspectMul<5>(float t) {
		if (behavior5 & kTBPerspect) value5 = value5 * t;
	}
	template<> inline void lerpMul<5>(float t) {
		if (behavior5 & kTBLerp) value5 = value5 * t;\
	}
	template<> inline void lerpPlus<5>(const self_t& v2) {
		if (behavior5 & kTBLerp) value5 = value5 + v2.value5;
	}
	template<> inline void lerpSub<5>(const self_t& v2) {
		if (behavior5 & kTBLerp) value5 = value5 - v2.value5;
	}

	typedef T6 argument_type6;
	static const uint16_t behavior6 = B6;
	argument_type6 value6;
	template<> inline void perspectMul<6>(float t) {
		if (behavior6 & kTBPerspect) value6 = value6 * t;
	}
	template<> inline void lerpMul<6>(float t) {
		if (behavior6 & kTBLerp) value6 = value6 * t;\
	}
	template<> inline void lerpPlus<6>(const self_t& v2) {
		if (behavior6 & kTBLerp) value6 = value6 + v2.value6;
	}
	template<> inline void lerpSub<6>(const self_t& v2) {
		if (behavior6 & kTBLerp) value6 = value6 - v2.value6;
	}
};

template<typename T0, uint16_t B0
	, typename T1, uint16_t B1
	, typename T2, uint16_t B2
	, typename T3, uint16_t B3
	, typename T4, uint16_t B4
	, typename T5, uint16_t B5
	, typename T6, uint16_t B6
	, typename T7, uint16_t B7>
class GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4, T5, B5, T6, B6, T7, B7> {
public:
	typedef GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4, T5, B5, T6, B6, T7, B7> self_t;
	static const int length = 8;

public:
	template<int Index> void perspectMul(float t) {}
	template<int Index> void lerpMul(float t) {}
	template<int Index> void lerpPlus(const self_t& v2) {}
	template<int Index> void lerpSub(const self_t& v2) {}

	typedef T0 argument_type0;
	static const uint16_t behavior0 = B0;
	argument_type0 value0;
	template<> inline void perspectMul<0>(float t) {
		if (behavior0 & kTBPerspect) value0 = value0 * t;
	}
	template<> inline void lerpMul<0>(float t) {
		if (behavior0 & kTBLerp) value0 = value0 * t;\
	}
	template<> inline void lerpPlus<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 + v2.value0;
	}
	template<> inline void lerpSub<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 - v2.value0;
	}

	typedef T1 argument_type1;
	static const uint16_t behavior1 = B1;
	argument_type1 value1;
	template<> inline void perspectMul<1>(float t) {
		if (behavior1 & kTBPerspect) value1 = value1 * t;
	}
	template<> inline void lerpMul<1>(float t) {
		if (behavior1 & kTBLerp) value1 = value1 * t;\
	}
	template<> inline void lerpPlus<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 + v2.value1;
	}
	template<> inline void lerpSub<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 - v2.value1;
	}

	typedef T2 argument_type2;
	static const uint16_t behavior2 = B2;
	argument_type2 value2;
	template<> inline void perspectMul<2>(float t) {
		if (behavior2 & kTBPerspect) value2 = value2 * t;
	}
	template<> inline void lerpMul<2>(float t) {
		if (behavior2 & kTBLerp) value2 = value2 * t;\
	}
	template<> inline void lerpPlus<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 + v2.value2;
	}
	template<> inline void lerpSub<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 - v2.value2;
	}

	typedef T3 argument_type3;
	static const uint16_t behavior3 = B3;
	argument_type3 value3;
	template<> inline void perspectMul<3>(float t) {
		if (behavior3 & kTBPerspect) value3 = value3 * t;
	}
	template<> inline void lerpMul<3>(float t) {
		if (behavior3 & kTBLerp) value3 = value3 * t;\
	}
	template<> inline void lerpPlus<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 + v2.value3;
	}
	template<> inline void lerpSub<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 - v2.value3;
	}

	typedef T4 argument_type4;
	static const uint16_t behavior4 = B4;
	argument_type4 value4;
	template<> inline void perspectMul<4>(float t) {
		if (behavior4 & kTBPerspect) value4 = value4 * t;
	}
	template<> inline void lerpMul<4>(float t) {
		if (behavior4 & kTBLerp) value4 = value4 * t;\
	}
	template<> inline void lerpPlus<4>(const self_t& v2) {
		if (behavior4 & kTBLerp) value4 = value4 + v2.value4;
	}
	template<> inline void lerpSub<4>(const self_t& v2) {
		if (behavior4 & kTBLerp) value4 = value4 - v2.value4;
	}

	typedef T5 argument_type5;
	static const uint16_t behavior5 = B5;
	argument_type5 value5;
	template<> inline void perspectMul<5>(float t) {
		if (behavior5 & kTBPerspect) value5 = value5 * t;
	}
	template<> inline void lerpMul<5>(float t) {
		if (behavior5 & kTBLerp) value5 = value5 * t;\
	}
	template<> inline void lerpPlus<5>(const self_t& v2) {
		if (behavior5 & kTBLerp) value5 = value5 + v2.value5;
	}
	template<> inline void lerpSub<5>(const self_t& v2) {
		if (behavior5 & kTBLerp) value5 = value5 - v2.value5;
	}

	typedef T6 argument_type6;
	static const uint16_t behavior6 = B6;
	argument_type6 value6;
	template<> inline void perspectMul<6>(float t) {
		if (behavior6 & kTBPerspect) value6 = value6 * t;
	}
	template<> inline void lerpMul<6>(float t) {
		if (behavior6 & kTBLerp) value6 = value6 * t;\
	}
	template<> inline void lerpPlus<6>(const self_t& v2) {
		if (behavior6 & kTBLerp) value6 = value6 + v2.value6;
	}
	template<> inline void lerpSub<6>(const self_t& v2) {
		if (behavior6 & kTBLerp) value6 = value6 - v2.value6;
	}

	typedef T7 argument_type7;
	static const uint16_t behavior7 = B7;
	argument_type7 value7;
	template<> inline void perspectMul<7>(float t) {
		if (behavior7 & kTBPerspect) value7 = value7 * t;
	}
	template<> inline void lerpMul<7>(float t) {
		if (behavior7 & kTBLerp) value7 = value7 * t;\
	}
	template<> inline void lerpPlus<7>(const self_t& v2) {
		if (behavior7 & kTBLerp) value7 = value7 + v2.value7;
	}
	template<> inline void lerpSub<7>(const self_t& v2) {
		if (behavior7 & kTBLerp) value7 = value7 - v2.value7;
	}
};

template<typename T0, uint16_t B0
	, typename T1, uint16_t B1
	, typename T2, uint16_t B2
	, typename T3, uint16_t B3
	, typename T4, uint16_t B4
	, typename T5, uint16_t B5
	, typename T6, uint16_t B6
	, typename T7, uint16_t B7
	, typename T8, uint16_t B8>
class GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4, T5, B5, T6, B6, T7, B7, T8, B8> {
public:
	typedef GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4, T5, B5, T6, B6, T7, B7, T8, B8> self_t;
	static const int length = 9;

public:
	template<int Index> void perspectMul(float t) {}
	template<int Index> void lerpMul(float t) {}
	template<int Index> void lerpPlus(const self_t& v2) {}
	template<int Index> void lerpSub(const self_t& v2) {}

	typedef T0 argument_type0;
	static const uint16_t behavior0 = B0;
	argument_type0 value0;
	template<> inline void perspectMul<0>(float t) {
		if (behavior0 & kTBPerspect) value0 = value0 * t;
	}
	template<> inline void lerpMul<0>(float t) {
		if (behavior0 & kTBLerp) value0 = value0 * t;\
	}
	template<> inline void lerpPlus<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 + v2.value0;
	}
	template<> inline void lerpSub<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 - v2.value0;
	}

	typedef T1 argument_type1;
	static const uint16_t behavior1 = B1;
	argument_type1 value1;
	template<> inline void perspectMul<1>(float t) {
		if (behavior1 & kTBPerspect) value1 = value1 * t;
	}
	template<> inline void lerpMul<1>(float t) {
		if (behavior1 & kTBLerp) value1 = value1 * t;\
	}
	template<> inline void lerpPlus<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 + v2.value1;
	}
	template<> inline void lerpSub<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 - v2.value1;
	}

	typedef T2 argument_type2;
	static const uint16_t behavior2 = B2;
	argument_type2 value2;
	template<> inline void perspectMul<2>(float t) {
		if (behavior2 & kTBPerspect) value2 = value2 * t;
	}
	template<> inline void lerpMul<2>(float t) {
		if (behavior2 & kTBLerp) value2 = value2 * t;\
	}
	template<> inline void lerpPlus<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 + v2.value2;
	}
	template<> inline void lerpSub<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 - v2.value2;
	}

	typedef T3 argument_type3;
	static const uint16_t behavior3 = B3;
	argument_type3 value3;
	template<> inline void perspectMul<3>(float t) {
		if (behavior3 & kTBPerspect) value3 = value3 * t;
	}
	template<> inline void lerpMul<3>(float t) {
		if (behavior3 & kTBLerp) value3 = value3 * t;\
	}
	template<> inline void lerpPlus<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 + v2.value3;
	}
	template<> inline void lerpSub<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 - v2.value3;
	}

	typedef T4 argument_type4;
	static const uint16_t behavior4 = B4;
	argument_type4 value4;
	template<> inline void perspectMul<4>(float t) {
		if (behavior4 & kTBPerspect) value4 = value4 * t;
	}
	template<> inline void lerpMul<4>(float t) {
		if (behavior4 & kTBLerp) value4 = value4 * t;\
	}
	template<> inline void lerpPlus<4>(const self_t& v2) {
		if (behavior4 & kTBLerp) value4 = value4 + v2.value4;
	}
	template<> inline void lerpSub<4>(const self_t& v2) {
		if (behavior4 & kTBLerp) value4 = value4 - v2.value4;
	}

	typedef T5 argument_type5;
	static const uint16_t behavior5 = B5;
	argument_type5 value5;
	template<> inline void perspectMul<5>(float t) {
		if (behavior5 & kTBPerspect) value5 = value5 * t;
	}
	template<> inline void lerpMul<5>(float t) {
		if (behavior5 & kTBLerp) value5 = value5 * t;\
	}
	template<> inline void lerpPlus<5>(const self_t& v2) {
		if (behavior5 & kTBLerp) value5 = value5 + v2.value5;
	}
	template<> inline void lerpSub<5>(const self_t& v2) {
		if (behavior5 & kTBLerp) value5 = value5 - v2.value5;
	}

	typedef T6 argument_type6;
	static const uint16_t behavior6 = B6;
	argument_type6 value6;
	template<> inline void perspectMul<6>(float t) {
		if (behavior6 & kTBPerspect) value6 = value6 * t;
	}
	template<> inline void lerpMul<6>(float t) {
		if (behavior6 & kTBLerp) value6 = value6 * t;\
	}
	template<> inline void lerpPlus<6>(const self_t& v2) {
		if (behavior6 & kTBLerp) value6 = value6 + v2.value6;
	}
	template<> inline void lerpSub<6>(const self_t& v2) {
		if (behavior6 & kTBLerp) value6 = value6 - v2.value6;
	}

	typedef T7 argument_type7;
	static const uint16_t behavior7 = B7;
	argument_type7 value7;
	template<> inline void perspectMul<7>(float t) {
		if (behavior7 & kTBPerspect) value7 = value7 * t;
	}
	template<> inline void lerpMul<7>(float t) {
		if (behavior7 & kTBLerp) value7 = value7 * t;\
	}
	template<> inline void lerpPlus<7>(const self_t& v2) {
		if (behavior7 & kTBLerp) value7 = value7 + v2.value7;
	}
	template<> inline void lerpSub<7>(const self_t& v2) {
		if (behavior7 & kTBLerp) value7 = value7 - v2.value7;
	}

	typedef T8 argument_type8;
	static const uint16_t behavior8 = B8;
	argument_type8 value8;
	template<> inline void perspectMul<8>(float t) {
		if (behavior8 & kTBPerspect) value8 = value8 * t;
	}
	template<> inline void lerpMul<8>(float t) {
		if (behavior8 & kTBLerp) value8 = value8 * t;\
	}
	template<> inline void lerpPlus<8>(const self_t& v2) {
		if (behavior8 & kTBLerp) value8 = value8 + v2.value8;
	}
	template<> inline void lerpSub<8>(const self_t& v2) {
		if (behavior8 & kTBLerp) value8 = value8 - v2.value8;
	}
};

template<typename T0, uint16_t B0
	, typename T1, uint16_t B1
	, typename T2, uint16_t B2
	, typename T3, uint16_t B3
	, typename T4, uint16_t B4
	, typename T5, uint16_t B5
	, typename T6, uint16_t B6
	, typename T7, uint16_t B7
	, typename T8, uint16_t B8
	, typename T9, uint16_t B9>
class GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4, T5, B5, T6, B6, T7, B7, T8, B8, T9, B9> {
public:
	typedef GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4, T5, B5, T6, B6, T7, B7, T8, B8, T9, B9> self_t;
	static const int length = 10;

public:
	template<int Index> void perspectMul(float t) {}
	template<int Index> void lerpMul(float t) {}
	template<int Index> void lerpPlus(const self_t& v2) {}
	template<int Index> void lerpSub(const self_t& v2) {}

	typedef T0 argument_type0;
	static const uint16_t behavior0 = B0;
	argument_type0 value0;
	template<> inline void perspectMul<0>(float t) {
		if (behavior0 & kTBPerspect) value0 = value0 * t;
	}
	template<> inline void lerpMul<0>(float t) {
		if (behavior0 & kTBLerp) value0 = value0 * t;\
	}
	template<> inline void lerpPlus<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 + v2.value0;
	}
	template<> inline void lerpSub<0>(const self_t& v2) {
		if (behavior0 & kTBLerp) value0 = value0 - v2.value0;
	}

	typedef T1 argument_type1;
	static const uint16_t behavior1 = B1;
	argument_type1 value1;
	template<> inline void perspectMul<1>(float t) {
		if (behavior1 & kTBPerspect) value1 = value1 * t;
	}
	template<> inline void lerpMul<1>(float t) {
		if (behavior1 & kTBLerp) value1 = value1 * t;\
	}
	template<> inline void lerpPlus<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 + v2.value1;
	}
	template<> inline void lerpSub<1>(const self_t& v2) {
		if (behavior1 & kTBLerp) value1 = value1 - v2.value1;
	}

	typedef T2 argument_type2;
	static const uint16_t behavior2 = B2;
	argument_type2 value2;
	template<> inline void perspectMul<2>(float t) {
		if (behavior2 & kTBPerspect) value2 = value2 * t;
	}
	template<> inline void lerpMul<2>(float t) {
		if (behavior2 & kTBLerp) value2 = value2 * t;\
	}
	template<> inline void lerpPlus<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 + v2.value2;
	}
	template<> inline void lerpSub<2>(const self_t& v2) {
		if (behavior2 & kTBLerp) value2 = value2 - v2.value2;
	}

	typedef T3 argument_type3;
	static const uint16_t behavior3 = B3;
	argument_type3 value3;
	template<> inline void perspectMul<3>(float t) {
		if (behavior3 & kTBPerspect) value3 = value3 * t;
	}
	template<> inline void lerpMul<3>(float t) {
		if (behavior3 & kTBLerp) value3 = value3 * t;\
	}
	template<> inline void lerpPlus<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 + v2.value3;
	}
	template<> inline void lerpSub<3>(const self_t& v2) {
		if (behavior3 & kTBLerp) value3 = value3 - v2.value3;
	}

	typedef T4 argument_type4;
	static const uint16_t behavior4 = B4;
	argument_type4 value4;
	template<> inline void perspectMul<4>(float t) {
		if (behavior4 & kTBPerspect) value4 = value4 * t;
	}
	template<> inline void lerpMul<4>(float t) {
		if (behavior4 & kTBLerp) value4 = value4 * t;\
	}
	template<> inline void lerpPlus<4>(const self_t& v2) {
		if (behavior4 & kTBLerp) value4 = value4 + v2.value4;
	}
	template<> inline void lerpSub<4>(const self_t& v2) {
		if (behavior4 & kTBLerp) value4 = value4 - v2.value4;
	}

	typedef T5 argument_type5;
	static const uint16_t behavior5 = B5;
	argument_type5 value5;
	template<> inline void perspectMul<5>(float t) {
		if (behavior5 & kTBPerspect) value5 = value5 * t;
	}
	template<> inline void lerpMul<5>(float t) {
		if (behavior5 & kTBLerp) value5 = value5 * t;\
	}
	template<> inline void lerpPlus<5>(const self_t& v2) {
		if (behavior5 & kTBLerp) value5 = value5 + v2.value5;
	}
	template<> inline void lerpSub<5>(const self_t& v2) {
		if (behavior5 & kTBLerp) value5 = value5 - v2.value5;
	}

	typedef T6 argument_type6;
	static const uint16_t behavior6 = B6;
	argument_type6 value6;
	template<> inline void perspectMul<6>(float t) {
		if (behavior6 & kTBPerspect) value6 = value6 * t;
	}
	template<> inline void lerpMul<6>(float t) {
		if (behavior6 & kTBLerp) value6 = value6 * t;\
	}
	template<> inline void lerpPlus<6>(const self_t& v2) {
		if (behavior6 & kTBLerp) value6 = value6 + v2.value6;
	}
	template<> inline void lerpSub<6>(const self_t& v2) {
		if (behavior6 & kTBLerp) value6 = value6 - v2.value6;
	}

	typedef T7 argument_type7;
	static const uint16_t behavior7 = B7;
	argument_type7 value7;
	template<> inline void perspectMul<7>(float t) {
		if (behavior7 & kTBPerspect) value7 = value7 * t;
	}
	template<> inline void lerpMul<7>(float t) {
		if (behavior7 & kTBLerp) value7 = value7 * t;\
	}
	template<> inline void lerpPlus<7>(const self_t& v2) {
		if (behavior7 & kTBLerp) value7 = value7 + v2.value7;
	}
	template<> inline void lerpSub<7>(const self_t& v2) {
		if (behavior7 & kTBLerp) value7 = value7 - v2.value7;
	}

	typedef T8 argument_type8;
	static const uint16_t behavior8 = B8;
	argument_type8 value8;
	template<> inline void perspectMul<8>(float t) {
		if (behavior8 & kTBPerspect) value8 = value8 * t;
	}
	template<> inline void lerpMul<8>(float t) {
		if (behavior8 & kTBLerp) value8 = value8 * t;\
	}
	template<> inline void lerpPlus<8>(const self_t& v2) {
		if (behavior8 & kTBLerp) value8 = value8 + v2.value8;
	}
	template<> inline void lerpSub<8>(const self_t& v2) {
		if (behavior8 & kTBLerp) value8 = value8 - v2.value8;
	}

	typedef T9 argument_type9;
	static const uint16_t behavior9 = B9;
	argument_type9 value9;
	template<> inline void perspectMul<9>(float t) {
		if (behavior9 & kTBPerspect) value9 = value9 * t;
	}
	template<> inline void lerpMul<9>(float t) {
		if (behavior9 & kTBLerp) value9 = value9 * t;\
	}
	template<> inline void lerpPlus<9>(const self_t& v2) {
		if (behavior9 & kTBLerp) value9 = value9 + v2.value9;
	}
	template<> inline void lerpSub<9>(const self_t& v2) {
		if (behavior9 & kTBLerp) value9 = value9 - v2.value9;
	}
};


template<class Varying>
struct IteratorCaller {
	template<int Index>
	static void PerspectMul(Varying& v1, float t) {
		v1.perspectMul<Index>(t);
		PerspectMul<Index + 1>(v1, t);
	}

	template<int Index>
	static void LerpMul(Varying& v1, float t) {
		v1.lerpMul<Index>(t);
		LerpMul<Index + 1>(v1, t);
	}

	template<int Index>
	static void LerpPlus(Varying& v1, const Varying& v2) {
		v1.lerpPlus<Index>(v2);
		LerpPlus<Index + 1>(v1, v2);
	}

	template<int Index>
	static void LerpSub(Varying& v1, const Varying& v2) {
		v1.lerpSub<Index>(v2);
		LerpSub<Index + 1>(v1, v2);
	}

	template<> 
	static void PerspectMul<Varying::length>(Varying& v1, float t) {}
	template<> 
	static void LerpMul<Varying::length>(Varying& v1, float t) {}
	template<> 
	static void LerpPlus<Varying::length>(Varying& v1, const Varying& v2) {}
	template<>
	static void LerpSub<Varying::length>(Varying& v1, const Varying& v2) {}
};


template<class Varying>
void PerspectMul(Varying& v1, float t) {
	IteratorCaller<Varying>::PerspectMul<0>(v1, t);
}

template<class Varying>
void LerpMul(Varying& v1, float t) {
	IteratorCaller<Varying>::LerpMul<0>(v1, t);
}

template<class Varying>
void LerpPlus(Varying& v1, const Varying& v2) {
	IteratorCaller<Varying>::LerpPlus<0>(v1, v2);
}

template<class Varying>
void LerpSub(Varying& v1, const Varying& v2) {
	IteratorCaller<Varying>::LerpSub<0>(v1, v2);
}


SHAKURAS_END;