#pragma once
#include "PacketDefine.h"
#include <Windows.h>
#include "ConCurrent_HashSet.h"
#include <list>

class Players
{

public:

	struct Player
	{
		UINT64 sessionID;
		int ID;
		unsigned char Direction;
		unsigned char HP;
		unsigned char Left;
		bool Moving;
		unsigned short X;
		unsigned short Y;
		int timeOut;
	};

	std::list<Player*> PlayerList; 
	SRWLOCK playerlistlock;
	Concurrent_Unordered_set<UINT64> PlayerRemove;
	bool playerremove_check[100000] = { 0, };


	Players()
	{
		m_User_ID = 0;
		InitializeSRWLock(&playerlistlock);

		// lock 초기화 필요시
	}

	~Players()
	{
	
	}

	void Player_Insert(Player* player)
	{
		AcquireSRWLockExclusive(&playerlistlock);
		PlayerList.push_front(player);
		ReleaseSRWLockExclusive(&playerlistlock);
	}

	void Player_init(Player*, UINT64 SessionID);

	void Delete_Player(Player*);



private:
	int m_User_ID;
//	SRWLOCK srw_lock;

};