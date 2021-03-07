#include <iostream>
#include <ctime>
#include <curses.h>
#include "GameClasses.h"

extern void playBeep();

using namespace std;

Maze::Maze(int mx, int my, int level) throw() {
	maxx = mx;
	maxy = my;
	Mat = new int*[maxx];
	for(int i = 0; i < maxx; i++) {
		Mat[i] = new int[maxy];
	}

	innerBot = NULL;
	_level = level;
	_bonus = _level*50;
};

Maze::~Maze() throw() {
	for(int i=0;i<maxx;i++) {
		delete []Mat[i];
	}
	delete []Mat;
};

void Maze::RandomizeMaze() throw() {
	for(int x = 0; x < maxx; x++) {
		for(int y = 0; y < maxy; y++) {
			if (rand()%(200-_level*_level) == 0) {
				Mat[x][y] = TileID_MoneyBag;
			} else if (rand()%(200-_level*_level) == 0) {
				Mat[x][y] = TileID_BombItem;
			} else if (rand()%(300-_level*_level) == 0) {
				Mat[x][y] = TileID_Life;
			} else if(rand()%3 == 0) {
				Mat[x][y] = TileID_Wall;
			} else if (rand()%2 == 0) {
				Mat[x][y] = TileID_ScorePoint;
			} else {
				Mat[x][y] = TileID_Passage;
			}
		}
	}

	bool set;
	STEP:

	//** Generate Player starting position
	set = true;
	while(set) {
		sx = rand()%maxx;
		sy = rand()%maxy;
		if (Mat[sx][sy]) {
			set = false;
		}
	}
	
	//** Generate exit position
	set = true;
	while(set) {
		ex = rand()%maxx;
		ey = rand()%maxy;
		if (Mat[ex][ey]) {
			set = false;
		}
	}

	//** Sanity checks
	if (!IsWay(sx, sy, ex, ey) || (sx == ex && sy == ey)) {
		goto STEP;
	}
	if (abs(sx-ex)+abs(sy-ey) < ((maxx+maxy)/3)) {
		goto STEP;
	}
	
	Mat[sx][sy] = TileID_Passage;
};

void Maze::DrawMaze() const throw() {
	/*
		Art references:

		1) ASCII Boxes -> https://en.wikipedia.org/wiki/Box-drawing_character
		2) Colour Coding -> https://misc.flogisoft.com/bash/tip_colors_and_formatting
		3) Unicode Symbols -> https://unicode-table.com/en/
		4) Enemy 'art' style -> https://dwarffortresswiki.org/index.php/v0.31:List_of_creatures
	*/

	cout<<"\e[33m\e[1mLife - "	<<_life
		<<"\tLevel - "			<<_level
		<<"\tBombs - "			<<_bombs
		<<"\tScore - "			<<_score
		<<"\tBonus - "			<<_bonus
		<<"\n\e[0m";

	/*
		Create Top Border frame
	*/
	cout<<"\u250F";
	for (int i = 0; i < maxx; i++) {
		cout<<"\u2501";
	}
	cout<<"\u2513\n";

	/*
		Draw maze insides
	*/
	for (int y = 0; y < maxy; y++) {
		cout<<"\u2503";
		for (int x = 0; x < maxx; x++) {
			if ((sx == x) && (sy == y)) {
				//** Player as a smiley face
				cout << "\e[36m\u263A\e[0m";
			} else if((ex == x) && (ey == y)) {
				//** Maze exit point
				cout<<"\e[32m\u261F\e[0m";
			} else if (WhatChar(x, y) != '?') {
				//** Display enemy specific Letter
				if (WhatChar(x, y) != '-') {
					cout<<"\e[1m\e[31m"<<WhatChar(x, y)<<"\e[0m";
				} else {
					if (WhatDeadBot(x, y) != OwnerID_ArmedBomb) {
						//** If bot is dead, we show small cross
						cout<<"\e[90m\u2671\e[0m";
					} else {
						//** Bomb impact point represented as a star (crater?)
						cout<<"\e[37m*\e[0m";
					}
				}
			} else {
				switch(Mat[x][y]){
					case TileID_Wall:		cout<<"\u25AE"; break;
					case TileID_Passage:	cout<<" "; break;
					case TileID_ScorePoint:	cout<<"\e[35m\u00B7\e[0m"; break;					
					case TileID_Life:		cout<<"\e[31m\u2665\e[0m"; break;
					case TileID_MoneyBag:	cout<<"\e[95m\u0024\e[0m"; break;
					case TileID_BombItem:	cout<<"\e[95m\u2620\e[0m"; break;
				};
			}
		}
		cout<<"\u2503\n";
	};

	/*
		Create Bottom Border frame
	*/
	cout<<"\u2517";
	for (int i = 0; i < maxx; i++) {
		cout<<"\u2501";
	}
	cout<<"\u251B"<<'\n';
};

bool Maze::PlayerMove() throw() {
	int SX, SY;
	char ch;

	#ifdef DEBUG
		_debugV_nextmaze = 0;
	#endif
	
	STEP:
	//** Player Turn
	ch = getchar();
	ch = tolower(ch);

	switch(ch) {
		case 'w':	if (sy > 0 && Mat[sx][sy-1] > 0) {
						SX = sx;
						SY = sy-1;

						if (WhatBot(sx, sy-1) >= OwnerID_Skeleton && WhatBot(sx, sy-1) < OwnerID_ArmedBomb) {
							_life--;
							playBeep();
						}
					} else {
						goto STEP;
					} break;
		case 's':	if (sy < maxy-1 && Mat[sx][sy+1] > 0) {
						SX = sx;
						SY = sy+1;

						if (WhatBot(sx, sy+1) >= OwnerID_Skeleton && WhatBot(sx, sy+1) < OwnerID_ArmedBomb) {
							_life--;
							playBeep();
						}
					} else {
						goto STEP;
					} break;
		case 'a':	if (sx > 0 && Mat[sx-1][sy] > 0) {
						SX = sx-1;
						SY = sy;

						if (WhatBot(sx-1, sy) >= OwnerID_Skeleton && WhatBot(sx-1, sy) < OwnerID_ArmedBomb) {
							_life--;
							playBeep();
						}
					} else {
						goto STEP;
					} break;
		case 'd':	if (sx < maxx-1 && Mat[sx+1][sy] > 0) {
						SX = sx+1;
						SY = sy;

						if (WhatBot(sx+1, sy) >= OwnerID_Skeleton && WhatBot(sx+1, sy) < OwnerID_ArmedBomb) {
							_life--;
							playBeep();
						}
					} else {
						goto STEP;
					} break;
		case 'f':	if (sx == ex && sy==ey) {
						return true;
					} else if (_bombs > 0) {
						AddBot(sx, sy, OwnerID_ArmedBomb);
						_bombs--;
					}
					SX = sx; 
					SY = sy;
					break;
		case 'e':	if (sx == ex && sy == ey) {
						return true; 
					}
					SX = sx;
					SY = sy;
					break;
		case 'q':	_life = 0;
					return true;
					break;
		#ifdef DEBUG
		case 'x':	_debugV_nextmaze = -1;
					SX = sx;
					SY = sy;
					break;
		#endif
		default :	goto STEP;
					break;
	};

	switch(Mat[sx][sy]) {
		case TileID_ScorePoint		:	_score += _level;
										SetTile(sx, sy, TileID_Passage);
										break;
		case TileID_Life			:	_life++;
										SetTile(sx, sy, TileID_Passage);
										break;
		case TileID_MoneyBag		:	_score += _level*10;
										SetTile(sx, sy, TileID_Passage);
										break;
		case TileID_BombItem		:	_bombs++;
										SetTile(sx, sy, TileID_Passage);
										break;
		default 					:	break;
	}

	//** Bots turn
	srand(time(NULL));
	int move = 4;
	Bot *tmp = this->innerBot;

	while (tmp != NULL) {
		if (tmp->IsAlive()) {
			if (tmp->GetOwner() == OwnerID_ScaredyGoblin) {
				switch(tmp->BotMove(*this)) {
					case 3 :	if (tmp->GetYCoord() > 0 && Mat[tmp->GetXCoord()][tmp->GetYCoord()-1] > 0) {
									tmp->SetYCoord(tmp->GetYCoord()-1);
								} break;
					case 2 :	if (tmp->GetXCoord() < maxx-1 && Mat[tmp->GetXCoord()+1][tmp->GetYCoord()] > 0) {
									tmp->SetXCoord(tmp->GetXCoord()+1);
								} break;
					case 1 :	if (tmp->GetYCoord() < maxy-1 && Mat[tmp->GetXCoord()][tmp->GetYCoord()+1] > 0) {
									tmp->SetYCoord(tmp->GetYCoord()+1);
								} break;
					case 0 :	if (tmp->GetXCoord() > 0 && Mat[tmp->GetXCoord()-1][tmp->GetYCoord()] > 0) {
									tmp->SetXCoord(tmp->GetXCoord()-1);
								} break;
				}
			} else {
				if (tmp->GetOwner() == OwnerID_DarkDwarf) {
					switch(tmp->BotMove(*this)) {
						case 0 :	if (tmp->GetYCoord() > 0) {
										tmp->SetYCoord(tmp->GetYCoord()-1);
									} break;
						case 1 :	if (tmp->GetXCoord() < maxx-1) {
										tmp->SetXCoord(tmp->GetXCoord()+1);
									} break;
						case 2 :	if (tmp->GetYCoord() < maxy-1) {
										tmp->SetYCoord(tmp->GetYCoord()+1);
									} break;
						case 3 :	if (tmp->GetXCoord() > 0) {
										tmp->SetXCoord(tmp->GetXCoord()-1);
									} break;			
					}
				} else {
					switch(tmp->BotMove(*this)) {
						case 0 :	if (tmp->GetYCoord() > 0 && Mat[tmp->GetXCoord()][tmp->GetYCoord()-1] > 0) {
										tmp->SetYCoord(tmp->GetYCoord()-1);
									} break;
						case 1 :	if (tmp->GetXCoord() < maxx-1 && Mat[tmp->GetXCoord()+1][tmp->GetYCoord()] > 0) {
										tmp->SetXCoord(tmp->GetXCoord()+1);
									} break;
						case 2 :	if (tmp->GetYCoord() < maxy-1 && Mat[tmp->GetXCoord()][tmp->GetYCoord()+1] > 0) {
										tmp->SetYCoord(tmp->GetYCoord()+1);
									} break;
						case 3 :	if (tmp->GetXCoord() > 0 && Mat[tmp->GetXCoord()-1][tmp->GetYCoord()] > 0) {
										tmp->SetXCoord(tmp->GetXCoord()-1);
									} break;
					};
				}
			}
		}

		tmp = tmp->next;
	}
	
	sx = SX;
	sy = SY;
	if (_bonus > 0) {
		_bonus--;
	}

	return false;
};

bool Maze::IsWay(int x1, int y1, int x2, int y2) const throw() {
	int **W,x,y;
	W = new int *[maxx];
	for (int i = 0; i < maxx; i++) {
		W[i] = new int[maxy];
	}

	for (int i = 0; i < maxx; i++) {
		for (int j = 0; j < maxy; j++) {
			W[i][j] = 0;
		}
	}

	x = x1;
	y = y1;
	
	if ((x+1 < maxx) && (Mat[x+1][y])) {
		W[x+1][y] = 1;
	}
	if ((x-1 >= 0) && (Mat[x-1][y])) {
		W[x-1][y] = 1;
	}
	if ((y+1 < maxy) && (Mat[x][y+1])) {
		W[x][y+1] = 1;
	}
	if ((y-1 >= 0) && (Mat[x][y-1])) {
		W[x][y-1] = 1;
	}

	bool finish = true;
	int ii = 1;
	while (finish) {
		finish = false;
		for (y = 0; y < maxy; y++) {
			for (x = 0; x < maxx; x++) {
				if (W[x][y] == ii) {
					if ((x+1 < maxx) && (Mat[x+1][y]) && (W[x+1][y] == 0)) {
						W[x+1][y] = ii+1;
					}
					if ((x-1 >= 0) && (Mat[x-1][y]) && (W[x-1][y] == 0)) {
						W[x-1][y] = ii+1;
					}
					if ((y+1 < maxy) && (Mat[x][y+1]) && (W[x][y+1] == 0)) {
						W[x][y+1] = ii+1;
					}
					if ((y-1 >= 0) && (Mat[x][y-1]) && (W[x][y-1] == 0)) {
						W[x][y-1] = ii+1;
					}
					finish = true;
				}
			}
		}
		ii++;
	}
	
	bool result;
	if (W[x2][y2] > 0) { 
		result = true;
	} else {
		result = false;
	}

	for (int i = 0; i < maxx; i++) {
		delete []W[i];
	}
	delete []W;
	
	return result;
};

bool Maze::AddBot(int xx, int yy, int owner) throw() {
	Bot *tmp = NULL, *ttt;
	//** Instantiating Armed Bomb
	if (owner == OwnerID_ArmedBomb) {
		tmp = new ArmedBomb(xx, yy);
		if (this->innerBot == NULL) {
			this->innerBot = tmp;
		} else {
			ttt = this->innerBot;
			while (ttt->next != NULL) {
				ttt = ttt->next;
			}
			ttt->next = tmp;
		}
		return true;
	}

	if (Mat[xx][yy] > 0 && 
		IsWay(xx, yy, sx, sy) && 
		(xx != sx || yy != sy) && 
		(xx != ex || yy != ey)) {
		switch(owner) {
			case OwnerID_Default		:	break;
			case OwnerID_Skeleton		:	tmp = new SkeletonBot(xx, yy); break;
			case OwnerID_Slime			:	tmp = new SlimeBot(xx, yy); break;
			case OwnerID_CaveBear		:	tmp = new CaveBearBot(xx, yy); break;
			case OwnerID_Necromancer	:	tmp = new NecromancerBot(xx, yy); break;
			case OwnerID_ScaredyGoblin	:	tmp = new ScaredyGoblinBot(xx, yy); break;
			case OwnerID_CaveBat		:	tmp = new CaveBatBot(xx, yy); break;
			case OwnerID_DarkDwarf		:	tmp = new DarkDwarfBot(xx, yy); break;
			case OwnerID_ArmedBomb		:	break;
			default						:	break;
		}
		if (this->innerBot == NULL) {
			this->innerBot = tmp;
		} else {
			ttt = this->innerBot;
			while (ttt->next != NULL) {
				ttt = ttt->next;
			}
			ttt->next = tmp;
		}
		
		return true;
	} else {
		return false;
	}
};

int Maze::WhatDeadBot(int xx, int yy) const throw() {
	Bot *tmp = this->innerBot;
	while (tmp != NULL) {
		if (tmp->GetXCoord() == xx && tmp->GetYCoord() == yy) {
			return tmp->GetOwner();
		}
		else tmp = tmp->next;
	}
	return 0;
}

int Maze::WhatBot(int xx, int yy) const throw() {
	Bot *tmp = this->innerBot;
	while (tmp != NULL) {
		if (tmp->GetXCoord() == xx && tmp->GetYCoord() == yy && tmp->IsAlive()) {
			return tmp->GetOwner();
		}
		else tmp = tmp->next;
	}
	return 0;
};

char Maze::WhatChar(int xx,int yy) const throw() {
	Bot *tmp = this->innerBot;
	int x, y;
	while (tmp != NULL) {
		x = tmp->GetXCoord();
		y = tmp->GetYCoord();

		if (x == xx && y == yy) {
			return tmp->GetChar();
		}
		tmp = tmp->next;
	}

	return '?';
};

void Maze::KillBotsInCoord(int xx, int yy) throw() {
	Bot *tmp = this->innerBot;
	while(tmp != NULL) {
		if (tmp->GetXCoord() == xx && tmp->GetYCoord() == yy &&
			tmp->IsAlive() && tmp->GetOwner() != OwnerID_ArmedBomb)	{
				tmp->alive = false;
			}
		tmp = tmp->next;
	}
}

bool Maze::KillAllBots() throw() {
	Bot *tmp = this->innerBot;
	while (tmp != NULL) {
		if (tmp->IsAlive() && 
			tmp->GetXCoord() == sx && tmp->GetYCoord() == sy && 
			tmp->GetOwner() != OwnerID_ArmedBomb) {
			_life--;
			playBeep();
		}

		if (_life <= 0) {
			return true;
		}
		tmp = tmp->next;
	}
	
	return false;
};