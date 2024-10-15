#include "Serial_Buffer.h"

// 버퍼크기는 eBUFFER_DEFAULT으로 10000byte
CPacket_TLS::CPacket_TLS(int thread_num)
{
	m_TlsPool = new MemoryPool_TLS<st_TLS>(thread_num, 1, false);
	m_Buffer_struct = new BufferDiscriptor[thread_num];
//	m_chpBuffer = nullptr;
	m_BufferSize = eBUFFER_DEFAULT;
	//m_inputDataSize = 0;
//	m_readDataSize = 0;

	m_ThreadNum = thread_num;
	mThreadIdx = -1;
}

/*
CPacket_TLS::CPacket_TLS(int thread_num, int iBufferSize)
{
	m_chpBuffer = new char[iBufferSize];
	m_BufferSize = iBufferSize;
	m_inputDataSize = 0;
	m_readDataSize = 0;
}
*/

CPacket_TLS::~CPacket_TLS()
{
	delete m_TlsPool;
	delete[] m_Buffer_struct;
}

// 사용하는 스레드에서 tls로부터 버퍼를 받아오기위한 함수
void CPacket_TLS::Buffer_init()
{
	// 캐스팅되나 확인필요

	short tmp = InterlockedIncrement16(&mThreadIdx);

	// 재활용은 구현되지 않았습니다. threadnum 초과
	if (tmp > m_ThreadNum) __debugbreak();

	m_Buffer_struct[tmp].chpBuffer = (char*)m_TlsPool->Alloc();
	m_Buffer_struct[tmp].inputDataSize = 0;
	m_Buffer_struct[tmp].readDataSize = 0;
	m_Buffer_struct[tmp].ThreadId = GetCurrentThreadId();


};


void CPacket_TLS::Clear(void)
{
	//memset(m_chpBuffer, 0, m_BufferSize);

	int idx = Buffer_Find();

	__s()inputDataSize = 0;
	__s()readDataSize = 0;
}

// 음수 즉, 뒤로갈수는 없다
int CPacket_TLS::MoveWritePos(int iSize)
{
	int idx = Buffer_Find();


	if (iSize < 0) __debugbreak();
	if (iSize + __s()inputDataSize > m_BufferSize)
	{
		__s()inputDataSize = m_BufferSize;
		return m_BufferSize - __s()inputDataSize;
	}

	__s()inputDataSize += iSize;

	return iSize;
}

int CPacket_TLS::MoveReadPos(int iSize)
{

	int idx = Buffer_Find();

	if (iSize < 0) __debugbreak();
	if (iSize + __s()readDataSize > __s()inputDataSize)
	{
		__s()readDataSize = __s()inputDataSize;
		return __s()inputDataSize;
	}

	__s()readDataSize += iSize;

	return iSize;
}

/* 멀티스레드에서는 tls에 반환하고 다시 등록해야함
CPacket_TLS& CPacket_TLS::operator=(CPacket_TLS& clSrcPacket)
{
	m_BufferSize = clSrcPacket.m_BufferSize;
	m_inputDataSize = clSrcPacket.m_inputDataSize;
	m_readDataSize = 0;
	m_chpBuffer = new char[m_BufferSize];
	memcpy(m_chpBuffer, clSrcPacket.m_chpBuffer, m_inputDataSize);

	return *this;
}
*/

CPacket_TLS& CPacket_TLS::operator<<(unsigned char byValue)
{

	int idx = Buffer_Find();
	if (m_Buffer_struct[idx].inputDataSize + sizeof(byValue) > m_BufferSize) __debugbreak();

	//memcpy((char*)(m_chpBuffer + m_inputDataSize), &byValue, sizeof(byValue));

	//m_inputDataSize += sizeof(byValue);

	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = byValue;

	return *this;

}

CPacket_TLS& CPacket_TLS::operator<<(char chValue)
{
	int idx = Buffer_Find();

	
	if (m_Buffer_struct[idx].inputDataSize + sizeof(chValue) > m_BufferSize) __debugbreak();

	//	memcpy((char*)(m_chpBuffer + m_inputDataSize), &chValue, sizeof(chValue));

	//	m_inputDataSize += sizeof(chValue);

	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = chValue;

	return *this;


}

CPacket_TLS& CPacket_TLS::operator<<(short shValue)
{
	int idx = Buffer_Find();

	if (m_Buffer_struct[idx].inputDataSize + sizeof(shValue) > m_BufferSize) __debugbreak();

	//memcpy((char*)(m_chpBuffer + m_inputDataSize), &shValue, sizeof(shValue));

	//m_inputDataSize += sizeof(shValue);

	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = shValue;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = shValue >> 8;

	return *this;

}

CPacket_TLS& CPacket_TLS::operator<<(unsigned short wValue)
{
	int idx = Buffer_Find();

	if (m_Buffer_struct[idx].inputDataSize + sizeof(wValue) > m_BufferSize) __debugbreak();

	//	memcpy((char*)(m_chpBuffer + m_inputDataSize), &wValue, sizeof(wValue));

	//	m_inputDataSize += sizeof(wValue);

	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = wValue;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = wValue >> 8;

	return *this;
}

CPacket_TLS& CPacket_TLS::operator<<(int iValue)
{
	int idx = Buffer_Find();

	if (m_Buffer_struct[idx].inputDataSize + sizeof(iValue) > m_BufferSize) __debugbreak();

	//memcpy((char*)(m_chpBuffer + m_inputDataSize), &iValue, sizeof(iValue));

	//m_inputDataSize += sizeof(iValue);

	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = iValue;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = iValue >> 8;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = iValue >> 16;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = iValue >> 24;

	return *this;
}

CPacket_TLS& CPacket_TLS::operator<<(long lValue)
{
	int idx = Buffer_Find();

	if (m_Buffer_struct[idx].inputDataSize + sizeof(lValue) > m_BufferSize) __debugbreak();

	//memcpy((char*)(m_chpBuffer + m_inputDataSize), &lValue, sizeof(lValue));

	//m_inputDataSize += sizeof(lValue);

	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = lValue;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = lValue >> 8;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = lValue >> 16;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = lValue >> 24;

	return *this;
}

CPacket_TLS& CPacket_TLS::operator<<(float fValue)
{
	int idx = Buffer_Find();

	if (m_Buffer_struct[idx].inputDataSize + sizeof(fValue) > m_BufferSize) __debugbreak();

	memcpy((char*)(m_Buffer_struct[idx].chpBuffer + m_Buffer_struct[idx].inputDataSize), &fValue, sizeof(fValue));

	m_Buffer_struct[idx].inputDataSize += sizeof(fValue);

	return *this;
}

CPacket_TLS& CPacket_TLS::operator<<(__int64 iValue)
{
	int idx = Buffer_Find();

	if (m_Buffer_struct[idx].inputDataSize + sizeof(iValue) > m_BufferSize) __debugbreak();

	//	memcpy((char*)(m_chpBuffer + m_inputDataSize), &iValue, sizeof(iValue));

	//	m_inputDataSize += sizeof(iValue);

	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = iValue;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = iValue >> 8;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = iValue >> 16;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = iValue >> 24;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = iValue >> 32;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = iValue >> 40;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = iValue >> 48;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = iValue >> 56;

	return *this;
}

CPacket_TLS& CPacket_TLS::operator<<(double dValue)
{
	
	int idx = Buffer_Find();
	if (m_Buffer_struct[idx].inputDataSize + sizeof(dValue) > m_BufferSize) __debugbreak();

	memcpy((char*)(m_Buffer_struct[idx].chpBuffer + m_Buffer_struct[idx].inputDataSize), &dValue, sizeof(dValue));

	m_Buffer_struct[idx].inputDataSize += sizeof(dValue);

	return *this;
}

CPacket_TLS& CPacket_TLS::operator<<(D_Packet dynamic_)
{
	int idx = Buffer_Find();
	if (m_Buffer_struct[idx].inputDataSize + dynamic_.len + sizeof(dynamic_.len) > m_BufferSize) __debugbreak();

	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = dynamic_.len;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = dynamic_.len >> 8;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = dynamic_.len >> 16;
	m_Buffer_struct[idx].chpBuffer[m_Buffer_struct[idx].inputDataSize++] = dynamic_.len >> 24;

	memcpy((char*)(m_Buffer_struct[idx].chpBuffer + m_Buffer_struct[idx].inputDataSize), dynamic_.pointer, dynamic_.len);
	m_Buffer_struct[idx].inputDataSize += dynamic_.len;

	return *this;

}

CPacket_TLS& CPacket_TLS::operator>>(BYTE& byValue)
{
	int idx = Buffer_Find();

	if (m_Buffer_struct[idx].readDataSize + sizeof(byValue) > m_Buffer_struct[idx].inputDataSize) __debugbreak();

	//memcpy(&byValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(byValue));

	//m_readDataSize += sizeof(byValue);

	byValue = *reinterpret_cast<BYTE*>(m_Buffer_struct[idx].chpBuffer + m_Buffer_struct[idx].readDataSize);

	m_Buffer_struct[idx].readDataSize++;

	return *this;
}

CPacket_TLS& CPacket_TLS::operator>>(char& chValue)
{
	int idx = __f();

	if (__s()readDataSize + sizeof(chValue) > __s()inputDataSize) __debugbreak();

	//memcpy(&chValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(chValue));

	//m_readDataSize += sizeof(chValue);



	chValue = *reinterpret_cast<char*>(__s()chpBuffer + __s()readDataSize);

	__s()readDataSize++;

	return *this;
}

CPacket_TLS& CPacket_TLS::operator>>(short& shValue)
{
	int idx = __f();

	if (__s()readDataSize + sizeof(shValue) > __s()inputDataSize) __debugbreak();

	//	memcpy(&shValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(shValue));

	//	m_readDataSize += sizeof(shValue);

	shValue = *reinterpret_cast<short*>(__s()chpBuffer + __s()readDataSize);

	__s()readDataSize += 2;

	return *this;
}

CPacket_TLS& CPacket_TLS::operator>>(WORD& wValue)
{

	int idx = __f();

	if (__s()readDataSize + sizeof(wValue) > __s()inputDataSize) __debugbreak();

	//	memcpy(&wValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(wValue));

	//	m_readDataSize += sizeof(wValue);

	wValue = *reinterpret_cast<WORD*>(__s()chpBuffer + __s()readDataSize);

	__s()readDataSize += 2;


	return *this;
}

CPacket_TLS& CPacket_TLS::operator>>(int& iValue)
{
	int idx = __f();

	if (__s()readDataSize + sizeof(iValue) > __s()inputDataSize) __debugbreak();

	//	memcpy(&iValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(iValue));

	//	m_readDataSize += sizeof(iValue);

	iValue = *reinterpret_cast<int*>(__s()chpBuffer + __s()readDataSize);

	__s()readDataSize += 4;

	return *this;
}

CPacket_TLS& CPacket_TLS::operator>>(DWORD& dwValue)
{
	int idx = __f();
	if (__s()readDataSize + sizeof(dwValue) > __s()inputDataSize) __debugbreak();

	//	memcpy(&dwValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(dwValue));

	//	m_readDataSize += sizeof(dwValue);

	dwValue = *reinterpret_cast<DWORD*>(__s()chpBuffer + __s()readDataSize);

	__s()readDataSize += 4;

	return *this;
}

CPacket_TLS& CPacket_TLS::operator>>(float& fValue)
{
	int idx = __f();
	if (__s()readDataSize + sizeof(fValue) > __s()inputDataSize) __debugbreak();

	fValue = *reinterpret_cast<float*>(__s()chpBuffer + __s()readDataSize);

	__s()readDataSize += sizeof(fValue);

	return *this;
}

CPacket_TLS& CPacket_TLS::operator>>(__int64& iValue)
{
	int idx = __f();
	if (__s()readDataSize + sizeof(iValue) > __s()inputDataSize) __debugbreak();

	//	memcpy(&iValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(iValue));

	//	m_readDataSize += sizeof(iValue);

	iValue = *reinterpret_cast<__int64*>(__s()chpBuffer + __s()readDataSize);

	__s()readDataSize += 8;


	return *this;
}

CPacket_TLS& CPacket_TLS::operator>>(double& dValue)
{
	int idx = __f();
	if (__s()readDataSize + sizeof(dValue) > __s()inputDataSize) __debugbreak();

	dValue = *reinterpret_cast<double*>(__s()chpBuffer + __s()readDataSize);

	__s()readDataSize += sizeof(dValue);

	return *this;
}

//memcpy 메모리오류는 proxy 받는쪽에서 책임지자
CPacket_TLS& CPacket_TLS::operator>>(D_Packet& dynamic_)
{
	int idx = __f();
	if (__s()readDataSize + sizeof(dynamic_.len) + dynamic_.len > __s()inputDataSize) __debugbreak();

	int temp_len = *reinterpret_cast<int*>(__s()chpBuffer + __s()readDataSize);

	if (temp_len > dynamic_.len) __debugbreak();

	dynamic_.len = *reinterpret_cast<int*>(__s()chpBuffer + __s()readDataSize);

	__s()readDataSize += sizeof(dynamic_.len);

	memcpy(dynamic_.pointer, (char*)(__s()chpBuffer + __s()readDataSize), dynamic_.len);

	__s()readDataSize += dynamic_.len;

	return *this;
}



int CPacket_TLS::GetData(char* chpDest, int iSize)
{
	int idx = __f();
	if (iSize < 0) __debugbreak();
	int size = __s()inputDataSize - __s()readDataSize < iSize ? __s()inputDataSize - __s()readDataSize : iSize;

	

	memcpy(chpDest, (char*)(__s()chpBuffer + __s()readDataSize), size);

	__s()readDataSize += size;

	

	return size;
}

int CPacket_TLS::PutData(char* chpSrc, int iSrcSize)
{
	int idx = __f();
	if (iSrcSize < 0) __debugbreak();

	

	int size = m_BufferSize - __s()inputDataSize < iSrcSize ? m_BufferSize - __s()inputDataSize : iSrcSize;

	memcpy((char*)(__s()chpBuffer + __s()inputDataSize), chpSrc, size);

	__s()inputDataSize += size;



	return size;
}

CPacket::CPacket()
{
	m_chpBuffer = new char[eBUFFER_DEFAULT];

	m_BufferSize = eBUFFER_DEFAULT;
	m_inputDataSize = 0;
	m_readDataSize = 0;

}

CPacket::CPacket(int iBufferSize)
{
	m_chpBuffer = new char[iBufferSize];
	m_BufferSize = iBufferSize;
	m_inputDataSize = 0;
	m_readDataSize = 0;
}

CPacket::~CPacket()
{
	delete[] m_chpBuffer;
}

void CPacket::Clear(void)
{
	memset(m_chpBuffer, 0, m_BufferSize);
	m_inputDataSize = 0;
	m_readDataSize = 0;
}

// 음수 즉, 뒤로갈수는 없다
int CPacket::MoveWritePos(int iSize)
{
	if (iSize < 0) __debugbreak();
	if (iSize + m_inputDataSize > m_BufferSize)
	{
		m_inputDataSize = m_BufferSize;
		return m_BufferSize - m_inputDataSize;
	}

	m_inputDataSize += iSize;

	return iSize;
}

int CPacket::MoveReadPos(int iSize)
{
	if (iSize < 0) __debugbreak();
	if (iSize + m_readDataSize > m_inputDataSize)
	{
		m_readDataSize = m_inputDataSize;
		return m_inputDataSize;
	}

	m_readDataSize += iSize;

	return iSize;
}

CPacket& CPacket::operator=(CPacket& clSrcPacket)
{
	m_BufferSize = clSrcPacket.m_BufferSize;
	m_inputDataSize = clSrcPacket.m_inputDataSize;
	m_readDataSize = 0;
	m_chpBuffer = new char[m_BufferSize];
	memcpy(m_chpBuffer, clSrcPacket.m_chpBuffer, m_inputDataSize);

	return *this;
}

CPacket& CPacket::operator<<(unsigned char byValue)
{
	if (m_inputDataSize + sizeof(byValue) > m_BufferSize) __debugbreak();

	//memcpy((char*)(m_chpBuffer + m_inputDataSize), &byValue, sizeof(byValue));

	//m_inputDataSize += sizeof(byValue);

	m_chpBuffer[m_inputDataSize++] = byValue;

	return *this;

}

CPacket& CPacket::operator<<(char chValue)
{
	if (m_inputDataSize + sizeof(chValue) > m_BufferSize) __debugbreak();

	//	memcpy((char*)(m_chpBuffer + m_inputDataSize), &chValue, sizeof(chValue));

	//	m_inputDataSize += sizeof(chValue);

	m_chpBuffer[m_inputDataSize++] = chValue;

	return *this;


}

CPacket& CPacket::operator<<(short shValue)
{
	if (m_inputDataSize + sizeof(shValue) > m_BufferSize) __debugbreak();

	//memcpy((char*)(m_chpBuffer + m_inputDataSize), &shValue, sizeof(shValue));

	//m_inputDataSize += sizeof(shValue);

	m_chpBuffer[m_inputDataSize++] = shValue;
	m_chpBuffer[m_inputDataSize++] = shValue >> 8;

	return *this;

}

CPacket& CPacket::operator<<(unsigned short wValue)
{
	if (m_inputDataSize + sizeof(wValue) > m_BufferSize) __debugbreak();

	//	memcpy((char*)(m_chpBuffer + m_inputDataSize), &wValue, sizeof(wValue));

	//	m_inputDataSize += sizeof(wValue);

	m_chpBuffer[m_inputDataSize++] = wValue;
	m_chpBuffer[m_inputDataSize++] = wValue >> 8;

	return *this;
}

CPacket& CPacket::operator<<(int iValue)
{
	if (m_inputDataSize + sizeof(iValue) > m_BufferSize) __debugbreak();

	//memcpy((char*)(m_chpBuffer + m_inputDataSize), &iValue, sizeof(iValue));

	//m_inputDataSize += sizeof(iValue);

	m_chpBuffer[m_inputDataSize++] = iValue;
	m_chpBuffer[m_inputDataSize++] = iValue >> 8;
	m_chpBuffer[m_inputDataSize++] = iValue >> 16;
	m_chpBuffer[m_inputDataSize++] = iValue >> 24;

	return *this;
}

CPacket& CPacket::operator<<(long lValue)
{
	if (m_inputDataSize + sizeof(lValue) > m_BufferSize) __debugbreak();

	//memcpy((char*)(m_chpBuffer + m_inputDataSize), &lValue, sizeof(lValue));

	//m_inputDataSize += sizeof(lValue);

	m_chpBuffer[m_inputDataSize++] = lValue;
	m_chpBuffer[m_inputDataSize++] = lValue >> 8;
	m_chpBuffer[m_inputDataSize++] = lValue >> 16;
	m_chpBuffer[m_inputDataSize++] = lValue >> 24;

	return *this;
}

CPacket& CPacket::operator<<(float fValue)
{
	if (m_inputDataSize + sizeof(fValue) > m_BufferSize) __debugbreak();

	memcpy((char*)(m_chpBuffer + m_inputDataSize), &fValue, sizeof(fValue));

	m_inputDataSize += sizeof(fValue);

	return *this;
}

CPacket& CPacket::operator<<(__int64 iValue)
{
	if (m_inputDataSize + sizeof(iValue) > m_BufferSize) __debugbreak();

	//	memcpy((char*)(m_chpBuffer + m_inputDataSize), &iValue, sizeof(iValue));

	//	m_inputDataSize += sizeof(iValue);

	m_chpBuffer[m_inputDataSize++] = iValue;
	m_chpBuffer[m_inputDataSize++] = iValue >> 8;
	m_chpBuffer[m_inputDataSize++] = iValue >> 16;
	m_chpBuffer[m_inputDataSize++] = iValue >> 24;
	m_chpBuffer[m_inputDataSize++] = iValue >> 32;
	m_chpBuffer[m_inputDataSize++] = iValue >> 40;
	m_chpBuffer[m_inputDataSize++] = iValue >> 48;
	m_chpBuffer[m_inputDataSize++] = iValue >> 56;

	return *this;
}

CPacket& CPacket::operator<<(double dValue)
{
	if (m_inputDataSize + sizeof(dValue) > m_BufferSize) __debugbreak();

	memcpy((char*)(m_chpBuffer + m_inputDataSize), &dValue, sizeof(dValue));

	m_inputDataSize += sizeof(dValue);

	return *this;
}

CPacket& CPacket::operator<<(D_Packet dynamic_)
{

	if (m_inputDataSize + dynamic_.len + sizeof(dynamic_.len) > m_BufferSize) __debugbreak();

	m_chpBuffer[m_inputDataSize++] = dynamic_.len;
	m_chpBuffer[m_inputDataSize++] = dynamic_.len >> 8;
	m_chpBuffer[m_inputDataSize++] = dynamic_.len >> 16;
	m_chpBuffer[m_inputDataSize++] = dynamic_.len >> 24;

	memcpy((char*)(m_chpBuffer + m_inputDataSize), dynamic_.pointer, dynamic_.len);
	m_inputDataSize += dynamic_.len;

	return *this;

}

CPacket& CPacket::operator>>(BYTE& byValue)
{
	if (m_readDataSize + sizeof(byValue) > m_inputDataSize) __debugbreak();

	//memcpy(&byValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(byValue));

	//m_readDataSize += sizeof(byValue);

	byValue = *reinterpret_cast<BYTE*>(m_chpBuffer + m_readDataSize);

	++m_readDataSize;

	return *this;
}

CPacket& CPacket::operator>>(char& chValue)
{
	if (m_readDataSize + sizeof(chValue) > m_inputDataSize) __debugbreak();

	//memcpy(&chValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(chValue));

	//m_readDataSize += sizeof(chValue);



	chValue = *reinterpret_cast<char*>(m_chpBuffer + m_readDataSize);

	++m_readDataSize;

	return *this;
}

CPacket& CPacket::operator>>(short& shValue)
{
	if (m_readDataSize + sizeof(shValue) > m_inputDataSize) __debugbreak();

	//	memcpy(&shValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(shValue));

	//	m_readDataSize += sizeof(shValue);

	shValue = *reinterpret_cast<short*>(m_chpBuffer + m_readDataSize);

	m_readDataSize += 2;

	return *this;
}

CPacket& CPacket::operator>>(WORD& wValue)
{
	if (m_readDataSize + sizeof(wValue) > m_inputDataSize) __debugbreak();

	//	memcpy(&wValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(wValue));

	//	m_readDataSize += sizeof(wValue);

	wValue = *reinterpret_cast<WORD*>(m_chpBuffer + m_readDataSize);

	m_readDataSize += 2;


	return *this;
}

CPacket& CPacket::operator>>(int& iValue)
{
	if (m_readDataSize + sizeof(iValue) > m_inputDataSize) __debugbreak();

	//	memcpy(&iValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(iValue));

	//	m_readDataSize += sizeof(iValue);

	iValue = *reinterpret_cast<int*>(m_chpBuffer + m_readDataSize);

	m_readDataSize += 4;

	return *this;
}

CPacket& CPacket::operator>>(DWORD& dwValue)
{
	if (m_readDataSize + sizeof(dwValue) > m_inputDataSize) __debugbreak();

	//	memcpy(&dwValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(dwValue));

	//	m_readDataSize += sizeof(dwValue);

	dwValue = *reinterpret_cast<DWORD*>(m_chpBuffer + m_readDataSize);

	m_readDataSize += 4;

	return *this;
}

CPacket& CPacket::operator>>(float& fValue)
{
	if (m_readDataSize + sizeof(fValue) > m_inputDataSize) __debugbreak();

	fValue = *reinterpret_cast<float*>(m_chpBuffer + m_readDataSize);

	m_readDataSize += sizeof(fValue);

	return *this;
}

CPacket& CPacket::operator>>(__int64& iValue)
{
	if (m_readDataSize + sizeof(iValue) > m_inputDataSize) __debugbreak();

	//	memcpy(&iValue, (char*)(m_chpBuffer + m_readDataSize), sizeof(iValue));

	//	m_readDataSize += sizeof(iValue);

	iValue = *reinterpret_cast<__int64*>(m_chpBuffer + m_readDataSize);

	m_readDataSize += 8;


	return *this;
}

CPacket& CPacket::operator>>(double& dValue)
{
	if (m_readDataSize + sizeof(dValue) > m_inputDataSize) __debugbreak();

	dValue = *reinterpret_cast<double*>(m_chpBuffer + m_readDataSize);

	m_readDataSize += sizeof(dValue);

	return *this;
}

//memcpy 메모리오류는 proxy 받는쪽에서 책임지자
CPacket& CPacket::operator>>(D_Packet& dynamic_)
{
	if (m_readDataSize + sizeof(dynamic_.len) + dynamic_.len > m_inputDataSize) __debugbreak();

	int temp_len = *reinterpret_cast<int*>(m_chpBuffer + m_readDataSize);

	if (temp_len > dynamic_.len) __debugbreak();

	dynamic_.len = *reinterpret_cast<int*>(m_chpBuffer + m_readDataSize);

	m_readDataSize += sizeof(dynamic_.len);

	memcpy(dynamic_.pointer, (char*)(m_chpBuffer + m_readDataSize), dynamic_.len);

	m_readDataSize += dynamic_.len;

	return *this;
}



int CPacket::GetData(char* chpDest, int iSize)
{
	if (iSize < 0) __debugbreak();
	int size = m_inputDataSize - m_readDataSize < iSize ? m_inputDataSize - m_readDataSize : iSize;

	memcpy(chpDest, (char*)(m_chpBuffer + m_readDataSize), size);

	m_readDataSize += size;

	

	return size;
}

int CPacket::PutData(char* chpSrc, int iSrcSize)
{
	if (iSrcSize < 0) __debugbreak();

	int size = m_BufferSize - m_inputDataSize < iSrcSize ? m_BufferSize - m_inputDataSize : iSrcSize;

	memcpy((char*)(m_chpBuffer + m_inputDataSize), chpSrc, size);

	m_inputDataSize += size;
	
	return size;
}
