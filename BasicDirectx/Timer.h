#pragma once

#include <Windows.h>
#include <math.h>

const ULONG MAX_SAMPLE_COUNT	= 50;		   // Maximum frame time sample count

class CTimer
{
public:
	CTimer(void);
	virtual ~CTimer(void);

	float			getDeltaTime();
	float			getTotalTime();

	void			reset();
	void			start();
	void			stop();

	void			frameAdvanced  ();
	float			getTimeElapsed ();
	float			getCurrentTime ();
	float			getLastTime	   ();
	unsigned long	getFPS		   ();

private:
	__int64					m_lastTime;
	__int64					m_PerfFreq;
	float					m_TimeScale;

	__int64					m_baseTime;
	__int64					m_pausedTime;
	__int64					m_stopTime;

	bool					m_stopped;

	float					m_avgTimeDelta;
	float                   m_timeDeltas[MAX_SAMPLE_COUNT];
	ULONG					m_SampleCount;

	unsigned long			m_FrameRate;       // Stores current framerate
	unsigned long			m_FPSFrameCount;   // Elapsed frames in any given second
	float					m_FPSTimeElapsed;  // How much time has passed during FPS sample

};
