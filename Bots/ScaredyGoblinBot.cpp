#include "../GameClasses.h"

//** Pre declaration
int	brainProcess(int bx, int by, int sx, int sy, int ey, int ex, int **Mat, int maxx, int maxy) throw();

ScaredyGoblinBot::ScaredyGoblinBot(int xx, int yy) throw() : Bot(xx, yy) {
	SetOwner(OwnerID_ScaredyGoblin);
	SetChar('G');
};

int	ScaredyGoblinBot::BotMove(Maze &ob) throw() {
	int move = brainProcess(GetXCoord(),GetYCoord(),ob.sx,ob.sy,ob.ex,ob.ey,ob.Mat,ob.maxx,ob.maxy),
		bx = GetXCoord(),
		by = GetYCoord();
    
    /*
        Scaredy Goblins have a strong desire to see things blow up.
        So they casually plant armed bombs.
        Tick-tok... Tick-tok...
    */
	if (rand()%20 == 0) {
		ob.AddBot(bx, by, OwnerID_ArmedBomb);
    }

    return move;
};

int	brainProcess (int bx, int by, int sx, int sy, int ey, int ex, int **Mat, int maxx, int maxy) throw() {
    if((bx == sx) || (by > sy)) {
		if (bx > by) { 
            if (bx > 0 && Mat[bx-1][by] != 0) {
			     return 0;
            } else if (by > 0 && Mat[bx][by-1] != 0) {
				return 3;
            } else if (by < maxy-1 && Mat[bx][by+1] != 0) {
				return 1;
            } else if (bx < maxx-1 && Mat[bx+1][by] != 0) {
			    return 2;
            } else {
                return 4;
            }
		} else if (bx < by) {
			if (by > 0 && Mat[bx][by-1] != 0) {
			    return 3;
            } else if (bx > 0 && Mat[bx-1][by] != 0) {
                return 0;
            } else if (by < maxy-1 && Mat[bx][by+1] != 0) {
                return 1;
            } else if (bx < maxx-1 && Mat[bx+1][by] != 0) {
				return 2;
            } else { 
                return 4; 
            }
		}
	}

    if ((bx > sx) && (by < sy)) {
		if ((bx == by) || (bx > by)) {
            if (bx > 0 && Mat[bx-1][by] != 0) {
			    return 0;
            } else if (by < maxy-1 && Mat[bx][by+1] != 0) {
				return 1;
            } else if (by > 0 && Mat[bx][by-1] != 0) {
                return 3;
            } else if (bx < maxx-1 && Mat[bx+1][by] != 0) {
                return 2;
            } else {
                return 4;
            }
		} else if (bx < by) {
			if (by < maxy-1 && Mat[bx][by+1] != 0) {
				return 1;
            } else if (bx > 0 && Mat[bx-1][by] != 0) {
			    return 0;
            } else if (bx < maxx-1 && Mat[bx+1][by] != 0) {
                return 2;
            } else if (by > 0 && Mat[bx][by-1] != 0) {
                return 3;
            } else {
                return 4;
            }
		}
	}

    if ((bx < sx) && (by < sy)) {
		if ((bx == by) || (bx > by)) {
			if (Mat[bx][by+1] != 0) {
                return 1;
            } else if (Mat[bx+1][by] != 0) {
                return 2;
            } else if (Mat[bx][by-1] != 0) {
                return 3;
            } else if (Mat[bx-1][by] != 0) {
				return 0;
            } else {
                return 4;
            }
		} else if (bx < by) {
			if (Mat[bx+1][by] != 0) {
			    return 2;
            } else if (Mat[bx][by+1] != 0) {
                return 1;
            } else if (Mat[bx][by-1] != 0) {
			    return 3;
            } else if (Mat[bx-1][by] != 0) {
			    return 0;
            } else {
                return 4;
            }
		}
	}

    if ((bx < sx) && (by > sy)) {
		if ((bx == by) || (bx > by)) {
			if (Mat[bx][by-1] != 0) {
			    return 3;
            } else if (Mat[bx+1][by] != 0) {
			    return 2;
            } else if (Mat[bx-1][by] != 0) {
			    return 0;
            } else if (Mat[bx][by+1] != 0) {
			    return 1;
            } else {
                return 4;
            }
		} else if (bx < by) {
            if (Mat[bx+1][by] != 0) {
			    return 2;
            } else if (Mat[bx][by-1] != 0) {
			    return 3;
            } else if (Mat[bx][by+1] != 0) {
			 return 1;
            } else if (Mat[bx-1][by] != 0) {
			    return 0;
            } else {
                return 4;
            }
		}
	}

    return 4;
};