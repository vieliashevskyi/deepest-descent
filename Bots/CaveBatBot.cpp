#include "../GameClasses.h"

CaveBatBot::CaveBatBot(int xx, int yy) throw() : Bot(xx, yy) {
	SetOwner(OwnerID_CaveBat);
	SetChar('b');
};

int	CaveBatBot::BotMove(Maze &ob) throw() {
	int bx = GetXCoord(),
		by = GetYCoord(),
		i = rand()%2,
		_move;

    switch(i) {
	    case 0: if (bx < ob.sx) { _move = 1; goto END; }
				if (bx > ob.sx) { _move = 3; goto END; }
                if (bx == ob.sx && 
                    by < ob.sy) { _move = 2; goto END; }
                if (bx == ob.sx &&
                    by > ob.sy) { _move = 0; goto END; }
                if (by < ob.sy) { _move = 2; goto END; }
                if (by > ob.sy) { _move = 0; goto END; }
                if (by == ob.sy &&
                    bx < ob.sx) { _move = 1; goto END; }
                if (by == ob.sy && 
                    bx > ob.sx) { _move = 3; goto END; }
				break;
	    case 1: if (by < ob.sy) { _move = 2; goto END; }
                if (by > ob.sy) { _move = 0; goto END; }
                if (by == ob.sy &&
                    bx < ob.sx) { _move = 1; goto END; }
                if (by == ob.sy &&
                    bx > ob.sx) { _move = 3; goto END; }
                if (bx < ob.sx) { _move = 1; goto END; }
                if (bx > ob.sx) { _move = 3; goto END; }
                if (bx == ob.sx &&
                    by < ob.sy) { _move = 2; goto END; }
                if (bx == ob.sx &&
                    by > ob.sy) { _move = 0; goto END; }
                break;
	    default: _move = 4; 
                 goto END;
	}
    
    _move = 4;
    END: 
    /*
        Cave Bats can magically teleport somewhere close to a Player.
        Very strange bats tbh... must be some old charms.
    */
    if (rand()%15 == 0) {
	    if (ob.sx > 0 && ob.sy > 0 && ob.Mat[ob.sx-1][ob.sy-1] > 0) {
            SetXCoord(ob.sx-1);
            SetYCoord(ob.sy-1);
        } else if (ob.sx < ob.maxx-1 && ob.sy > 0 && ob.Mat[ob.sx+1][ob.sy-1] > 0) {
            SetXCoord(ob.sx+1);
            SetYCoord(ob.sy-1);
        } else if (ob.sx < ob.maxx-1 && ob.sy < ob.maxy-1 && ob.Mat[ob.sx+1][ob.sy+1] > 0) {
            SetXCoord(ob.sx+1);
            SetYCoord(ob.sy+1);
        } else if (ob.sx > 0 && ob.sy < ob.maxy-1 && ob.Mat[ob.sx-1][ob.sy+1] > 0) {
            SetXCoord(ob.sx-1);
            SetYCoord(ob.sy+1);
        }
    }
    
    return _move;
};