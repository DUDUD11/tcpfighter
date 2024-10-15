/*---------------------------------------------------------------

	Packet.

	네트워크 패킷용 클래스.
	간편하게 패킷에 순서대로 데이타를 In, Out 한다.

	- 사용법.

	CPacket_TLS cPacket;  or CMessage Message;

	넣기.
	clPacket << 40030;		or	clPacket << iValue;	(int 넣기)
	clPacket << 1.4;		or	clPacket << fValue;	(float 넣기)


	빼기.
	clPacket >> iValue;		(int 빼기)
	clPacket >> byValue;		(BYTE 빼기)
	clPacket >> fValue;		(float 빼기)

	CPacket_TLS Packet2;

	!.	삽입되는 데이타 FIFO 순서로 관리된다.
		환형 큐는 아니므로, 넣기(<<).빼기(>>) 를 혼합해서 사용하지 않도록 한다



	* 실제 패킷 프로시저에서의 처리

	BOOL	netPacketProc_CreateMyCharacter(CPacket_TLS *clpPacket)
	{
		DWORD dwSessionID;
		short shX, shY;
		char chHP;
		BYTE byDirection;

//		*clpPacket >> dwSessionID >> byDirection >> shX >> shY >> chHP;


		*clpPacket >> dwSessionID;
		*clpPacket >> byDirection;
		*clpPacket >> shX;
		*clpPacket >> shY;
		*clpPacket >> chHP;

		...
		...
	}


	* 실제 메시지(패킷) 생성부에서의 처리

	CPacket_TLS MoveStart;
	mpMoveStart(&MoveStart, dir, x, y);
	SendPacket(&MoveStart);


	void	mpMoveStart(CPacket_TLS *clpPacket, BYTE byDirection, short shX, short shY)
	{
		st_NETWORK_PACKET_HEADER	stPacketHeader;
		stPacketHeader.byCode = dfNETWORK_PACKET_CODE;
		stPacketHeader.bySize = 5;
		stPacketHeader.byType = dfPACKET_CS_MOVE_START;

		clpPacket->PutData((char *)&stPacketHeader, dfNETWORK_PACKET_HEADER_SIZE);

		*clpPacket << byDirection;
		*clpPacket << shX;
		*clpPacket << shY;

	}

----------------------------------------------------------------*/
#pragma once

#define __s() m_Buffer_struct[idx].
#define __f() Buffer_Find()

#include <Windows.h>
#include "MemoryPoolTLS.h"
#ifndef  __PACKET__
#define  __PACKET__

class CPacket_TLS
{

private:

	enum en_PACKET
	{
		// 패킷의 버퍼 사이즈.
		eBUFFER_DEFAULT = 1400,
	};


	struct st_TLS
	{
		char buf[eBUFFER_DEFAULT];
	};

	struct BufferDiscriptor
	{
		DWORD ThreadId;
		char* chpBuffer;
		//------------------------------------------------------------
		// 현재 버퍼에 사용중인 사이즈.
		//------------------------------------------------------------
		int	inputDataSize;
		//------------------------------------------------------------
		// 현재 버퍼에서 읽은 사이즈.
		//------------------------------------------------------------
		int readDataSize;

	};

public:

	struct D_Packet
	{
		int len;
		void* pointer;

	};


	/*---------------------------------------------------------------
	Packet Enum.

	----------------------------------------------------------------*/


	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CPacket_TLS(int Thrade_num);
	//CPacket_TLS(int Thread_num, int iBufferSize);

	//virtual	~CPacket_TLS();
	~CPacket_TLS();

	//////////////////////////////////////////////////////////////////////////
	// 패킷 청소.
	//
	// Parameters: 없음.
	// Return: 없음.
	//////////////////////////////////////////////////////////////////////////
	void	Clear(void);

	//////////////////////////////////////////////////////////////////////////
// 쓰레드 생성시 버퍼 초기화를 위해 사용합니다
//
// Parameters: 없음.
// Return: 없음.
//////////////////////////////////////////////////////////////////////////
	void Buffer_init();

	//////////////////////////////////////////////////////////////////////////
	// 버퍼 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)패킷 버퍼 사이즈 얻기.
	//////////////////////////////////////////////////////////////////////////
	int	GetBufferSize(void) { return m_BufferSize; }
	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)사용중인 데이타 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetDataSize(void) {
		return m_Buffer_struct[Buffer_Find()].inputDataSize;}



	//////////////////////////////////////////////////////////////////////////
	// 버퍼 포인터 얻기.
	//
	// Parameters: 없음.
	// Return: (char *)버퍼 포인터.
	//////////////////////////////////////////////////////////////////////////
	char* GetBufferPtr(void) { return m_Buffer_struct[Buffer_Find()].chpBuffer; }

	//////////////////////////////////////////////////////////////////////////
	// 버퍼 Pos 이동. (음수이동은 안됨)
	// GetBufferPtr 함수를 이용하여 외부에서 강제로 버퍼 내용을 수정할 경우 사용. 
	//
	// Parameters: (int) 이동 사이즈.
	// Return: (int) 이동된 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int iSize);
	int		MoveReadPos(int iSize);

	int Buffer_Find()
	{
	//	printf("%d threadid\n", GetCurrentThreadId());

		for (int i = 0; i <= mThreadIdx;)
		{
		//	printf("%d threadidGet\n", m_Buffer_struct[i].ThreadId);

			if (m_Buffer_struct[i].ThreadId == GetCurrentThreadId()) return i;
			++i;
		}
		__debugbreak();
		return -1;
	};




	/* ============================================================================= */
	// 연산자 오버로딩
	/* ============================================================================= */
	CPacket_TLS& operator = (CPacket_TLS& clSrcPacket);

	//////////////////////////////////////////////////////////////////////////
	// 넣기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	CPacket_TLS& operator << (unsigned char byValue);
	CPacket_TLS& operator << (char chValue);

	CPacket_TLS& operator << (short shValue);
	CPacket_TLS& operator << (unsigned short wValue);

	CPacket_TLS& operator << (int iValue);
	CPacket_TLS& operator << (long lValue);
	CPacket_TLS& operator << (float fValue);

	CPacket_TLS& operator << (__int64 iValue);
	CPacket_TLS& operator << (double dValue);

	CPacket_TLS& operator << (D_Packet dynamic_);

	//////////////////////////////////////////////////////////////////////////
	// 빼기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	CPacket_TLS& operator >> (BYTE& byValue);
	CPacket_TLS& operator >> (char& chValue);

	CPacket_TLS& operator >> (short& shValue);
	CPacket_TLS& operator >> (WORD& wValue);

	CPacket_TLS& operator >> (int& iValue);
	CPacket_TLS& operator >> (DWORD& dwValue);
	CPacket_TLS& operator >> (float& fValue);

	CPacket_TLS& operator >> (__int64& iValue);
	CPacket_TLS& operator >> (double& dValue);

	CPacket_TLS& operator>>(D_Packet& dynamic_);


	//////////////////////////////////////////////////////////////////////////
	// 데이타 얻기.
	//
	// Parameters: (char *)Dest 포인터. (int)Size.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char* chpDest, int iSize);

	//////////////////////////////////////////////////////////////////////////
	// 데이타 삽입.
	//
	// Parameters: (char *)Src 포인터. (int)SrcSize.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(char* chpSrc, int iSrcSize);




protected:



	int	m_BufferSize;
	MemoryPool_TLS<st_TLS>* m_TlsPool;
	BufferDiscriptor* m_Buffer_struct;
	short mThreadIdx;
	int m_ThreadNum;


};


class CPacket
{
public:

	struct D_Packet
	{
		int len;
		void* pointer;

	};

	/*---------------------------------------------------------------
	Packet Enum.

	----------------------------------------------------------------*/
	enum en_PACKET
	{
		eBUFFER_DEFAULT = 1400		// 패킷의 기본 버퍼 사이즈.
	};

	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CPacket();
	CPacket(int iBufferSize);

	//virtual	~CPacket();
	~CPacket();

	//////////////////////////////////////////////////////////////////////////
	// 패킷 청소.
	//
	// Parameters: 없음.
	// Return: 없음.
	//////////////////////////////////////////////////////////////////////////
	void	Clear(void);

	//////////////////////////////////////////////////////////////////////////
	// 버퍼 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)패킷 버퍼 사이즈 얻기.
	//////////////////////////////////////////////////////////////////////////
	int	GetBufferSize(void) { return m_BufferSize; }
	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)사용중인 데이타 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetDataSize(void) { return m_inputDataSize; }

	//////////////////////////////////////////////////////////////////////////
	// 버퍼 포인터 얻기.
	//
	// Parameters: 없음.
	// Return: (char *)버퍼 포인터.
	//////////////////////////////////////////////////////////////////////////
	char* GetBufferPtr(void) { return m_chpBuffer; }

	//////////////////////////////////////////////////////////////////////////
	// 버퍼 Pos 이동. (음수이동은 안됨)
	// GetBufferPtr 함수를 이용하여 외부에서 강제로 버퍼 내용을 수정할 경우 사용. 
	//
	// Parameters: (int) 이동 사이즈.
	// Return: (int) 이동된 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int iSize);
	int		MoveReadPos(int iSize);

	/* ============================================================================= */
	// 연산자 오버로딩
	/* ============================================================================= */
	CPacket& operator = (CPacket& clSrcPacket);

	//////////////////////////////////////////////////////////////////////////
	// 넣기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	CPacket& operator << (unsigned char byValue);
	CPacket& operator << (char chValue);

	CPacket& operator << (short shValue);
	CPacket& operator << (unsigned short wValue);

	CPacket& operator << (int iValue);
	CPacket& operator << (long lValue);
	CPacket& operator << (float fValue);

	CPacket& operator << (__int64 iValue);
	CPacket& operator << (double dValue);

	CPacket& operator << (D_Packet dynamic_);

	//////////////////////////////////////////////////////////////////////////
	// 빼기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	CPacket& operator >> (BYTE& byValue);
	CPacket& operator >> (char& chValue);

	CPacket& operator >> (short& shValue);
	CPacket& operator >> (WORD& wValue);

	CPacket& operator >> (int& iValue);
	CPacket& operator >> (DWORD& dwValue);
	CPacket& operator >> (float& fValue);

	CPacket& operator >> (__int64& iValue);
	CPacket& operator >> (double& dValue);

	CPacket& operator>>(D_Packet& dynamic_);


	//////////////////////////////////////////////////////////////////////////
	// 데이타 얻기.
	//
	// Parameters: (char *)Dest 포인터. (int)Size.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char* chpDest, int iSize);

	//////////////////////////////////////////////////////////////////////////
	// 데이타 삽입.
	//
	// Parameters: (char *)Src 포인터. (int)SrcSize.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(char* chpSrc, int iSrcSize);

protected:

	int	m_BufferSize;
	char* m_chpBuffer;
	//------------------------------------------------------------
	// 현재 버퍼에 사용중인 사이즈.
	//------------------------------------------------------------
	int	m_inputDataSize;
	//------------------------------------------------------------
	// 현재 버퍼에서 읽은 사이즈.
	//------------------------------------------------------------
	int m_readDataSize;

};



#endif
