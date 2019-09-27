#pragma once

struct Hz {};
struct Sec {};
struct NoTimer {};

class BasicTimerOptions
{
public:
	enum type { single_shot, continuous };

private:
	float timeStep;
	type timerType;

public:
	
	BasicTimerOptions(float timeStep, type timerType = type::single_shot)
		: timeStep(timeStep)
		, timerType(timerType)
	{}

	float getTimeStep() const
	{
		return timeStep;
	}

	bool getType()const
	{
		return timerType;
	}

	virtual bool isAvailable() = 0;

	//virtual ~BasicTimerOptions() = 0;
};

template <class type>
class TimerOptions;

template<>
struct TimerOptions<Sec>: public BasicTimerOptions
{
	TimerOptions(float time, type timerType = type::single_shot)
		: BasicTimerOptions(time, timerType)
	{}

	bool isAvailable() override
	{
		return true;
	}

	//~TimerOptions() override
	//{}
};

template<>
struct TimerOptions<Hz>: public BasicTimerOptions
{
	TimerOptions(float refreshRate, type timerType = type::single_shot)
		: BasicTimerOptions(1.0f / refreshRate, timerType)
	{}

	bool isAvailable() override
	{
		return true;
	}

	//~TimerOptions() override
	//{}
};

template<>
struct TimerOptions<NoTimer> : public BasicTimerOptions
{
	TimerOptions()
		: BasicTimerOptions(0.0f)
	{}

	bool isAvailable() override
	{
		return false;
	}

	//~TimerOptions() override
	//{}
};

