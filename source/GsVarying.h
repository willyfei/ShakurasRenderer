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


template<class Varying, int Index>
struct GsVaryingType {
	typedef GSVNil type;
};

#define METAFUN_GSVARYING_TYPE(N)\
	template<class Varying>\
	struct GsVaryingType<Varying, N> {\
		typedef typename Varying::argument_type##N type;\
	};

METAFUN_GSVARYING_TYPE(0);
METAFUN_GSVARYING_TYPE(1);
METAFUN_GSVARYING_TYPE(2);
METAFUN_GSVARYING_TYPE(3);
METAFUN_GSVARYING_TYPE(4);
METAFUN_GSVARYING_TYPE(5);
METAFUN_GSVARYING_TYPE(6);
METAFUN_GSVARYING_TYPE(7);
METAFUN_GSVARYING_TYPE(8);
METAFUN_GSVARYING_TYPE(9);

#undef METAFUN_GSVARYING_TYPE


template<class Varying, int Index>
struct GsVaryingBehavior {
	static const uint16_t value = kTBNone;
};

#define METAFUN_GSVARYING_BEHAVIOR(N)\
	template<class Varying>\
	struct GsVaryingBehavior<Varying, N> {\
		static const uint16_t value = Varying::behavior##N;\
	};

METAFUN_GSVARYING_BEHAVIOR(0);
METAFUN_GSVARYING_BEHAVIOR(1);
METAFUN_GSVARYING_BEHAVIOR(2);
METAFUN_GSVARYING_BEHAVIOR(3);
METAFUN_GSVARYING_BEHAVIOR(4);
METAFUN_GSVARYING_BEHAVIOR(5);
METAFUN_GSVARYING_BEHAVIOR(6);
METAFUN_GSVARYING_BEHAVIOR(7);
METAFUN_GSVARYING_BEHAVIOR(8);
METAFUN_GSVARYING_BEHAVIOR(9);

#undef METAFUN_GSVARYING_BEHAVIOR


#define DEFINE_GSVARYING_MEMBERS(N)\
	typedef T##N argument_type##N;\
	static const uint16_t behavior##N = B##N;\
	argument_type##N value##N;


template<typename T0, uint16_t B0>
class GsVarying<T0, B0> {
public:
	DEFINE_GSVARYING_MEMBERS(0);
};


template<typename T0, uint16_t B0
	, typename T1, uint16_t B1>
class GsVarying<T0, B0, T1, B1> {
public:
	DEFINE_GSVARYING_MEMBERS(0);
	DEFINE_GSVARYING_MEMBERS(1);
};


template<typename T0, uint16_t B0
	, typename T1, uint16_t B1
	, typename T2, uint16_t B2>
class GsVarying<T0, B0, T1, B1, T2, B2> {
public:
	DEFINE_GSVARYING_MEMBERS(0);
	DEFINE_GSVARYING_MEMBERS(1);
	DEFINE_GSVARYING_MEMBERS(2);
};

template<typename T0, uint16_t B0
	, typename T1, uint16_t B1
	, typename T2, uint16_t B2
	, typename T3, uint16_t B3>
class GsVarying<T0, B0, T1, B1, T2, B2, T3, B3> {
public:
	DEFINE_GSVARYING_MEMBERS(0);
	DEFINE_GSVARYING_MEMBERS(1);
	DEFINE_GSVARYING_MEMBERS(2);
	DEFINE_GSVARYING_MEMBERS(3);
};

template<typename T0, uint16_t B0
	, typename T1, uint16_t B1
	, typename T2, uint16_t B2
	, typename T3, uint16_t B3
	, typename T4, uint16_t B4>
class GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4> {
public:
	DEFINE_GSVARYING_MEMBERS(0);
	DEFINE_GSVARYING_MEMBERS(1);
	DEFINE_GSVARYING_MEMBERS(2);
	DEFINE_GSVARYING_MEMBERS(3);
	DEFINE_GSVARYING_MEMBERS(4);
};

template<typename T0, uint16_t B0
	, typename T1, uint16_t B1
	, typename T2, uint16_t B2
	, typename T3, uint16_t B3
	, typename T4, uint16_t B4
	, typename T5, uint16_t B5>
class GsVarying<T0, B0, T1, B1, T2, B2, T3, B3, T4, B4, T5, B5> {
public:
	DEFINE_GSVARYING_MEMBERS(0);
	DEFINE_GSVARYING_MEMBERS(1);
	DEFINE_GSVARYING_MEMBERS(2);
	DEFINE_GSVARYING_MEMBERS(3);
	DEFINE_GSVARYING_MEMBERS(4);
	DEFINE_GSVARYING_MEMBERS(5);
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
	DEFINE_GSVARYING_MEMBERS(0);
	DEFINE_GSVARYING_MEMBERS(1);
	DEFINE_GSVARYING_MEMBERS(2);
	DEFINE_GSVARYING_MEMBERS(3);
	DEFINE_GSVARYING_MEMBERS(4);
	DEFINE_GSVARYING_MEMBERS(5);
	DEFINE_GSVARYING_MEMBERS(6);
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
	DEFINE_GSVARYING_MEMBERS(0);
	DEFINE_GSVARYING_MEMBERS(1);
	DEFINE_GSVARYING_MEMBERS(2);
	DEFINE_GSVARYING_MEMBERS(3);
	DEFINE_GSVARYING_MEMBERS(4);
	DEFINE_GSVARYING_MEMBERS(5);
	DEFINE_GSVARYING_MEMBERS(6);
	DEFINE_GSVARYING_MEMBERS(7);
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
	DEFINE_GSVARYING_MEMBERS(0);
	DEFINE_GSVARYING_MEMBERS(1);
	DEFINE_GSVARYING_MEMBERS(2);
	DEFINE_GSVARYING_MEMBERS(3);
	DEFINE_GSVARYING_MEMBERS(4);
	DEFINE_GSVARYING_MEMBERS(5);
	DEFINE_GSVARYING_MEMBERS(6);
	DEFINE_GSVARYING_MEMBERS(7);
	DEFINE_GSVARYING_MEMBERS(8);
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
	DEFINE_GSVARYING_MEMBERS(0);
	DEFINE_GSVARYING_MEMBERS(1);
	DEFINE_GSVARYING_MEMBERS(2);
	DEFINE_GSVARYING_MEMBERS(3);
	DEFINE_GSVARYING_MEMBERS(4);
	DEFINE_GSVARYING_MEMBERS(5);
	DEFINE_GSVARYING_MEMBERS(6);
	DEFINE_GSVARYING_MEMBERS(7);
	DEFINE_GSVARYING_MEMBERS(8);
	DEFINE_GSVARYING_MEMBERS(9);
};


#undef DEFINE_GSVARYING_MEMBERS


SHAKURAS_END;