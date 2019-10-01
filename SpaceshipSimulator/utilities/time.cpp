#include "time.h"

Time::TimeVec Time::timers;
const float Time::deltaTime = 1.0f / static_cast<float>(Time::logicRefreshRate);
const float Time::timeDivider = 1000000.0f;

Time::Time()
	: lastTimeMeasurment(0.0f)
	, timer(std::make_unique<TimerOptions<NoTimer> >())
	, timeResidue(0.0f)
	, timerCounter(0.0f)
{
	timers.push_back(this);
}

Time::~Time()
{
	for (int i = 0 ;i<timers.size();++i)
	{
		if (timers[i] == this)
			timers.erase(timers.begin() + i);
	}
}

void Time::starMeasurment()
{
	if(!timer->isAvailable())
		timePoint = std::chrono::system_clock::now();
}

void Time::stopMeasurment()
{
	if (!timer->isAvailable())
	{
		lastTimeMeasurment = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timePoint).count();
		lastTimeMeasurment /= timeDivider;

	}
}

float Time::getLastMeasurment()
{
	return lastTimeMeasurment;
}

int Time::getTimerCount()const
{
	return timerCounter;
}

void Time::setTimer(std::shared_ptr<BasicTimerOptions> options, const std::function<void(int)>& callback)
{
	timer = options;
	this->callback = callback;

	timerCounter = 0;
	timeResidue = 0.0f;

	timePoint = std::chrono::system_clock::now();
}

void Time::refresh()
{
	if (timer->isAvailable())
	{
		lastTimeMeasurment = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - timePoint).count();
		lastTimeMeasurment /= timeDivider;

		if (lastTimeMeasurment > timer->getTimeStep())
		{
			int lastTimerOverflowCount = static_cast<int>(lastTimeMeasurment / timer->getTimeStep());
			timeResidue += lastTimeMeasurment - static_cast<float>(lastTimerOverflowCount * timer->getTimeStep());
			if (timeResidue > 1.0f)
			{
				lastTimerOverflowCount += 1;
				timeResidue -= 1.0f;
			}

			timerCounter += lastTimerOverflowCount;

			if (timer->getType() == BasicTimerOptions::single_shot)
			{
				timer = std::make_shared<TimerOptions<NoTimer> >();
			}
			else timePoint = std::chrono::system_clock::now();

			callback(lastTimerOverflowCount);
		}
	}
}

void Time::refreshTimers()
{
	std::for_each(timers.begin(), timers.end(), [](auto timer) {
		timer->refresh();
	});
}
