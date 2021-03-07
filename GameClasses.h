#ifndef _classes
#define _classes

#include <cstdlib>

enum Owner {
	OwnerID_Default = -1,
	OwnerID_Skeleton = 1,
	OwnerID_Slime = 2,
	OwnerID_CaveBear = 3,
	OwnerID_Necromancer = 4,
	OwnerID_ScaredyGoblin = 5,
	OwnerID_CaveBat = 6,
	OwnerID_DarkDwarf = 7,
	OwnerID_ArmedBomb = 8
};

enum TileIDs {
	TileID_Wall = 0,
	TileID_Passage = 1,
	TileID_ScorePoint = 2,
	TileID_Life = 3,
	TileID_MoneyBag = 4,
	TileID_BombItem = 5
};

class Maze;
class Bot {
	private:
		int bx;
		int by;
		int owner;
		char mychar;

	public:
		Bot *next;
		bool alive;

		Bot(int xx = 0, int yy = 0) throw();
		virtual int BotMove(Maze &ob) = 0;

		void SetXCoord(int x) throw() { bx = x; };
		void SetYCoord(int y) throw() { by = y; };
		void SetOwner(Owner in_owner) throw() { owner = in_owner; };
		void SetChar(char ch) throw() { mychar = ch; };

		int GetXCoord() const throw() { return bx; };
		int GetYCoord() const throw() { return by; };
		int GetOwner() const throw() { return owner; };
		char GetChar() const throw() { 
			if (false == alive) {
				return '-';
			}
			return mychar; 
		};

		bool IsAlive() const throw() { return alive; };
};

class SkeletonBot : public Bot {
public:
	SkeletonBot(int xx, int yy) throw();
	int BotMove(Maze &ob) throw();
};

class SlimeBot : public Bot {
public:
	SlimeBot(int xx, int yy) throw();
	int BotMove(Maze &ob) throw();
};

class NecromancerBot : public Bot {
public:
	NecromancerBot(int xx, int yy) throw();
	int BotMove(Maze &ob) throw();

	void Revive(int, int, Maze &ob) throw();
};

class CaveBearBot : public Bot {
public:
	CaveBearBot(int xx, int yy) throw();
	int BotMove(Maze &ob) throw();
};

class ScaredyGoblinBot : public Bot {
public:
	ScaredyGoblinBot(int xx, int yy) throw();
	int BotMove(Maze &ob) throw();
};

class CaveBatBot : public Bot {
public:
	CaveBatBot(int xx, int yy) throw();
	int BotMove(Maze &ob) throw();
};

class DarkDwarfBot : public Bot {
public:
	DarkDwarfBot(int xx, int yy) throw();
	int BotMove(Maze &ob) throw();
};

class ArmedBomb : public Bot {
private:
	int charge;

public:
	ArmedBomb(int xx, int yy) throw();
	int BotMove(Maze &ob) throw();
};

class Maze {
	friend int SkeletonBot::BotMove(Maze &ob) throw();
	friend int SlimeBot::BotMove(Maze &) throw();
	friend int NecromancerBot::BotMove(Maze &) throw();
	friend int CaveBearBot::BotMove(Maze &) throw();
	friend int ScaredyGoblinBot::BotMove(Maze &) throw();
	friend int CaveBatBot::BotMove(Maze &) throw();
	friend int DarkDwarfBot::BotMove(Maze &) throw();
	friend int ArmedBomb::BotMove(Maze &ob) throw();
	friend void NecromancerBot::Revive(int, int, Maze &ob) throw();

private:
	int maxx;
	int maxy;
	int **Mat;
	/*
		sx and sy - represent starting coorinates. In other words - player coordinates.
		ex, ey - level exit coordinates.
	*/
	int sx, sy, ex, ey;
	int _bonus;
	
	Bot *innerBot;
	int _score;
	int _life;
	int _level;
	int _bombs;

/*
	To enable DEBUG mode simply pass "-DDEBUG" as one of arguments (args) inside "tasks.json".
	Now when level was loaded input 'x' to generate next maze.
*/
#ifdef DEBUG
	int _debugV_nextmaze;
#endif

public:
#ifdef DEBUG
	int return_debugV_nextMaze() { return _debugV_nextmaze; }
#endif

	Maze(int mx = 77, int my = 45, int level = 1) throw();
	~Maze() throw();

	void RandomizeMaze() throw();
	void DrawMaze() const throw();
	
	void DisplayCurrentLives(int life) throw() { _life = life; };
	void DisplayCurrentScore(int score) throw() { _score = score; };
	void DisplayCurrentLevel(int level) throw()
	{
		_level = level;
		_bonus = _level * 25;
	};
	void DisplayCurrentBombCount(int bombCount) throw() { _bombs = bombCount; };

	void SetTile(int xx, int yy, int tid) throw() { Mat[xx][yy] = tid; };
	void SetExitXCoord(int xx) throw() { ex = xx; };
	void SetExitYCoord(int yy) throw() { ey = yy; };
	
	void KillBotsInCoord(int, int) throw();
	bool KillAllBots() throw();

	bool IsWay(int, int, int, int) const throw();
	bool AddBot(int xx, int yy, int owner) throw();
	
	bool PlayerMove() throw();

	char WhatChar(int xx, int yy) const throw();
	int WhatBot(int xx, int yy) const throw();
	int WhatDeadBot(int xx, int yy) const throw();
	
	int GetTile(int xx, int yy) const throw() { return Mat[xx][yy]; };
	int GetPlayerXCoord() const throw() { return sx; };
	int GetPlayerYCoord() const throw() { return sy; };
	
	int GetExitXCoord() const throw() { return ex; };
	int GetExitYCoord() const throw() { return ey; };

	int GetRemainingScore() const throw() { return _score; };
	int GetRemainingLives() const throw() { return _life; };
	int GetCalculatedBonus() const throw() { return _bonus; };
	int GetRemainingBombCount() const throw() { return _bombs; };
};

#endif