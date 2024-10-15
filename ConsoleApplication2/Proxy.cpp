#include "Proxy.h"

Proxy::~Proxy()
{

}

Proxy::Proxy()
{

}

void Proxy::Character_Movement_Start_Send_Proxy (unsigned __int64   SessionID  , int type, int ID  ,   unsigned char   Direction, unsigned short X, unsigned short Y)
{


	CPacket* pack = Obj->Alloc_Packet(SessionID);
	if (pack == nullptr) return;
	HEADER header;
	Obj->Make_Header(type, header);
	pack->PutData((CHAR*)&header, sizeof(header));
	*pack << ID << Direction << X << Y;
	Obj->SendPacket(SessionID,pack);
}

void Proxy::Character_Movement_Stop_Send_Proxy(unsigned __int64 SessionID, int type, int ID, unsigned char Direction, unsigned short X, unsigned short Y)
{


	CPacket* pack = Obj->Alloc_Packet(SessionID);
	if (pack == nullptr) return;
	HEADER header;
	Obj->Make_Header(type, header);
	pack->PutData((CHAR*)&header, sizeof(header));
	*pack << ID << Direction << X << Y;
	Obj->SendPacket(SessionID,pack);
}

void Proxy::Character_Echo_Proxy(unsigned __int64  SessionID, int type, int time)
{
	/*
	printf("Character_Echo_Proxy %d\n", SessionID);
	CPacket* pack = Obj->Alloc_Packet(SessionID);
	HEADER header;
	Obj->Make_Header(type, header);
	pack->PutData((CHAR*)&header, sizeof(header));
	*pack << time;
	Obj->SendPacket(SessionID,pack);
	*/
}

void Proxy::Character_Sync_Proxy(unsigned __int64  SessionID, int type, int id, unsigned short X, unsigned short Y)
{
	CPacket* pack = Obj->Alloc_Packet(SessionID);
	if (pack == nullptr) return;
	HEADER header;
	Obj->Make_Header(type, header);
	pack->PutData((CHAR*)&header, sizeof(header));
	*pack << id << X << Y;
	Obj->SendPacket(SessionID,pack);
}

void Proxy::Character_Damage_Proxy(unsigned __int64  SessionID, int type, int attacker_id, int damaged_id, unsigned char damaged_hp)
{
	CPacket* pack = Obj->Alloc_Packet(SessionID);
	if (pack == nullptr) return;
	HEADER header;
	Obj->Make_Header(type, header);
	pack->PutData((CHAR*)&header, sizeof(header));
	*pack << attacker_id << damaged_id << damaged_hp;
	Obj->SendPacket(SessionID,pack);
}

void Proxy::Character_Generate_Self_Proxy(unsigned __int64  SessionID, int type, int id, unsigned char Dir, unsigned short X, unsigned short Y, unsigned char hp)
{
		
	CPacket* pack = Obj->Alloc_Packet(SessionID);	
	if (pack == nullptr) return;
	HEADER header;
	Obj->Make_Header(type, header);
	pack->PutData((CHAR*)&header, sizeof(header));
	*pack << id << Dir << X << Y << hp;
	Obj->SendPacket(SessionID,pack);
	
	
}

void Proxy::Character_Generate_Other_Proxy(unsigned __int64  SessionID, int type, int id, unsigned char Left_Dir, unsigned short X, unsigned short Y, unsigned char hp)
{
	
		CPacket* pack = Obj->Alloc_Packet(SessionID);
		if (pack == nullptr) return;

		HEADER header;
		Obj->Make_Header(type, header);
		pack->PutData((CHAR*)&header, sizeof(header));
		*pack << id << Left_Dir << X << Y << hp;
		Obj->SendPacket(SessionID, pack);
	
		
}

void Proxy::Character_Delete_Proxy(unsigned __int64  SessionID, int type, int id)
{
	
	CPacket* pack = Obj->Alloc_Packet(SessionID);
	if (pack == nullptr) return;
	HEADER header;
	Obj->Make_Header(type, header);
	pack->PutData((CHAR*)&header, sizeof(header));
	*pack << id;
	Obj->SendPacket(SessionID,pack);
}

void Proxy::Character_Attack_1_Start_Send_Proxy(unsigned __int64  SessionID, int type, int id, unsigned char Dir, unsigned short X, unsigned short Y)
{

	CPacket* pack = Obj->Alloc_Packet(SessionID);
	if (pack == nullptr) return;
	HEADER header;
	Obj->Make_Header(type, header);
	pack->PutData((CHAR*)&header, sizeof(header));
	*pack << id << Dir << X << Y;
	Obj->SendPacket(SessionID,pack);
}

void Proxy::Character_Attack_2_Start_Send_Proxy(unsigned __int64  SessionID, int type, int id, unsigned char Dir, unsigned short X, unsigned short Y)
{
	
	CPacket* pack = Obj->Alloc_Packet(SessionID);
	if (pack == nullptr) return;
	HEADER header;
	Obj->Make_Header(type, header);
	pack->PutData((CHAR*)&header, sizeof(header));
	*pack << id << Dir << X << Y;
	Obj->SendPacket(SessionID,pack);
}

void Proxy::Character_Attack_3_Start_Send_Proxy(unsigned __int64  SessionID, int type, int id, unsigned char Dir, unsigned short X, unsigned short Y)
{
	
	CPacket* pack = Obj->Alloc_Packet(SessionID);
	if (pack == nullptr) return;
	HEADER header;
	Obj->Make_Header(type, header);
	pack->PutData((CHAR*)&header, sizeof(header));
	*pack << id << Dir << X << Y;
	Obj->SendPacket(SessionID,pack);
}

