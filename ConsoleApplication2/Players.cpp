#include "Players.h"

void Players::Player_init(Player* player, UINT64 _SessionID)
{
    player->sessionID = _SessionID;
    player->ID = m_User_ID++;
    player->Direction = dfPACKET_MOVE_DIR_LL;
    player->Left = dfPACKET_MOVE_DIR_LL;
    player->Moving = false;
    player->X = player_init_x * (m_User_ID % dfRANGE_MOVE_RIGHT);
    player->Y = ((player->X % dfRANGE_MOVE_RIGHT) * player_init_y) % (dfRANGE_MOVE_BOTTOM - 100);
    player->X = player->X % (dfRANGE_MOVE_RIGHT - 100);

//   player->X = 100;
//    player->Y = 100;
    player->HP = 100;
    player->timeOut = timeGetTime();

}

void Players:: Delete_Player(Player* player)
{
   
    AcquireSRWLockExclusive(&playerlistlock);
    PlayerList.remove(player);
    ReleaseSRWLockExclusive(&playerlistlock);
    

}
