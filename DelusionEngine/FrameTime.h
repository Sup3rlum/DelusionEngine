#pragma once

#include "nchfx.h"


using namespace std;

typedef chrono::time_point<chrono::steady_clock> TimeSpan;
typedef chrono::duration<double> TimeDuration;

EXPORT_CLASS FrameTime
{
public:


	static void Start();

	static TimeSpan Now();

	static TimeDuration FrameDeltaTime;

	static TimeSpan StartTime;
	static TimeSpan TotalTime;

	static TimeSpan ApplicationRunTime();

	static DELuint64 EllapsedFrames;
};

