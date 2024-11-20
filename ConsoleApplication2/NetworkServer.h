#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include<WinSock2.h>
#include<WS2tcpip.h>
#include <Windows.h>
#include <iostream>
#include <process.h>
#include "Periodtimer.h"
#include "ConCurrent_HashMap.h"
#include "ConCurrent_HashSet.h"
#include "RingBuffer.h"
//#include "CSerializationBufferLan\CSerializationBufferLan.h" // memory pool tls에다가
#include "LockFreeQueue.h" // 구성했음
#include "Serial_Buffer.h" // 그냥 메모리풀
#include "Profiler.h" // memorypool tls에다가 하면될듯
#include "MemoryPoolTLS.h"
#include "PacketDefine.h"


#define WSABUFSIZE 200
#define TimeOut 30000
#define Packet_Length_Limit 1000
#define Packet_Per_Client 200
#define null_socket -1


class NetworkServer
{
public:
	UINT64 mCurrentClientCount; // 현재 클라이언트 유저수

	NetworkServer();

	VOID Init(SHORT port, INT workerThreadNum, INT sendThreadNum, INT maxClientNum);

	BYTE Start(const WCHAR* ip, SHORT port, INT workerThreadNum, INT sendThreadNum, BYTE nagleOption, INT maxClientNum);

	BYTE SendPacket(UINT64, CPacket*);

	BYTE Disconnect(UINT64);

	INT GetClientCount();

	UINT64 GetAcceptCount();

	VOID Stop();

	//Accept후 접속처리 완료 후 호출
	virtual VOID OnClientJoin(UINT64) = 0;

	//Release후 호출
	virtual VOID OnClientLeave(UINT64) = 0;

	//accept직후, return false, return true;
	//false 시 클라이언트 거부, true 시 접속 허용
	virtual bool OnConnectionRequest(PWCHAR, SHORT) = 0;

	//패킷 수신 완료 후
	virtual VOID OnRecv(UINT64, CPacket_TLS*) = 0;

	//패킷 송신 완료 후
	virtual VOID OnSend(UINT64);
	//
	//워커스레드 GQCS 바로 하단에서 호출
	virtual VOID OnWorkerThreadBegin(UINT64, DWORD, BYTE);
	//
	//워커스레드 1루프 종료 후
	virtual VOID OnWorkerThreadEnd();

	//OnSendThreadLooP();
	virtual VOID OnSendThreadLooP();

	UINT64 acceptCount = 0; // accept 하면 증가하네
	UINT64 acceptTPS = 0; // accept 하면 증가하네
	UINT64 recvTPS = 0; // recvproc 증가
	UINT64 sendTPS = 0; // sendpacket 증가

private:

	
	struct st_Packet
	{
		CPacket* Packet;
		int Packet_Size;
	};


	__declspec(align(64))struct stSESSION
	{
		UINT64 sessionID; // 고유 ID
		SOCKET socket; // 소켓
		DWORD timeOut;

		__declspec(align(64))OVERLAPPED recvOverlapped;
		OVERLAPPED sendOverlapped;
		SHORT IOCount; // 세션 재활용을 위해서 필요함 IO 처리를 위해서 필요함
		RingBuffer recvQ; // 좋아요

		CLockFreeQueue<st_Packet> sendQ;
		CMemoryPool<CPacket>* SendPacketPool;
		st_Packet SendBuf[WSABUFSIZE];
		volatile BYTE sendFlag;
		volatile SHORT SendCnt;

	

	};




private:

	SOCKET mListenSocket; // 리슨소켓
	
	UINT64 mClientID; // 하나씩 계속증가함
	INT mMaxClientNum; // 허용된 클라이언트 수
	INT mWorkerThreadNum; // worker 스레드 개수
	INT mSendThreadNum;
	SHORT mSendThreadIdx;
	SHORT mServerPort; // 서버포트
	HANDLE mIocpHandle; // 핸들
	HANDLE* mThreadArr; // 스레드핸들 저장소
	CPacket_TLS* mSerialBuffer; // TLS
	BOOL ThreadExit;



	//SRWLOCK Send_Lock;
	

	CMemoryPool<stSESSION>* mSessionPool;
	//stSESSION* mSessionArray;
	Concurrent_Unordered_set<UINT64>* UserMap;
	Concurrent_Unordered_map<UINT64, stSESSION*> SessionMap;

	static UINT WINAPI _WorkerThread(PVOID p);
	VOID WorkerThread();

	static UINT WINAPI _AcceptThread(PVOID p);
	VOID AcceptThread();

	static UINT WINAPI _SendThread(PVOID p);
	VOID SendThread();

	VOID RecvProc(SHORT, stSESSION*);
	VOID SendProc(stSESSION*);

	VOID RecvPost(stSESSION*);
	VOID RecvPost2(stSESSION*);
	VOID SendPost(stSESSION*);

	stSESSION* FindSession(UINT64);
//	UINT64 MakeSessionID(USHORT, UINT64);

	BYTE ReleaseSession(stSESSION*);

//	BYTE AcquireSession(UINT64, stSESSION*);

	VOID TryLeaveSession(stSESSION*);



protected:
	Profiler_Manager m_Profiler;
/*
#pragma pack(1)
	struct stHEADER
	{
		USHORT length;
	};
#pragma pack(pop)
*/

	CPacket* Alloc_Packet(UINT64);
};

