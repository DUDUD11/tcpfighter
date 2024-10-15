#pragma once
#pragma warning(disable:26110)

#include <Windows.h>
#include <list>
#include "PacketDefine.h"
#include "Players.h"

class Sector
{
public:
	
	//용량이 크니 포인터를 사용할까?
	std::list<Players::Player*> Sector_Map[dfRANGE_MOVE_BOTTOM / Sector_Size][dfRANGE_MOVE_RIGHT / Sector_Size];

	SRWLOCK Sector_lock;

	Sector()
	{
		InitializeSRWLock(&Sector_lock);
	}

	~Sector()
	{
	
	}

	bool In_Section(int, int, int, int);

	bool Vaild_Area_Sector(unsigned short, unsigned short);

	bool Sector_Change(int , int ,int ,int );

	

	void Read_Lock();

	void Read_UnLock();

	void Write_Lock();

	void Write_UnLock();
	
	void Sector_Insert(int y,int x, Players::Player*);
};


