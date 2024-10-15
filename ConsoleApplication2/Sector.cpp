#include "Sector.h"

bool Sector::In_Section(int a, int b, int c, int d)
{
    return abs(a - c) <= 1 && abs(b - d) <= 1;

}

bool Sector::Vaild_Area_Sector(unsigned short x, unsigned short y)
{
    if (x < 0 || y < 0 || x >= dfRANGE_MOVE_RIGHT / Sector_Size || y >= dfRANGE_MOVE_BOTTOM / Sector_Size)
        return false;
    return true;

}     

bool Sector::Sector_Change(int player_x, int player_y, int packet_x, int packet_y)
{
   

    return player_x / Sector_Size != packet_x / Sector_Size || player_y / Sector_Size != packet_y / Sector_Size;
}

void Sector::Read_Lock()
{
    AcquireSRWLockShared(&Sector_lock);
}

void Sector::Read_UnLock()
{
    ReleaseSRWLockShared(&Sector_lock);
}

void Sector::Write_Lock()
{

    AcquireSRWLockExclusive(&Sector_lock);
}

void Sector::Write_UnLock()
{

    ReleaseSRWLockExclusive(&Sector_lock);
}

void Sector::Sector_Insert(int y, int x, Players::Player* player)
{

    Sector_Map[y][x].push_front(player);

}
