#include <iostream>
#include "../GameClasses.h"

using namespace std;

namespace FSW {
	void FindWay (int **MS, int mx, int my, int **MR, int sx, int sy) throw() {
		for (int j = 0; j < my; j++) {
			for (int i = 0; i < mx; i++) {
				MR[i][j] = 0;
			}
		}

		MR[sx][sy] = 1;
		bool y;
		int p = 1;
		
		do {
			y = false;
			for (int j = 0; j < my; j++) {
				for (int i = 0; i < mx; i++) {
					if (MR[i][j] == p) {
						if (i < mx-1 && MS[i+1][j] && !MR[i+1][j]) {
							MR[i+1][j] = p+1;
						}
						if (i > 0 && MS[i-1][j] && !MR[i-1][j]) {
							MR[i-1][j] = p+1;
						}
						if (j < my-1 && MS[i][j+1] && !MR[i][j+1]) {
							MR[i][j+1] = p+1;
						}
						if (j > 0 && MS[i][j-1] && !MR[i][j-1]) {
								MR[i][j-1]=p+1;
						}
						y = true;
					}
				}
			}
			p++;
		} while(y);
	}

	void GiveWay (int ex, int ey, int **MS, int mx, int my) throw() {
		bool **HM = new bool*[mx];
		for (int i = 0; i < mx; i++) {
			HM[i] = new bool[my];
			for(int j = 0; j < my; j++) {
				HM[i][j] = false;
			}
		}
		
		int xg = ey, yg = ex;
		int per;
		
		do {
			HM[xg][yg] = true;
			per = MS[xg][yg];
			per--;
			if (xg < mx-1 && MS[xg+1][yg] == per) {
				xg++;
				continue;
			}
			if (xg > 0 && MS[xg-1][yg] == per) {
				xg--;
				continue;
			}
			if (yg < my-1 && MS[xg][yg+1] == per) {
				yg++;
				continue;
			}
			if (yg > 0 && MS[xg][yg-1] == per) {
				yg--;
				continue;
			}
		} while (MS[xg][yg] > 1);

		for (int j = 0; j < my; j++) {
			for(int i = 0; i < mx; i++) {
				if(!HM[i][j]) {
					MS[i][j] = 0;
				}
			}
		}
	}

	void ShowMTX (int **M, int x, int y) throw() {
		for (int j = 0; j < y; j++) {
			for (int i = 0; i < x; i++) {
				cout<<M[i][j]<<"\t";
			}
			cout<<endl;
		}
	}
}

using namespace FSW;

CaveBearBot::CaveBearBot(int xx, int yy) throw() : Bot(xx, yy) {
	SetOwner(OwnerID_CaveBear);
	SetChar('B');
};

int CaveBearBot::BotMove(Maze &ob) throw() {
	/*
        This bot hunts down player at all times. The most dangerous predator out there.
    */

	int bx = GetYCoord(),
		by = GetXCoord();
	
	if (!ob.IsWay(GetXCoord(), GetYCoord(), ob.sx, ob.sy)) {
		return rand()%4;
	}

	ob.Mat[by][bx] = 1;

	if (ob.sy-1 == bx && by == ob.sx) {
		return 2;
	}
	if (ob.sy+1 == bx && by == ob.sx) {
		return 0;
	}
	if (ob.sx-1 == by && bx == ob.sy) {
		return 1;
	}
	if (ob.sx+1 == by && bx == ob.sy) {
		return 3;
	}

	int **SWP, **SWB;
	SWP = new int*[ob.maxx];
	SWB = new int*[ob.maxx];

	for (int i = 0; i < ob.maxx; i++) {
		SWP[i] =new int[ob.maxy];
		SWB[i] =new int[ob.maxy];
	}

	FindWay(ob.Mat, ob.maxx, ob.maxy, SWP, ob.sx, ob.sy);
	FindWay(ob.Mat, ob.maxx, ob.maxy, SWB, by, bx);
	GiveWay(ob.ey, ob.ex, SWP, ob.maxx, ob.maxy);

	/*
		Cave Bear super move is to drop Money bag on a field.
		Think of this as Bear having so strong and heavy paws that by chasing the Player
		he unearthed old treasures.
	*/
	if (rand()%50 == 0) {
		ob.SetTile(GetXCoord(), GetYCoord(), TileID_MoneyBag);
	}

	int dx, dy;
	int riz = SWB[ob.sx][ob.sy];

	for (int j = 0; j < ob.maxy; j++) {
		for (int i = 0; i < ob.maxx; i++) {
			if (SWP[i][j]) {
				if ((SWB[i][j] - SWP[i][j]) < riz && (SWB[i][j] - SWP[i][j]) >= -1) {
					riz = SWB[i][j] - SWP[i][j];
					dx = i;
					dy = j;
				}
				if (riz <= 0) {
					break;
				}
			}
		}

		if (riz <= 0){
			break;
		}
	}

	GiveWay (dy, dx, SWB,ob.maxx, ob.maxy);
	
	if (bx < ob.maxy-1 && SWB[by][bx+1]) {
		return 2;
	}
	if (bx > 0 && SWB[by][bx-1]) {
		return 0;
	}
	if (by < ob.maxx-1 && SWB[by+1][bx]) {
		return 1;
	}
	if (by > 0 && SWB[by-1][bx]) {
		return 3;
	}

	return rand()%4;
}