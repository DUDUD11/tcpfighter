#include "Periodtimer.h"


void CPeriodTimer::timer_init(int Millisecond, bool Start)
{
	tHandle = CreateWaitableTimer(NULL, false, L"PeriodTimer");
	//tHandle = CreateWaitableTimerExW(NULL, L"PeriodTimer", CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
	
	m_MilliSeconds = Millisecond;
	
	if (tHandle == NULL)
	{
		DWORD p = GetLastError();

		__debugbreak();
	}

	if (Start)
	{
		this->Start(Millisecond);
	}

}

CPeriodTimer::CPeriodTimer()
{
	TimerThreadExit = false;
	tHandle = NULL;
	timer_flag = 0;
	m_MilliSeconds = -1;

}

CPeriodTimer::~CPeriodTimer()
{
	CloseHandle(tHandle);



}

unsigned __stdcall CPeriodTimer::TimerThread(LPVOID pArguments)
{
	// 종료조건에서 100 ms 의 여유를 두었다.
	DWORD Period = *(DWORD*)pArguments + 100;

	while (!TimerThreadExit)
	{
		WaitForSingleObject(tHandle, Period);
		InterlockedExchange16(&timer_flag, 1);
	}
	return 0;
}

void CPeriodTimer::Start(int Millisecond)
{
	LARGE_INTEGER startTime;
	startTime.QuadPart = 0;
	SetWaitableTimer(tHandle, &startTime, Millisecond, NULL, NULL, FALSE);
	timerthread = (HANDLE)_beginthreadex(NULL, 0, TimerThread, (void*)this, 0, NULL);

}

void CPeriodTimer::End()
{
	TimerThreadExit = true;
	CancelWaitableTimer(tHandle);

	DWORD retval = WaitForSingleObject(timerthread, 1000);
	if (retval == WAIT_TIMEOUT)
	{
		TerminateThread(timerthread, -1);
	}

}

bool CPeriodTimer::AlarmState()
{
	
	int ret = InterlockedDecrement16(&timer_flag);
	
	return ret == 0;
}
