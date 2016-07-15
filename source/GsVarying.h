#pragma once
#include "Utility.h"


SHAKURAS_BEGIN;


enum GsTraversalBehavior {
	kTBLerp = 1UL << 0,//线性插值
	kTBPerspect = 1UL << 1,//透视
	kTBAll = kTBLerp | kTBPerspect
};


SHAKURAS_END;