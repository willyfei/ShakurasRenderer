#pragma once
#include "Utility.h"
#include <tuple>


SHAKURAS_BEGIN;


#define PARAM_ARG_1 <A>
#define PARAM_ARG_2 <A, B>
#define PARAM_ARG_3 <A, B, C>
#define PARAM_ARG_4 <A, B, C, D>
#define PARAM_ARG_5 <A, B, C, D, E>
#define PARAM_ARG_6 <A, B, C, D, E, F>
#define PARAM_ARG_7 <A, B, C, D, E, F, G>
#define PARAM_ARG_8 <A, B, C, D, E, F, G, H>
#define PARAM_ARG_9 <A, B, C, D, E, F, G, H, I>


#define TEMPLATE_ARG_1 template<class A> 
#define TEMPLATE_ARG_2 template<class A, class B>
#define TEMPLATE_ARG_3 template<class A, class B, class C>
#define TEMPLATE_ARG_4 template<class A, class B, class C, class D>
#define TEMPLATE_ARG_5 template<class A, class B, class C, class D, class E>
#define TEMPLATE_ARG_6 template<class A, class B, class C, class D, class E, class F>
#define TEMPLATE_ARG_7 template<class A, class B, class C, class D, class E, class F, class G>
#define TEMPLATE_ARG_8 template<class A, class B, class C, class D, class E, class F, class G, class H>
#define TEMPLATE_ARG_9 template<class A, class B, class C, class D, class E, class F, class G, class H, class I>


template<class T, int N>
struct TupleOperator {
	template<bool E> static inline void plusb(const T& t1, const T& t2, T& tr) {}
	template<> static inline void plusb<true>(const T& t1, const T& t2, T& tr) {
		std::get<N>(tr) = std::get<N>(t1) + std::get<N>(t2);
	}

	template<bool E> static inline void subb(const T& t1, const T& t2, T& tr) {}
	template<> static inline void subb<true>(const T& t1, const T& t2, T& tr) {
		std::get<N>(tr) = std::get<N>(t1) - std::get<N>(t2);
	}

	template<bool E> static inline void mulb(const T& t1, float t, T& tr) {}
	template<> static inline void mulb<true>(const T& t1, float t, T& tr) {
		std::get<N>(tr) = std::get<N>(t1) * t;
	}

	static inline void plus(const T& t1, const T& t2, T& tr) {
		plusb<N < std::tuple_size<T>::value>(t1, t2, tr);
	}
	static inline void sub(const T& t1, const T& t2, T& tr) {
		subb<N < std::tuple_size<T>::value>(t1, t2, tr);
	}
	static inline void mul(const T& t1, float t, T& tr) {
		mulb<N < std::tuple_size<T>::value>(t1, t, tr);
	}
};


#define TEMPLATE_ARG(N) TEMPLATE_ARG_##N
#define PARAM_ARG(N) PARAM_ARG_##N


#define TUPLE_SUPPORT_INTERP(N)\
	TEMPLATE_ARG(N) inline std::tuple PARAM_ARG(N) operator+(const std::tuple PARAM_ARG(N)& v1, const std::tuple PARAM_ARG(N)& v2) {\
	std::tuple PARAM_ARG(N) res;\
	TupleOperator<std::tuple PARAM_ARG(N), 0>::plus(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 1>::plus(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 2>::plus(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 3>::plus(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 4>::plus(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 5>::plus(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 6>::plus(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 7>::plus(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 8>::plus(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 9>::plus(v1, v2, res);\
	return res;\
	}\
	TEMPLATE_ARG(N) inline std::tuple PARAM_ARG(N) operator-(const std::tuple PARAM_ARG(N)& v1, const std::tuple PARAM_ARG(N)& v2) {\
	std::tuple PARAM_ARG(N) res;\
	TupleOperator<std::tuple PARAM_ARG(N), 0>::sub(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 1>::sub(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 2>::sub(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 3>::sub(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 4>::sub(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 5>::sub(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 6>::sub(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 7>::sub(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 8>::sub(v1, v2, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 9>::sub(v1, v2, res);\
	return res;\
	}\
	TEMPLATE_ARG(N) inline std::tuple PARAM_ARG(N) operator*(const std::tuple PARAM_ARG(N)& v1, float t) {\
	std::tuple PARAM_ARG(N) res;\
	TupleOperator<std::tuple PARAM_ARG(N), 0>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 1>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 2>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 3>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 4>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 5>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 6>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 7>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 8>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 9>::mul(v1, t, res);\
	return res;\
	}\
	TEMPLATE_ARG(N) inline std::tuple PARAM_ARG(N) operator/(const std::tuple PARAM_ARG(N)& v1, float d) {\
	float t = 1.0f / d;\
	std::tuple PARAM_ARG(N) res;\
	TupleOperator<std::tuple PARAM_ARG(N), 0>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 1>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 2>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 3>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 4>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 5>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 6>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 7>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 8>::mul(v1, t, res);\
	TupleOperator<std::tuple PARAM_ARG(N), 9>::mul(v1, t, res);\
	return res;\
	}


TUPLE_SUPPORT_INTERP(1);
TUPLE_SUPPORT_INTERP(2);
TUPLE_SUPPORT_INTERP(3);
TUPLE_SUPPORT_INTERP(4);
TUPLE_SUPPORT_INTERP(5);
TUPLE_SUPPORT_INTERP(6);
TUPLE_SUPPORT_INTERP(7);
TUPLE_SUPPORT_INTERP(8);
TUPLE_SUPPORT_INTERP(9);


#undef TEMPLATE_ARG_1
#undef TEMPLATE_ARG_2
#undef TEMPLATE_ARG_3
#undef TEMPLATE_ARG_4
#undef TEMPLATE_ARG_5
#undef TEMPLATE_ARG_6
#undef TEMPLATE_ARG_7
#undef TEMPLATE_ARG_8
#undef TEMPLATE_ARG_9

#undef PARAM_ARG_1
#undef PARAM_ARG_2
#undef PARAM_ARG_3
#undef PARAM_ARG_4
#undef PARAM_ARG_5
#undef PARAM_ARG_6
#undef PARAM_ARG_7
#undef PARAM_ARG_8
#undef PARAM_ARG_9

#undef TEMPLATE_ARG
#undef PARAM_ARG
#undef TUPLE_SUPPORT_INTERP


SHAKURAS_END;