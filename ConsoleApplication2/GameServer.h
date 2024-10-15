#pragma once
#include "NetworkServer.h"
#include "PacketDefine.h"
#include "Content.h"
#include "Proxy.h"
#include "ProxyCaller.h"

class GameServer : public NetworkServer, public ProxyCaller
{
private:
	Content m_Content;

	

public:

	friend class Proxy;

	GameServer()
	{
		
	}
	~GameServer()
	{
		Stop();
	}

	BYTE Start(const WCHAR* ip, SHORT port, INT workerThreadNum, INT sendThreadNum, BYTE nagleOption, INT maxClientNum)
	{
		BYTE ret =NetworkServer::Start(ip, port, workerThreadNum, sendThreadNum, nagleOption, maxClientNum);
		Proxy::GetInstance()->Set_Server(this);
		

		return ret;
	}

	virtual VOID OnRecv(UINT64 SessionID, CPacket_TLS* packet)
	{
		


		HEADER tmp_header;
		packet->GetData((char*)&tmp_header, sizeof(HEADER));
		Players::Player* player;
		if (!m_Content.SessionToPlayer.at(SessionID, player))
		{
			packet->Clear();
			return;
		}
		
		Time_Validate(player);
	
		switch (tmp_header.byType)
		{
		case 10:
			m_Content.Character_Move_Start_Recv(SessionID, player, packet);
			break;
		case 12:
			m_Content.Character_Move_Stop_Recv(SessionID, player, packet);
			break;
		case 20:
			m_Content.Character_Attack_1_Start_Recv(player, packet);
			break;
		case 22:
			m_Content.Character_Attack_2_Start_Recv(player, packet);
			break;
		case 24:
			m_Content.Character_Attack_3_Start_Recv(player, packet);
			break;
		case 252:
			m_Content.System_ECHO_recv(SessionID, player, packet);
			break;
		default:
			__debugbreak();
			break;
		}

		packet->Clear(); 

	}

	virtual VOID OnClientJoin(UINT64 SessionID)
	{

		

		Players::Player* player = m_Content.New_User(SessionID);
		m_Content.SessionToPlayer.insert(SessionID, player);
	

	}

	//Release후 호출
	virtual VOID OnClientLeave(UINT64 SessionID)
	{
		m_Content.disconnectFromServer(SessionID);
		//m_Content.Delete_Player(SessionID);
	}

	//accept직후, return false, return true;
	//false 시 클라이언트 거부, true 시 접속 허용
	virtual bool OnConnectionRequest(PWCHAR, SHORT)
	{
		return true;
	}

	//패킷 수신 완료 후


	//패킷 송신 완료 후
	virtual VOID OnSend(UINT64)
	{
	}
	//
	//워커스레드 GQCS 바로 하단에서 호출
	virtual VOID OnWorkerThreadBegin(UINT64, DWORD, BYTE)
	{
	
	}
	//
	//워커스레드 1루프 종료 후
	virtual VOID OnWorkerThreadEnd()
	{





	}
	
	virtual VOID OnSendThreadLooP()
	{
		m_Content.Update();
	}


	virtual CPacket* Alloc_Packet(UINT64 SessionID)
	{
		CPacket* packet = NetworkServer::Alloc_Packet(SessionID);

		if (packet == nullptr)
		{
			return nullptr;

		}

	


		return packet;
	
	};

	virtual VOID Make_Header(int type, HEADER& header)
	{
		header.byCode = Packet_Code;
		header.byType = type;

		switch (type)
		{
		case 0:
		{
			header.bySize = header_num_0;
		
			break;
		}

		case 1:
		{
			header.bySize = header_num_1;
			
			break;
		}

		case 2:
		{
			header.bySize = header_num_2;
		
			break;
		}


		case 11:
		{
			header.bySize = header_num_11;
			
			break;
		}


		case 13:
		{
			header.bySize = header_num_13;
			
			break;
		}

		case 21:
		{
			header.bySize = header_num_21;
			
			break;
		}

		case 23:
		{
			header.bySize = header_num_23;
		
			break;
		}

		case 25:
		{
			header.bySize = header_num_25;
			
			break;
		}

		case 30:
		{
			header.bySize = header_num_30;
			
			break;
		}
		case 251:
		{
			header.bySize = header_num_251;
		
			break;
		}
		case 253:
		{
			header.bySize = header_num_253;
			
			break;
		}

		default:
		{
			__debugbreak();
		}
		}

	};

	virtual BYTE SendPacket(UINT64 SessionID, CPacket* packet)
	{
		return NetworkServer::SendPacket(SessionID,packet);
	};

/*
	VOID Make_Header(int type, HEADER& header)
	{
		
		
	
	}
	*/
	
	VOID Time_Validate(Players::Player* player)
	{
		player->timeOut = timeGetTime();
	}


};