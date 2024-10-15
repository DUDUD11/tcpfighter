/*---------------------------------------------------------------

	Packet.

	��Ʈ��ũ ��Ŷ�� Ŭ����.
	�����ϰ� ��Ŷ�� ������� ����Ÿ�� In, Out �Ѵ�.

	- ����.

	CPacket_TLS cPacket;  or CMessage Message;

	�ֱ�.
	clPacket << 40030;		or	clPacket << iValue;	(int �ֱ�)
	clPacket << 1.4;		or	clPacket << fValue;	(float �ֱ�)


	����.
	clPacket >> iValue;		(int ����)
	clPacket >> byValue;		(BYTE ����)
	clPacket >> fValue;		(float ����)

	CPacket_TLS Packet2;

	!.	���ԵǴ� ����Ÿ FIFO ������ �����ȴ�.
		ȯ�� ť�� �ƴϹǷ�, �ֱ�(<<).����(>>) �� ȥ���ؼ� ������� �ʵ��� �Ѵ�



	* ���� ��Ŷ ���ν��������� ó��

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


	* ���� �޽���(��Ŷ) �����ο����� ó��

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
		// ��Ŷ�� ���� ������.
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
		// ���� ���ۿ� ������� ������.
		//------------------------------------------------------------
		int	inputDataSize;
		//------------------------------------------------------------
		// ���� ���ۿ��� ���� ������.
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
	// ������, �ı���.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CPacket_TLS(int Thrade_num);
	//CPacket_TLS(int Thread_num, int iBufferSize);

	//virtual	~CPacket_TLS();
	~CPacket_TLS();

	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ û��.
	//
	// Parameters: ����.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void	Clear(void);

	//////////////////////////////////////////////////////////////////////////
// ������ ������ ���� �ʱ�ȭ�� ���� ����մϴ�
//
// Parameters: ����.
// Return: ����.
//////////////////////////////////////////////////////////////////////////
	void Buffer_init();

	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)��Ŷ ���� ������ ���.
	//////////////////////////////////////////////////////////////////////////
	int	GetBufferSize(void) { return m_BufferSize; }
	//////////////////////////////////////////////////////////////////////////
	// ���� ������� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)������� ����Ÿ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetDataSize(void) {
		return m_Buffer_struct[Buffer_Find()].inputDataSize;}



	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (char *)���� ������.
	//////////////////////////////////////////////////////////////////////////
	char* GetBufferPtr(void) { return m_Buffer_struct[Buffer_Find()].chpBuffer; }

	//////////////////////////////////////////////////////////////////////////
	// ���� Pos �̵�. (�����̵��� �ȵ�)
	// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���. 
	//
	// Parameters: (int) �̵� ������.
	// Return: (int) �̵��� ������.
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
	// ������ �����ε�
	/* ============================================================================= */
	CPacket_TLS& operator = (CPacket_TLS& clSrcPacket);

	//////////////////////////////////////////////////////////////////////////
	// �ֱ�.	�� ���� Ÿ�Ը��� ��� ����.
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
	// ����.	�� ���� Ÿ�Ը��� ��� ����.
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
	// ����Ÿ ���.
	//
	// Parameters: (char *)Dest ������. (int)Size.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char* chpDest, int iSize);

	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ����.
	//
	// Parameters: (char *)Src ������. (int)SrcSize.
	// Return: (int)������ ������.
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
		eBUFFER_DEFAULT = 1400		// ��Ŷ�� �⺻ ���� ������.
	};

	//////////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CPacket();
	CPacket(int iBufferSize);

	//virtual	~CPacket();
	~CPacket();

	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ û��.
	//
	// Parameters: ����.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void	Clear(void);

	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)��Ŷ ���� ������ ���.
	//////////////////////////////////////////////////////////////////////////
	int	GetBufferSize(void) { return m_BufferSize; }
	//////////////////////////////////////////////////////////////////////////
	// ���� ������� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)������� ����Ÿ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetDataSize(void) { return m_inputDataSize; }

	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (char *)���� ������.
	//////////////////////////////////////////////////////////////////////////
	char* GetBufferPtr(void) { return m_chpBuffer; }

	//////////////////////////////////////////////////////////////////////////
	// ���� Pos �̵�. (�����̵��� �ȵ�)
	// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���. 
	//
	// Parameters: (int) �̵� ������.
	// Return: (int) �̵��� ������.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int iSize);
	int		MoveReadPos(int iSize);

	/* ============================================================================= */
	// ������ �����ε�
	/* ============================================================================= */
	CPacket& operator = (CPacket& clSrcPacket);

	//////////////////////////////////////////////////////////////////////////
	// �ֱ�.	�� ���� Ÿ�Ը��� ��� ����.
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
	// ����.	�� ���� Ÿ�Ը��� ��� ����.
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
	// ����Ÿ ���.
	//
	// Parameters: (char *)Dest ������. (int)Size.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char* chpDest, int iSize);

	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ����.
	//
	// Parameters: (char *)Src ������. (int)SrcSize.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(char* chpSrc, int iSrcSize);

protected:

	int	m_BufferSize;
	char* m_chpBuffer;
	//------------------------------------------------------------
	// ���� ���ۿ� ������� ������.
	//------------------------------------------------------------
	int	m_inputDataSize;
	//------------------------------------------------------------
	// ���� ���ۿ��� ���� ������.
	//------------------------------------------------------------
	int m_readDataSize;

};



#endif
