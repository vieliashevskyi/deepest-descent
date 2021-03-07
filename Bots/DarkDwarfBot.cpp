#include "../GameClasses.h"

DarkDwarfBot::DarkDwarfBot(int xx, int yy) throw() : Bot(xx, yy) {
	SetOwner(OwnerID_DarkDwarf);
	SetChar('D');
}

int DarkDwarfBot::BotMove(Maze &ob) throw() {
    int xx = GetXCoord(),
		yy = GetYCoord(),
		x1, y1;
    /*
        Dark Dwarfs super power is to change maze structure.
        He builds walls, and destroys passages. 
        He even dislocates level exit if he wishes so.
    */
    if (rand()%50 == 0) {
    	x1 = rand()%ob.maxx;
	    y1 = rand()%ob.maxy;

		while (!ob.IsWay(x1, y1, ob.sx, ob.sy) || ob.GetTile(x1, y1) == TileID_Wall) {
			x1 = rand()%ob.maxx;
			y1 = rand()%ob.maxy;
		}

	    ob.SetExitXCoord(x1);
	    ob.SetExitYCoord(y1);	
    }

	if (xx == ob.GetExitXCoord() && yy == ob.GetExitYCoord()) {
		return rand()%4;
    }

	if (ob.GetTile(xx, yy) == TileID_Passage) {
		ob.SetTile(xx, yy, TileID_Wall);
    } else if (ob.GetTile(xx, yy) == TileID_Wall) {
			ob.SetTile(xx, yy, TileID_Passage);
        } else if (ob.GetTile(xx, yy) == TileID_ScorePoint) {
			ob.SetTile(xx, yy, TileID_Passage);
        } else if (ob.GetTile(xx, yy) == TileID_Life || ob.GetTile(xx, yy) == TileID_MoneyBag) {
			ob.SetTile(xx, yy, TileID_ScorePoint);
        }

		return rand()%4;
};