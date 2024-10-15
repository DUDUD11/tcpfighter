#pragma once
#include "Serial_Buffer.h"
#include "PacketDefine.h"

class ProxyCaller
{
public:

	virtual ~ProxyCaller() {};

	virtual CPacket* Alloc_Packet(UINT64) = 0;

	virtual VOID Make_Header(int type, HEADER& header) = 0;
		
	virtual BYTE SendPacket(UINT64, CPacket*) = 0;

};
