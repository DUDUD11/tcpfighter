#pragma once
#include <Windows.h> 
#include <process.h> 
#include <stdio.h>



class CPeriodTimer
{
public:
	void timer_init(int Millisecond, bool Start);

	CPeriodTimer();
	~CPeriodTimer();

	static unsigned __stdcall TimerThread(LPVOID pArguments);

	void Start(int Millisecond);

	void End();

	bool AlarmState();



private:

	static HANDLE tHandle;
	static short timer_flag;
	static bool TimerThreadExit;
	int m_MilliSeconds;
	HANDLE timerthread;


};


