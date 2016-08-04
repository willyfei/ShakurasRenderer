#pragma once
#include <vector>
#include <string>
#include <memory>
#include <array>


#define SHAKURAS_BEGIN namespace shakuras {
#define SHAKURAS_END }


SHAKURAS_BEGIN;


#define SHAKURAS_SHARED_PTR(C) typedef std::shared_ptr<C> C##Ptr


template<class T>
void UnstableErase(std::vector<T>& vec, size_t pos, size_t count) {
	size_t bpos = vec.size() - count;
	std::swap_ranges(vec.begin() + pos, vec.begin() + pos + count, vec.begin() + bpos);
	vec.erase(vec.begin() + bpos, vec.end());
}


SHAKURAS_END;