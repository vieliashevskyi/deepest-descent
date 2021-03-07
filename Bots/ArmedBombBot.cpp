#include "../GameClasses.h"
#include <iostream>

extern void playBeep();

ArmedBomb::ArmedBomb(int xx, int yy) throw() : Bot(xx, yy) {
    /*
        Yep. It was easier to leverage Bot to make this 'enemy'
    */
	SetOwner(OwnerID_ArmedBomb);
	SetChar('3');
	charge = 4;
}

int ArmedBomb::BotMove(Maze &ob) throw() {
	int bx = GetXCoord(),
        by = GetYCoord();

	if (charge > 0) {
		charge--;
		SetChar(char(charge) + '0');
    } else {
		alive = false;
		ob.KillBotsInCoord(bx, by);

        if (bx == ob.sx && by == ob.sy) {
            ob._life -= 2;
			playBeep();
        }

		if (by > 0) {
			if (ob.sx == bx && ob.sy == by-1) {
                ob._life--;
				playBeep();
            }
            ob.SetTile(bx, by-1, TileID_Passage);
			ob.KillBotsInCoord(bx, by-1);

			if (bx > 0) {
                ob.SetTile(bx-1, by-1, TileID_Passage);
				ob.KillBotsInCoord(bx-1, by-1);
				if (ob.sx == bx-1 && ob.sy == by-1) {
                    ob._life--;
					playBeep();
                }
			}

			if (bx < ob.maxx-1) {
                ob.SetTile(bx+1, by-1, TileID_Passage);
				ob.KillBotsInCoord(bx+1, by-1);
				if (ob.sx+1 == bx && ob.sy == by-1) {
                    ob._life--;
					playBeep();
                }
			}
		}

		if (by < ob.maxy-1) {
		    if (ob.sx == bx && ob.sy == by+1) {
                ob._life--;
				playBeep();
            }
            ob.SetTile(bx, by+1, TileID_Passage);
			ob.KillBotsInCoord(bx, by+1);

			if (bx > 0) {
				if (ob.sx-1 == bx && ob.sy == by+1) {
                    ob._life--;
					playBeep();
                }
                ob.SetTile(bx-1, by+1, TileID_Passage);
				ob.KillBotsInCoord(bx-1, by+1);
			}
			if (bx < ob.maxx-1) {
				if (ob.sx == bx+1 && ob.sy == by+1) {
                    ob._life--;
					playBeep();
                }
                ob.SetTile(bx+1, by+1, TileID_Passage);
				ob.KillBotsInCoord(bx+1, by+1);
            }

		    if (bx > 0) {
                ob.SetTile(bx-1, by, TileID_Passage);
				ob.KillBotsInCoord(bx-1, by);
				if (ob.sx == bx-1 && ob.sy == by) {
                    ob._life--;
					playBeep();
                }
		    }

		    if (bx < ob.maxx-1) {
				if (ob.sx == bx+1 && ob.sy == by) {
                    ob._life--;
					playBeep();
                }
                ob.SetTile(bx+1, by, TileID_Passage);
				ob.KillBotsInCoord(bx+1,by);
		    }
	    }
	}

    return 4;
}