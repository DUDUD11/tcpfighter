#pragma once
#include "ProxyCaller.h"

class Proxy
{

private:
	Proxy();
	~Proxy();

	ProxyCaller* Obj;

public:

	static inline Proxy* GetInstance()
	{
		static Proxy proxy;
		return &proxy;
	}


	void Set_Server(ProxyCaller* server)
	{
		this->Obj = server;
	}


	void Character_Movement_Start_Send_Proxy (unsigned __int64   SessionID  , int type, int ID  ,   unsigned char   Direction, unsigned short X, unsigned short Y);

	void Character_Movement_Stop_Send_Proxy(unsigned __int64 SessionID, int type, int ID, unsigned char Direction, unsigned short X, unsigned short Y);

	void Character_Echo_Proxy(unsigned __int64  SessionID, int type, int time);

	void Character_Sync_Proxy(unsigned __int64  SessionID, int type, int id, unsigned short X, unsigned short Y);

	void Character_Damage_Proxy(unsigned __int64  SessionID, int type, int attacker_id, int damaged_id, unsigned char damaged_hp);

	void Character_Generate_Self_Proxy(unsigned __int64  SessionID, int type, int id, unsigned char Dir, unsigned short X, unsigned short Y, unsigned char hp);

	void Character_Generate_Other_Proxy(unsigned __int64  SessionID, int type, int id, unsigned char Left_Dir, unsigned short X, unsigned short Y, unsigned char hp);

	void Character_Delete_Proxy(unsigned __int64  SessionID, int type, int id);

	void Character_Attack_1_Start_Send_Proxy(unsigned __int64  SessionID, int type, int id, unsigned char Dir, unsigned short X, unsigned short Y);

	void Character_Attack_2_Start_Send_Proxy(unsigned __int64  SessionID, int type, int id, unsigned char Dir, unsigned short X, unsigned short Y);

	void Character_Attack_3_Start_Send_Proxy(unsigned __int64  SessionID, int type, int id, unsigned char Dir, unsigned short X, unsigned short Y);

};