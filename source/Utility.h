#pragma once
#include <memory>


#define SHAKURAS_BEGIN namespace shakuras {
#define SHAKURAS_END }


SHAKURAS_BEGIN;


#define SHAKURAS_SHARED_PTR(C) typedef std::shared_ptr<C> C##Ptr


SHAKURAS_END;