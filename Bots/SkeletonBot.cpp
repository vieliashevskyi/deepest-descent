#include "../GameClasses.h"

SkeletonBot::SkeletonBot(int xx, int yy) throw() : Bot(xx, yy) {
	SetOwner(OwnerID_Skeleton);
	SetChar('S');
};

inline int SkeletonBot::BotMove(Maze &ob) throw() {
    /*
        Skeleton bot super power is to drop a Bomb item
    */
	if (rand()%100 == 0) {
        ob.SetTile(GetXCoord(), GetYCoord(), TileID_BombItem);
    }

    /*
        This bot simply moves at random all the time.
        Skeletons don't have brains to think, right? RIGHT?
    */
	return rand()%4;
};