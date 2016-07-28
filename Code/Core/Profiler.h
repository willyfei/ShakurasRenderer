#pragma once
#include "Utility.h"
#include <chrono>
#include <iostream>


SHAKURAS_BEGIN;


class Profiler {
public:
	Profiler() {
		time_pre_ = std::chrono::steady_clock::now();
		frame_count_ = 0;
		report_span_ = 1000;
	}

public:
	void begin() {
		vert_sharder_excuted_ = 0;
		frag_count_ = 0;
		frag_sharder_excuted_ = 0;
	}

	void end() {
		frame_count_++;

		std::chrono::time_point<std::chrono::steady_clock> time_now = std::chrono::steady_clock::now();
		auto du = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_pre_);

		if (du.count() > report_span_) {
			time_pre_ = time_now;

			//FPS
			float fps = (1.0f * report_span_ * frame_count_) / du.count();
			std::cout << "FPS : " << fps << std::endl;
			frame_count_ = 0;

			//Vert-Sharder
			std::cout << "Vert-Sharder Excuted : " << vert_sharder_excuted_ << std::endl;

			//Frag
			std::cout << "Frag Count : " << frag_count_ << std::endl;

			//Frag-Sharder
			std::cout << "Frag-Sharder Excuted : " << frag_sharder_excuted_ << std::endl;

			//Duration
			std::cout << "Duration In Milliseconds : " << du.count() << std::endl;

			//Endl
			std::cout << std::endl;
		}
	}

public:
	std::chrono::time_point<std::chrono::steady_clock> time_pre_;
	uint32_t report_span_;
	size_t frame_count_;
	size_t vert_sharder_excuted_;
	size_t frag_count_;
	size_t frag_sharder_excuted_;
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