#pragma once
#include "Utility.h"
#include <chrono>
#include <iostream>


SHAKURAS_BEGIN;


class Profiler {
public:
	Profiler() {
		time_fps_ = std::chrono::steady_clock::now();
		frame_count_ = 0;
	}

public:
	void begin() {

	}

	void end() {
		frame_count_++;
		auto du = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - time_fps_);
		if (du.count() > 1000) {
			float fps = (1000.0f * frame_count_) / du.count();
			std::cout << "FPS : " << fps << std::endl;
			frame_count_ = 0;
			time_fps_ = std::chrono::steady_clock::now();
		}
	}

private:
	std::chrono::time_point<std::chrono::steady_clock> time_fps_;
	size_t frame_count_;
};


class ScopeProfiling {
public:
	ScopeProfiling(Profiler& profiler) {
		profiler_ = &profiler;
		profiler_->begin();
	}

	~ScopeProfiling() {
		profiler_->end();
	}

private:
	Profiler* profiler_;
};


SHAKURAS_END;