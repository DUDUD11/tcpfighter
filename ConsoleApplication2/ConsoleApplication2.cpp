// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "GameServer.h"

//#pragma comment(lib, "winmm.lib")
HANDLE CPeriodTimer::tHandle = NULL;
short CPeriodTimer::timer_flag = false;
bool CPeriodTimer::TimerThreadExit = false;

LARGE_INTEGER Profiler_Manager::Profile_Freq;
INT64 Profiler_Manager::Profile_total_Counter;
FILE* Profiler_Manager::Profile_fp;
DWORD Profiler_Manager::Profile_dwTlsIndex;
Profiler_Manager::st_TLS* Profiler_Manager::Profile_Tlspool[ThreadNum];
SRWLOCK Profiler_Manager::tlspool_lock;

#define SERVERPORT 20000


int main()
{
	GameServer GameServer;

	timeBeginPeriod(1);
	
	wprintf(L"MainThread Start\n");

	GameServer.Start(L"127.0.0.1", SERVERPORT, 4, 4, true, 10000);

	DWORD startTime, EndTime;
	startTime = timeGetTime();

	Sleep(1000);

	while (1)
	{

		EndTime = timeGetTime();

		float Period = (EndTime - startTime) / 1000;
	
		wprintf(L"==========================\n");
		wprintf(L"Accept User Current = %lld \n", GameServer.mCurrentClientCount);
			
		wprintf(L"AcceptTPS = %f \n", GameServer.acceptTPS / Period);
		wprintf(L"Sec RecvTPS = %f \n", GameServer.recvTPS / Period);
		wprintf(L"Sec SendTPS = %f \n", GameServer.sendTPS / Period);
	
		Sleep(2000);
	}

 

};

