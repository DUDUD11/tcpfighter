#include"Content.h"


void Content::Character_Movement_Start_Send(int SessionID, int type, Players::Player* player)
{
    Profiler_Manager::Profile p(L"move start_s");


    int sector_x = player->X / Sector_Size;
    int sector_y = player->Y / Sector_Size;

    sector.Read_Lock();

    for (int i = sector_y - 1; i < sector_y + 2; i++)
    {
        for (int j = sector_x - 1; j < sector_x + 2; j++)
        {
            if (!sector.Vaild_Area_Sector(i, j)) continue;

            for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[i][j].begin(); iter != sector.Sector_Map[i][j].end(); iter++)
            {
                Players::Player* tmp = *iter;
                if (tmp == player) continue;
                if (m_Players.PlayerRemove.find(tmp->sessionID)) continue;


                Proxy::GetInstance()->Character_Movement_Start_Send_Proxy(tmp->sessionID, type, player->ID, player->Direction, player->X, player->Y);

            }

        }

    }
    sector.Read_UnLock();
}

void Content::Character_Movement_Stop_Send(int SessionID, int type, Players::Player* player)
{
    Profiler_Manager::Profile p(L"move stop_s");
    int sector_x = player->X / Sector_Size;
    int sector_y = player->Y / Sector_Size;

    sector.Read_Lock();

    for (int i = sector_y - 1; i < sector_y + 2; i++)
    {
        for (int j = sector_x - 1; j < sector_x + 2; j++)
        {
            if (!sector.Vaild_Area_Sector(i, j)) continue;

            for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[i][j].begin(); iter != sector.Sector_Map[i][j].end(); iter++)
            {
                Players::Player* tmp = *iter;
                if (tmp == player) continue;
                if (m_Players.PlayerRemove.find(tmp->sessionID)) continue;
            
                Proxy::GetInstance()->Character_Movement_Stop_Send_Proxy(tmp->sessionID, type, player->ID, player->Direction, player->X, player->Y);

            }

        }

    }

    sector.Read_UnLock();
}

void Content::Character_Echo(int SessionID, int type, Players::Player* player)
{
    Proxy::GetInstance()->Character_Echo_Proxy(SessionID, type, player->timeOut);
}

void Content::Character_Move_Stop_Recv(int SessionID,  Players::Player* player, CPacket_TLS* packet)
{
    Profiler_Manager::Profile p(L"move stop_r");

    if (m_Players.PlayerRemove.find(player->sessionID)) return;

    unsigned char Direction;
    unsigned short X;
    unsigned short Y;

    *packet >> Direction >> X >> Y;

    if (sector.Sector_Change(player->X,player->Y,X,Y))
    {
        Enqueue_Movement_Sector(player, player->X, player->Y, X, Y);
    }

    player->Moving = false;
    player->Direction = Direction;
    player->X = X;
    player->Y = Y;
    if (player->Direction == dfPACKET_MOVE_DIR_LL) player->Left = dfPACKET_MOVE_DIR_LL;
    else player->Left = dfPACKET_MOVE_DIR_RR;

    Character_Movement_Stop_Send(SessionID, dfPACKET_SC_MOVE_STOP, player);

}

void Content::Character_Sync(int SessionID, int type, Players::Player* player)
{
    
    Proxy::GetInstance()->Character_Sync_Proxy(SessionID, type, player->ID, player->X, player->Y);

}

void Content::Character_Move_Start_Recv(int SessionID, Players::Player* player, CPacket_TLS* packet)
{
    Profiler_Manager::Profile p(L"move start_r");

    if (m_Players.PlayerRemove.find(player->sessionID)) return;

    unsigned char Direction;
    unsigned short X;
    unsigned short Y;

    *packet >> Direction >> X >> Y;


    if (!sector.Vaild_Area_Sector(X/Sector_Size,Y/Sector_Size))
       
    {
        disconnect(player);

        return ;
    }
    else if (abs(X - player->X) > dfERROR_RANGE || abs(Y - player->Y) > dfERROR_RANGE)
    {

        Character_Sync(SessionID, dfPACKET_SC_SYNC, player);

        player->Moving = true;
        player->Direction = Direction;
        if (player->Direction == dfPACKET_MOVE_DIR_LL || player->Direction == dfPACKET_MOVE_DIR_LU || player->Direction == dfPACKET_MOVE_DIR_LD) player->Left = dfPACKET_MOVE_DIR_LL;

        else if (player->Direction == dfPACKET_MOVE_DIR_RU || player->Direction == dfPACKET_MOVE_DIR_RR || player->Direction == dfPACKET_MOVE_DIR_RD) player->Left = dfPACKET_MOVE_DIR_RR;

        Character_Movement_Start_Send(SessionID, dfPACKET_SC_MOVE_START, player);

        return ;

    }

    else if (sector.Sector_Change(player->X,player->Y,X,Y))
    {
        Enqueue_Movement_Sector(player, player->X, player->Y, X, Y);
    }

    player->X = X;
    player->Y = Y;
    player->Moving = true;
    player->Direction = Direction;

    if (player->Direction == dfPACKET_MOVE_DIR_LL || player->Direction == dfPACKET_MOVE_DIR_LU || player->Direction == dfPACKET_MOVE_DIR_LD) player->Left = dfPACKET_MOVE_DIR_LL;
    else if (player->Direction == dfPACKET_MOVE_DIR_RU || player->Direction == dfPACKET_MOVE_DIR_RR || player->Direction == dfPACKET_MOVE_DIR_RD) player->Left = dfPACKET_MOVE_DIR_RR;

    Character_Movement_Start_Send(SessionID, dfPACKET_SC_MOVE_START, player);

    return ;

}

void Content::Character_Damage(int type, Players::Player* attack_player, Players::Player* damaged_player)
{
    Profiler_Manager::Profile p(L"damage");

    int sector_x = damaged_player->X / Sector_Size;
    int sector_y = damaged_player->Y / Sector_Size;

    //sector.Read_Lock(); 공격을 했을때 서버에서 판정을 하여 보내주는 것으로 공격시 락을 걸기때문에 여기서는 걸지 않는다.

    for (int i = sector_y - 1; i < sector_y + 2; i++)
    {
        for (int j = sector_x - 1; j < sector_x + 2; j++)
        {
            if (!sector.Vaild_Area_Sector(i, j)) continue;

            for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[i][j].begin(); iter != sector.Sector_Map[i][j].end(); iter++)
            {
                Players::Player* tmp = *iter;   
                if (m_Players.PlayerRemove.find(tmp->sessionID)) continue;

                Proxy::GetInstance()->Character_Damage_Proxy(tmp->sessionID, type , attack_player->ID, damaged_player->ID, damaged_player->HP);

            }

        }

    }

   // sector.Read_UnLock();

}

void Content::Character_Generate_Self(int SessionID, int type, Players::Player* player)
{
    Proxy::GetInstance()->Character_Generate_Self_Proxy(SessionID, type, player->ID, player->Direction, player->X, player->Y, player->HP);
}

void Content::Character_Generate_Other(int SessionID, int type, Players::Player* player)
{
    Profiler_Manager::Profile p(L"gene_other");

    int sector_x = player->X / Sector_Size;
    int sector_y = player->Y / Sector_Size;

    sector.Read_Lock();

    for (int i = sector_y - 1; i < sector_y + 2; i++)
    {
        for (int j = sector_x - 1; j < sector_x + 2; j++)
        {
            if (!sector.Vaild_Area_Sector(i, j)) continue;

            for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[i][j].begin(); iter != sector.Sector_Map[i][j].end(); iter++)
            {
                Players::Player* tmp = *iter;
                if (tmp == player) continue;
                if (m_Players.PlayerRemove.find(tmp->sessionID)) continue;
  
                Proxy::GetInstance()->Character_Generate_Other_Proxy(SessionID, type, tmp->ID, tmp->Left, tmp->X, tmp->Y, tmp->HP);

       

                Proxy::GetInstance()->Character_Generate_Other_Proxy(tmp->sessionID, type, player->ID, player->Left, player->X, player->Y, player->HP);
                


            }

        }

    }

    sector.Read_UnLock();

}

void Content::Character_Delete(int type, Players::Player* player)
{
    Profiler_Manager::Profile p(L"character_dele");
    int sector_x = player->X / Sector_Size;
    int sector_y = player->Y / Sector_Size;

    sector.Read_Lock();

    for (int i = sector_y - 1; i < sector_y + 2; i++)
    {
        for (int j = sector_x - 1; j < sector_x + 2; j++)
        {
            if (!sector.Vaild_Area_Sector(i, j)) continue;

            for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[i][j].begin(); iter != sector.Sector_Map[i][j].end(); iter++)
            {
                Players::Player* tmp = *iter;
                if (tmp == player) continue;
                if (m_Players.PlayerRemove.find(tmp->sessionID)) continue;

                Proxy::GetInstance()->Character_Delete_Proxy(tmp->sessionID, type, player->ID);

            }

        }

    }

    sector.Read_UnLock();

}

void Content::Character_Attack_1_Start_Send(int type, Players::Player* player, unsigned char Left_Dir)
{
    // 이미 방향이랑 hp는 세팅이 되어있어야함
    Profiler_Manager::Profile p(L"at1_s");
    int sector_x = player->X / Sector_Size;
    int sector_y = player->Y / Sector_Size;

    // 죽은 사람의 공격은 무시된다.
    if (m_Players.PlayerRemove.find(player->sessionID))
    {
        return;
    }

    sector.Read_Lock();


    for (int i = sector_y - 1; i < sector_y + 2; i++)
    {
        for (int j = sector_x - 1; j < sector_x + 2; j++)
        {
            if (!sector.Vaild_Area_Sector(i, j)) continue;

            for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[i][j].begin(); iter != sector.Sector_Map[i][j].end(); iter++)
            {
                Players::Player* tmp = *iter;

                // 접속이 끊긴 사람에게 보내지 않는다.
                if (tmp == player || m_Players.PlayerRemove.find(tmp->sessionID)) continue;

                if (Left_Dir == dfPACKET_MOVE_DIR_LL)
                {
                       Proxy::GetInstance()->Character_Attack_1_Start_Send_Proxy(tmp->sessionID, type,player->ID, player->Left, player->X, player->Y);
                 

                    if (player->X - tmp->X <= dfATTACK1_RANGE_X && player->X - tmp->X >= 0 && abs(player->Y - tmp->Y) <= dfATTACK1_RANGE_Y)
                    {
                        tmp->HP = max(0, tmp->HP - dfATTACK1_DAMAGE);
                        Character_Damage(dfPACKET_SC_DAMAGE, player, tmp);

                        if (tmp->HP <= 0)
                        {
                            disconnect(tmp);
                        }
                    }
                }

                else
                {
                   
                    Proxy::GetInstance()->Character_Attack_1_Start_Send_Proxy(tmp->sessionID,type, player->ID, player->Left, player->X, player->Y);

                    if (tmp->X - player->X <= dfATTACK1_RANGE_X && tmp->X - player->X >= 0 && abs(player->Y - tmp->Y) <= dfATTACK1_RANGE_Y)
                    {
                        tmp->HP = max(0, tmp->HP - dfATTACK1_DAMAGE);
                        Character_Damage(dfPACKET_SC_DAMAGE, player, tmp);

                        if (tmp->HP <= 0)
                        {
                            disconnect(tmp);
                        }

                    }
                }
            }
        }
    }

    sector.Read_UnLock();
}

void Content::Character_Attack_1_Start_Recv(Players::Player* player, CPacket_TLS* packet)
{
    Profiler_Manager::Profile p(L"at1_r");
    if (m_Players.PlayerRemove.find(player->sessionID)) return;
    unsigned char Direction;
    unsigned short X;
    unsigned short Y;

    *packet >> Direction >> X >> Y;

    player->Direction = Direction;
    player->Left = Direction;

    Character_Attack_1_Start_Send(dfPACKET_SC_ATTACK1, player, player->Left);

}

void Content::Character_Attack_2_Start_Send(int type, Players::Player* player, unsigned char Left_Dir)
{
    Profiler_Manager::Profile p(L"at2_s");

    // 이미 방향이랑 hp는 세팅이 되어있어야함

    int sector_x = player->X / Sector_Size;
    int sector_y = player->Y / Sector_Size;

    // 죽은 사람의 공격은 무시된다.
    if (m_Players.PlayerRemove.find(player->sessionID))
    {
        return;
    }

    sector.Read_Lock();

    for (int i = sector_y - 1; i < sector_y + 2; i++)
    {
        for (int j = sector_x - 1; j < sector_x + 2; j++)
        {
            if (!sector.Vaild_Area_Sector(i, j)) continue;

            for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[i][j].begin(); iter != sector.Sector_Map[i][j].end(); iter++)
            {
                Players::Player* tmp = *iter;

                // 접속이 끊긴 사람에게 보내지 않는다.
                if (tmp == player || m_Players.PlayerRemove.find(tmp->sessionID)) continue;


                if (Left_Dir == dfPACKET_MOVE_DIR_LL)
                {
                    Proxy::GetInstance()->Character_Attack_2_Start_Send_Proxy(tmp->sessionID, type, player->ID, player->Left, player->X, player->Y);

                    if (player->X - tmp->X <= dfATTACK2_RANGE_X && player->X - tmp->X >= 0 && abs(player->Y - tmp->Y) <= dfATTACK2_RANGE_Y)
                    {
                        tmp->HP = max(0, tmp->HP - dfATTACK2_DAMAGE);
                        Character_Damage(dfPACKET_SC_DAMAGE, player, tmp);

                        if (tmp->HP <= 0)
                        {
                            disconnect(tmp);
                        }
                    }
                }

                else
                {
                   
                    Proxy::GetInstance()->Character_Attack_2_Start_Send_Proxy(tmp->sessionID, type, player->ID, player->Left, player->X, player->Y);

                    if (tmp->X - player->X <= dfATTACK2_RANGE_X && tmp->X - player->X >= 0 && abs(player->Y - tmp->Y) <= dfATTACK2_RANGE_Y)
                    {
                        tmp->HP = max(0, tmp->HP - dfATTACK2_DAMAGE);
                        Character_Damage(dfPACKET_SC_DAMAGE, player, tmp);

                        if (tmp->HP <= 0)
                        {
                            disconnect(tmp);
                        }

                    }
                }
            }
        }
    }

    sector.Read_UnLock();
}

void Content::Character_Attack_2_Start_Recv(Players::Player* player, CPacket_TLS* packet)
{
    Profiler_Manager::Profile p(L"at2_r");
    if (m_Players.PlayerRemove.find(player->sessionID)) return;
    unsigned char Direction;
    unsigned short X;
    unsigned short Y;

    *packet >> Direction >> X >> Y;

    player->Direction = Direction;
    player->Left = Direction;

    Character_Attack_2_Start_Send(dfPACKET_SC_ATTACK2, player, player->Left);

}

void Content::Character_Attack_3_Start_Send(int type, Players::Player* player, unsigned char Left_Dir)
{
    // 이미 방향이랑 hp는 세팅이 되어있어야함
    Profiler_Manager::Profile p(L"at3_s");
    int sector_x = player->X / Sector_Size;
    int sector_y = player->Y / Sector_Size;

    // 죽은 사람의 공격은 무시된다.
    if (m_Players.PlayerRemove.find(player->sessionID))
    {
        return;
    }

    sector.Read_Lock();

    for (int i = sector_y - 1; i < sector_y + 2; i++)
    {
        for (int j = sector_x - 1; j < sector_x + 2; j++)
        {
            if (!sector.Vaild_Area_Sector(i, j)) continue;

            for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[i][j].begin(); iter != sector.Sector_Map[i][j].end(); iter++)
            {
                Players::Player* tmp = *iter;

                // 접속이 끊긴 사람에게 보내지 않는다.
                if (tmp == player || m_Players.PlayerRemove.find(tmp->sessionID)) continue;


                if (Left_Dir == dfPACKET_MOVE_DIR_LL)
                {
                 
                    Proxy::GetInstance()->Character_Attack_3_Start_Send_Proxy(tmp->sessionID, type, player->ID, player->Left, player->X, player->Y);

                    if (player->X - tmp->X <= dfATTACK3_RANGE_X && player->X - tmp->X >= 0 && abs(player->Y - tmp->Y) <= dfATTACK3_RANGE_Y)
                    {
                        tmp->HP = max(0, tmp->HP - dfATTACK3_DAMAGE);
                        Character_Damage(dfPACKET_SC_DAMAGE, player, tmp);

                        if (tmp->HP <= 0)
                        {
                            disconnect(tmp);
                        }
                    }
                }

                else
                {
                 
                    Proxy::GetInstance()->Character_Attack_3_Start_Send_Proxy(tmp->sessionID, type, player->ID, player->Left, player->X, player->Y);

                    if (tmp->X - player->X <= dfATTACK3_RANGE_X && tmp->X - player->X >= 0 && abs(player->Y - tmp->Y) <= dfATTACK3_RANGE_Y)
                    {
                        tmp->HP = max(0, tmp->HP - dfATTACK3_DAMAGE);
                        Character_Damage(dfPACKET_SC_DAMAGE, player, tmp);

                        if (tmp->HP <= 0)
                        {
                            disconnect(tmp);
                        }

                    }
                }
            }
        }
    }

    sector.Read_UnLock();
}

void Content::Character_Attack_3_Start_Recv(Players::Player* player, CPacket_TLS* packet)
{
    Profiler_Manager::Profile p(L"at3_r");

    if (m_Players.PlayerRemove.find(player->sessionID)) return;
    unsigned char Direction;
    unsigned short X;
    unsigned short Y;

    *packet >> Direction >> X >> Y;

    player->Direction = Direction;
    player->Left = Direction;

    Character_Attack_3_Start_Send(dfPACKET_SC_ATTACK3, player, player->Left);
}

void Content::System_ECHO_recv(int SessionID, Players::Player* player, CPacket_TLS* packet)
{

    if (m_Players.PlayerRemove.find(player->sessionID)) return;
    int time;
    *packet >> time;
    Proxy::GetInstance()->Character_Echo_Proxy(SessionID, dfPACKET_SC_ECHO, time);
}

Players::Player* Content::New_User(int SessionID)
{


    Profiler_Manager::Profile p(L"OnClientJoin");
    
    Players::Player* player = new Players::Player();
 
    m_Players.Player_init(player, SessionID);
    m_Players.Player_Insert(player);
    
    {
        sector.Write_Lock();

        sector.Sector_Insert(player->Y / Sector_Size, player->X / Sector_Size, player);

        //  sector.Sector_Map[player->Y / Sector_Size][player->X / Sector_Size].push_front(player);
        sector.Write_UnLock();
    }
    // 아이디 전송 및 타인 ID 정보 전송과 내 ID 남에게 broadcast
    {


       Character_Generate_Self(SessionID,dfPACKET_SC_CREATE_MY_CHARACTER, player);
        Character_Generate_Other(SessionID,dfPACKET_SC_CREATE_OTHER_CHARACTER, player);
    }

        // 움직이는 플레이어 추가
    {
        send_moving_character_toNewUser(player);
    }


    return player;
}



void Content::disconnect(Players::Player* player)
{
 

    m_Players.PlayerRemove.insert(player->sessionID);
    Character_Delete(dfPACKET_SC_DELETE_CHARACTER, player);

}

void Content::disconnectFromServer(UINT64 SessionID)
{
    Profiler_Manager::Profile p(L"disfromser");

    Players::Player* tmp;

    {
        Profiler_Manager::Profile p(L"session at");

        if (!SessionToPlayer.at(SessionID, tmp))
        {
            __debugbreak();

        }
    }

    m_Players.PlayerRemove.insert(tmp->sessionID);

    
    Character_Delete(dfPACKET_SC_DELETE_CHARACTER, tmp);


    {
        sector.Write_Lock();

        // Profile p(L"delete user time", profile_N);

        
       // for (typename Concurrency::concurrent_unordered_set<Players::Player*>::iterator rm_iter = m_Players.PlayerRemove.GetHashSet()->begin(); rm_iter != m_Players.PlayerRemove.GetHashSet()->end();rm_iter++)
       // {
        //    int x = 0;

          

            int s_y = tmp->Y / Sector_Size;
            int s_x = tmp->X / Sector_Size;

            bool flag = false;

            for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[s_y][s_x].begin(); iter != sector.Sector_Map[s_y][s_x].end(); )
            {
                if ((Players::Player*)*iter == tmp)
                {
                    
                    iter = sector.Sector_Map[s_y][s_x].erase(iter);
                    Delete_Player(SessionID);
                    //rm_iter = m_Players.PlayerRemove.erase(rm_iter);
                  
                    delete(tmp);
                    flag = true;
                    sector.Write_UnLock();
                    return;
                
                    break;
                }

                else
                {
                  
                    iter++;
                }
            }
            if (!flag) {

                Profiler_Manager::Profile p(L"Sector SYNC Del");
                bool ff = false;

                for (int ii = 0; ii < dfRANGE_MOVE_BOTTOM/Sector_Size; ii++)
                {
                    for (int jj = 0; jj < dfRANGE_MOVE_RIGHT / Sector_Size; jj++)
                    {
                        for (std::list<Players::Player*>::iterator it = sector.Sector_Map[ii][jj].begin(); it != sector.Sector_Map[ii][jj].end(); it++)
                
                        
                        {
                            if (*it == tmp)
                            {
                              
                               

                                sector.Sector_Map[ii][jj].erase(it);
                                Delete_Player(SessionID);
                               // rm_iter = m_Players.PlayerRemove.erase(rm_iter);
                //                __debugbreak();
                                delete(tmp);

                                ff = true; 
                                
                                break;
                            }

                        }
                       
                        if (ff) break;
                    }
                    if (ff) break;

                }

                if (!ff)
                {
               
                   // __debugbreak();
                }

            }
      
      //  }
        sector.Write_UnLock();
    }





}

void Content::Delete_Player(UINT64 SessionID)
{
    Players::Player* player;

    if (!SessionToPlayer.at(SessionID, player))
    {
        __debugbreak();
    }

    m_Players.Delete_Player(player);
    SessionToPlayer.erase(SessionID);
 
}


Content::Content()
{
    m_frametimer.timer_init(frame_rate, true);
}

Content::~Content()
{
    m_frametimer.End();
}

// 스레드 한곳에서만 돌아간다
void Content::Update()
{

    int time_now = (int)timeGetTime();


    if (!m_frametimer.AlarmState()) return;
    {

      
        Logic();

    }


}

// 스레드 한곳에서만 돌아간다
void Content::Logic()
{


    Profiler_Manager::Profile p(L"logic");
    AcquireSRWLockShared(&m_Players.playerlistlock);

    for (std::list<Players::Player*>::iterator iter = m_Players.PlayerList.begin(); iter != m_Players.PlayerList.end(); iter++)
    {

        Players::Player* tmp = *iter;

        if (m_Players.PlayerRemove.find(tmp->sessionID))
        {
            continue;
        }

        unsigned short old_x = tmp->X;
        unsigned short old_y = tmp->Y;

        if (!tmp->Moving)
        {
            continue;
        }

        switch (tmp->Direction)
        {
        case dfPACKET_MOVE_DIR_LL:
            tmp->X -= dfMOVE_SPEED_XAXIS;
            break;
        case dfPACKET_MOVE_DIR_LU:
            tmp->X -= dfMOVE_SPEED_XAXIS;
            tmp->Y -= dfMOVE_SPEED_YAXIS;
            break;
        case dfPACKET_MOVE_DIR_UU:
            tmp->Y -= dfMOVE_SPEED_YAXIS;
            break;
        case dfPACKET_MOVE_DIR_RU:
            tmp->X += dfMOVE_SPEED_XAXIS;
            tmp->Y -= dfMOVE_SPEED_YAXIS;
            break;
        case dfPACKET_MOVE_DIR_RR:
            tmp->X += dfMOVE_SPEED_XAXIS;
            break;
        case dfPACKET_MOVE_DIR_RD:
            tmp->X += dfMOVE_SPEED_XAXIS;
            tmp->Y += dfMOVE_SPEED_YAXIS;
            break;
        case dfPACKET_MOVE_DIR_DD:
            tmp->Y += dfMOVE_SPEED_YAXIS;
            break;
        case dfPACKET_MOVE_DIR_LD:
            tmp->X -= dfMOVE_SPEED_XAXIS;
            tmp->Y += dfMOVE_SPEED_YAXIS;
            break;
        default:
            __debugbreak();
            break;
        }

        if (tmp->X < dfRANGE_MOVE_LEFT || tmp->X >= dfRANGE_MOVE_RIGHT
            || tmp->Y < dfRANGE_MOVE_TOP || tmp->Y >= dfRANGE_MOVE_BOTTOM)
        {
            tmp->X = old_x;
            tmp->Y = old_y;

            continue;
        }

        if (old_x / Sector_Size != tmp->X / Sector_Size || old_y / Sector_Size != tmp->Y / Sector_Size)
        {
            ReleaseSRWLockShared(&m_Players.playerlistlock);
            Profiler_Manager::Profile p(L"logic enqueue");
            Enqueue_Movement_Sector(tmp, old_x, old_y, tmp->X, tmp->Y);
            AcquireSRWLockShared(&m_Players.playerlistlock);
        }

     
    }
    ReleaseSRWLockShared(&m_Players.playerlistlock);
    
}



void Content::Enqueue_Movement_Sector(Players::Player* player, unsigned short old_x, unsigned short old_y, unsigned short cur_x, unsigned short cur_y)
{

    unsigned short old_x_s = old_x / Sector_Size;
    unsigned short old_y_s = old_y / Sector_Size;
    unsigned short cur_x_s = cur_x / Sector_Size;
    unsigned short cur_y_s = cur_y / Sector_Size;


    {
        Profiler_Manager::Profile p(L"Enqueue_Movement_write");

        if (m_Players.PlayerRemove.find(player->sessionID))
        {
            //sector.Write_UnLock();
            return;
        }



        if (old_x_s == cur_x_s && old_y_s == cur_y_s) return;

        bool flag = true;
        // lock
        sector.Write_Lock();

        for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[old_y / Sector_Size][old_x / Sector_Size].begin(); iter != sector.Sector_Map[old_y / Sector_Size][old_x / Sector_Size].end(); iter++)
        {
            if ((Players::Player*)*iter == player)
            {
                iter = sector.Sector_Map[old_y / Sector_Size][old_x / Sector_Size].erase(iter);
                flag = false;
                break;
            }
        }

        if (!flag) {
            sector.Sector_Insert(cur_y / Sector_Size, cur_x / Sector_Size, player);
        }
        sector.Write_UnLock();

    

        if (flag)
        {
            return;
        }
    }
    sector.Read_Lock();
    {
        Profiler_Manager::Profile p(L"Enqueue_Movement_read");

        for (int i = old_y_s - 1; i < old_y_s + 2; i++)
        {
            for (int j = old_x_s - 1; j < old_x_s + 2; j++)
            {

                if (!sector.Vaild_Area_Sector(i, j) || sector.In_Section(i, j, cur_y_s, cur_x_s)) continue;

                else
                {
                    for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[i][j].begin(); iter != sector.Sector_Map[i][j].end(); iter++)
                    {
                        Players::Player* tmp = *iter;
                        if (m_Players.PlayerRemove.find(player->sessionID)) break;
                        if (tmp == player) continue;
                        if (m_Players.PlayerRemove.find(tmp->sessionID)) continue;
                        {
                            Profiler_Manager::Profile p(L"Enqueue_Movement_proxy");
                            Proxy::GetInstance()->Character_Delete_Proxy(tmp->sessionID, dfPACKET_SC_DELETE_CHARACTER, player->ID);      
                            Proxy::GetInstance()->Character_Delete_Proxy(player->sessionID, dfPACKET_SC_DELETE_CHARACTER, tmp->ID);

                        }
                        
                    }

                }
            }
        }

        for (int i = cur_y_s - 1; i < cur_y_s + 2; i++)
        {
            for (int j = cur_x_s - 1; j < cur_x_s + 2; j++)
            {

                if (!sector.Vaild_Area_Sector(i, j) || sector.In_Section(i, j, old_y_s, old_x_s)) continue;

                else
                {

                    for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[i][j].begin(); iter != sector.Sector_Map[i][j].end(); iter++)
                    {

                        Players::Player* tmp = *iter;
                        if (m_Players.PlayerRemove.find(player->sessionID)) break;
                        if (tmp == player) continue;
                        if (m_Players.PlayerRemove.find(tmp->sessionID)) continue;
                        {
                            Profiler_Manager::Profile p(L"Enqueue_Movement_proxy");


                            Proxy::GetInstance()->Character_Generate_Other_Proxy(player->sessionID, dfPACKET_SC_CREATE_OTHER_CHARACTER, tmp->ID, tmp->Left, tmp->X, tmp->Y, tmp->HP);

                            // 남이 움직이고 있으면 나에게 알린다

                            if (tmp->Moving)
                            {
                                Proxy::GetInstance()->Character_Movement_Start_Send_Proxy(player->sessionID, dfPACKET_SC_MOVE_START, tmp->ID, tmp->Direction, tmp->X, tmp->Y);
                            }

                            // 캐릭터 생성을 남에게 알린다

                            Proxy::GetInstance()->Character_Generate_Other_Proxy(tmp->sessionID, dfPACKET_SC_CREATE_OTHER_CHARACTER, player->ID, player->Left, player->X, player->Y, player->HP);

                            if (player->Moving)
                            {
                                Proxy::GetInstance()->Character_Movement_Start_Send_Proxy(tmp->sessionID, dfPACKET_SC_MOVE_START, player->ID, player->Direction, player->X, player->Y);
                            }
                        }

                    }
                }
            }
        }
    }
    sector.Read_UnLock();
 

}


void Content::send_moving_character_toNewUser(Players::Player* player)
{

    unsigned sector_x = player->X / Sector_Size;
    unsigned sector_y = player->Y / Sector_Size;
    
    sector.Read_Lock();

    for (unsigned short i = sector_y - 1; i < sector_y + 2; i++)
    {
        for (unsigned short j = sector_x - 1; j < sector_x + 2; j++)
        {

            if (!sector.Vaild_Area_Sector(i, j)) continue;

            for (std::list<Players::Player*>::iterator iter = sector.Sector_Map[i][j].begin(); iter != sector.Sector_Map[i][j].end(); iter++)
            {
                Players::Player* tmp = *iter;
                if (m_Players.PlayerRemove.find(tmp->sessionID)) continue;
                // 남이 움직이고 있으면 나에게 알린다

                if (tmp->Moving)
                {
           
                    Proxy::GetInstance()->Character_Movement_Start_Send_Proxy(player->sessionID, dfPACKET_SC_MOVE_START, tmp->ID, tmp->Direction, tmp->X, tmp->Y);
                }

                // player 본인은 움직이고 있지 않다
            }
        }
    }

    sector.Read_UnLock();

}




