#pragma once
#include <chrono>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>

#include "timeroptions.h"

class Time
{
public:
	typedef std::vector<Time* > TimeVec;
	static const int logicRefreshRate = 60;	//Hz
	static const float deltaTime;			//sec

	Time();
	~Time();

	void starMeasurment();
	void stopMeasurment();
	float getLastMeasurment();	//sec
	int getTimerCount()const;

	void setTimer(std::shared_ptr<BasicTimerOptions> options, const std::function<void(int)>& callback);

	void refresh();

	static void refreshTimers();

private:
	static TimeVec timers;

	std::chrono::system_clock::time_point timePoint;

	float lastTimeMeasurment;
	std::shared_ptr<BasicTimerOptions> timer;
	float timeResidue;
	int timerCounter;
	std::function<void(int)> callback;

	static const float timeDivider;	//time divider for retrieving results in seconds
};