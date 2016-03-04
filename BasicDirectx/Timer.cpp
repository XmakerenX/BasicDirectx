#include "Timer.h"

//-----------------------------------------------------------------------------
// Name : CTimer (constructor)
//-----------------------------------------------------------------------------
CTimer::CTimer(void)
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&m_PerfFreq);
	QueryPerformanceCounter((LARGE_INTEGER *) &m_lastTime); 
	m_TimeScale			= 1.0f / m_PerfFreq;
	m_avgTimeDelta		= 0.0;
	m_SampleCount		= 0;

	m_FrameRate			= 0;
	m_FPSFrameCount		= 0;
	m_FPSTimeElapsed	= 0.0f;

	m_baseTime = 0;
	m_pausedTime = 0;
	m_stopTime = 0;
	m_stopped = false;
}

//-----------------------------------------------------------------------------
// Name : CTimer (destructor)
//-----------------------------------------------------------------------------
CTimer::~CTimer(void)
{
}

//-----------------------------------------------------------------------------
// Name : getDeltaTime ()
//-----------------------------------------------------------------------------
float CTimer::getDeltaTime()
{
	return m_timeDeltas[0];
}

//-----------------------------------------------------------------------------
// Name : getTotalTime ()
//-----------------------------------------------------------------------------
float CTimer::getTotalTime()
{
	// If we are stopped, do not count the time that has passed since we stopped.
	// Moreover, if we previously already had a pause, the distance 
	// mStopTime - mBaseTime includes paused time, which we do not want to count.
	// To correct this, we can subtract the paused time from mStopTime:  
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime
	if (m_stopped)
	{
		return (float)( ((m_stopTime - m_pausedTime) - m_baseTime) * m_TimeScale);
	}

	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count.  To correct this, we can subtract 
	// the paused time from mCurrTime:  
	//
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime
	else
	{
		float curtime = getCurrentTime();
		return (float)( ((curtime - m_pausedTime) - m_baseTime) * m_TimeScale);
	}
}

//-----------------------------------------------------------------------------
// Name : reset ()
//-----------------------------------------------------------------------------
void CTimer::reset()
{
	__int64 curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);

	m_baseTime = curTime;
	m_lastTime = curTime;
	m_stopTime = 0;
	m_stopped = false;
}

//-----------------------------------------------------------------------------
// Name : start ()
//-----------------------------------------------------------------------------
void CTimer::start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime     
	if (m_stopped)
	{
		m_pausedTime += (startTime - m_stopTime);

		m_lastTime = startTime;
		m_stopTime = 0;
		m_stopped = false;
	}

}

//-----------------------------------------------------------------------------
// Name : stop ()
//-----------------------------------------------------------------------------
void CTimer::stop()
{
	if (!m_stopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		m_stopTime = currTime;
		m_stopped = true;
	}
}

//-----------------------------------------------------------------------------
// Name : frameAdvanced
// Desc : signals that a frame has advanced and there is a need to recalculate the time delta and the FPS
//-----------------------------------------------------------------------------
void CTimer::frameAdvanced()
{
	if (m_stopped)
	{
		m_timeDeltas[0] = 0.0;
		m_SampleCount = 0;
		return;
	}

	__int64 currTime;

	QueryPerformanceCounter((LARGE_INTEGER *)&currTime);
	float curTimeDelta = (currTime - m_lastTime) * m_TimeScale;//the current time delta between frames
	m_lastTime = currTime;

	if ( fabsf(curTimeDelta - m_avgTimeDelta) < 1.0f  )
	{
		// Wrap FIFO frame time buffer.
		memmove( &m_timeDeltas[1], m_timeDeltas, (MAX_SAMPLE_COUNT - 1) * sizeof(float) );
		m_timeDeltas[ 0 ] = curTimeDelta;
		if ( m_SampleCount < MAX_SAMPLE_COUNT ) 
			m_SampleCount++;

	} // End if

	// Count up the new average elapsed time
	m_avgTimeDelta = 0.0f;

	for ( ULONG i = 0; i < m_SampleCount; i++ )
		m_avgTimeDelta += m_timeDeltas[ i ];

	if ( m_SampleCount > 0 )
		m_avgTimeDelta /= m_SampleCount;

	// Calculate Frame Rate
	m_FPSFrameCount++;
	m_FPSTimeElapsed += curTimeDelta;

	if ( m_FPSTimeElapsed > 1.0f) 
	{
		m_FrameRate			= m_FPSFrameCount;
		m_FPSFrameCount		= 0;
		m_FPSTimeElapsed	= 0.0f;
	} // End If Second Elapsed

}

//-----------------------------------------------------------------------------
// Name : getTimeElapsed
//-----------------------------------------------------------------------------
float CTimer::getTimeElapsed()
{
	return m_avgTimeDelta;
}

//-----------------------------------------------------------------------------
// Name : getFPS
//-----------------------------------------------------------------------------
unsigned long CTimer::getFPS()
{
	return m_FrameRate;
}

//-----------------------------------------------------------------------------
// Name : getCurrentTime
//-----------------------------------------------------------------------------
float CTimer::getCurrentTime()
{
	__int64 currTime;

	QueryPerformanceCounter((LARGE_INTEGER *)&currTime);

	return currTime * m_TimeScale;
}

//-----------------------------------------------------------------------------
// Name : getLastTime
//-----------------------------------------------------------------------------
float CTimer::getLastTime()
{
	return m_lastTime * m_TimeScale;
}