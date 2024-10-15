#pragma once
#include <WinSock2.h>
#include "Players.h"
#include "PacketDefine.h"
#include "Sector.h"
#include "Serial_Buffer.h"
#include "Periodtimer.h"
#include "profiler.h"
#include "Proxy.h"
#include "ConCurrent_HashMap.h"

class Content {
private:


public:

	Concurrent_Unordered_map<uint64_t, Players::Player*> SessionToPlayer;

	Content();
	~Content();

	void Update();
	void Logic();

	void Enqueue_Movement_Sector( Players::Player*, unsigned short, unsigned short, unsigned short, unsigned short);

	void send_moving_character_toNewUser(Players::Player*);

	void Character_Movement_Start_Send(int SessionID, int type, Players::Player* player);

	void Character_Movement_Stop_Send(int SessionID, int type, Players::Player* player);

	void Character_Echo(int SessionID, int type, Players::Player* player);

	void Character_Sync(int SessionID, int type, Players::Player* player);

	void Character_Move_Stop_Recv(int SessionID, Players::Player* player, CPacket_TLS* packet);

	void Character_Move_Start_Recv(int SessionID, Players::Player* player, CPacket_TLS* packet);

	void Character_Damage(int type, Players::Player* attack_player, Players::Player* damaged_player);

	void Character_Generate_Self(int SessionID, int type, Players::Player* player);

	void Character_Generate_Other(int SessionID, int type, Players::Player* player);

	void Character_Delete(int type, Players::Player* player);

	void Character_Attack_1_Start_Send(int type, Players::Player* player, unsigned char Left_Dir);

	void Character_Attack_1_Start_Recv(Players::Player* player, CPacket_TLS* packet);

	void Character_Attack_2_Start_Send(int type, Players::Player* player, unsigned char Left_Dir);

	void Character_Attack_2_Start_Recv( Players::Player* player, CPacket_TLS* packet);

	void Character_Attack_3_Start_Send(int type, Players::Player* player, unsigned char Left_Dir);

	void Character_Attack_3_Start_Recv(Players::Player* player, CPacket_TLS* packet);

	void System_ECHO_recv(int SessionID, Players::Player* player, CPacket_TLS* packet);

	Players::Player* New_User(int SessionID);

	void disconnect(Players::Player* player);

	void disconnectFromServer(UINT64 SessionID);

	void Delete_Player(UINT64 SessionID);

private:
	Sector sector;
	Players m_Players;
	CPeriodTimer m_frametimer;
};