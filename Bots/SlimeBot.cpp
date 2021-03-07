#include "../GameClasses.h"

SlimeBot::SlimeBot(int xx, int yy) throw() : Bot(xx, yy) {
	SetOwner(OwnerID_Slime);
	SetChar('w');
};

int	SlimeBot::BotMove(Maze &ob) throw() {
	int i, bx, by;
    bx = GetXCoord();
    by = GetYCoord();
    /*
        Slime bot super power is to split itself in half
    */
	if (rand()%50 == 0) {
		ob.AddBot(bx, by, OwnerID_Slime);
    }

    STEP_RANDOM:
	    i = rand()%7;
    STEP_RANDOM_1:
        switch(i) {
            case 0: if (by > 0 && ob.Mat[bx][by-1] > 0) { return 0; } break;
            case 1: if (bx < ob.maxx-1 && ob.Mat[bx+1][by] > 0) { return 1; } break;
            case 2: if (by < ob.maxy-1 && ob.Mat[bx][by+1] > 0) { return 2; } break;
            case 3: if (bx > 0 && ob.Mat[bx-1][by] > 0) { return 3; } break;
            case 4: if ((bx-ob.sx) < 0) {
                        i = 1;
                    } else {
                        i = 3;
                    }
                    goto STEP_RANDOM_1; break;
            case 5: if ((by-ob.sy) < 0) {
                        i = 2;
                    } else {
                        i = 0;
                    }
                    goto STEP_RANDOM_1; break;
            case 6: if (abs(by-ob.sy) > abs(bx-ob.sx)) {
                        i = 5;
                    } else {
                        i = 4;
                    }
                    goto STEP_RANDOM_1; break;	
        };
        goto STEP_RANDOM;
};