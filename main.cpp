#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <curses.h>

#include"GameClasses.h"

namespace resources {
	int score = 0;
	
	int amountOfBombs = 3;
	int amountOfLife = 5;
	int startingLevel = 1;

	struct Highscore {
		char *nickname;
		long score;
		int level;
		Highscore *next;
	};
	Highscore *scoreList;
};

using namespace resources;
using namespace std;

void displayMenu() throw();
void displayOptions() throw();
void displayStory() throw();
void playGame() throw();

//** Scores and Results
void scores_LoadBest() throw();
void scores_SaveBest() throw();
void scores_CreateEmptyResultsFile() throw();
void scores_DisplayHighscores() throw();
void scores_OverwriteLowestScore() throw();
void scores_SortHighscores() throw();
void scores_ClearScoreListFromMemory() throw();

//** Game Constants and Settings values
bool option_PlayBeep = false;
static const char version [] = "1.0.0";
static const char results_file [] = "results.txt";

void playBeep () {
	if (true == option_PlayBeep) {
		cout<<"\a";
	}
};

int main(int argc, char *argv[], char *env[]) {
	//** set current time as a base for seed randomization
	//srand(int(time(NULL)));

	scores_LoadBest();

	char choise;
	while(true) {
		BEGINNING:
		displayMenu();

		choise = getchar();
		choise = tolower(choise);

		switch(choise) {
			case 'n': playGame(); break;
			case 'h': scores_DisplayHighscores(); break;
			case 'o': displayOptions(); break;
			case 'a': displayStory(); break;
			case 'q': cout<<"Thank you for playing!\nWe hope you like it and will get back soon!\nBye-bye!";
					  scores_ClearScoreListFromMemory();
					  exit(EXIT_SUCCESS);
					  break;
			default: cout<<"I did not understand your command. Give it another go!\n" << choise << endl;
					 goto BEGINNING;
					 break;
		}
	}

	return EXIT_SUCCESS;
};

void playGame() throw() {
	int xMazeDimension, yMazeDimension, nBots;
	bool dead = false;

	while (!dead) {
		srand(rand()%61329);

		xMazeDimension = (rand()%startingLevel)*2+12;
		yMazeDimension = (rand()%startingLevel)+9;

		if (xMazeDimension > 77) { xMazeDimension = 77; }
		if (yMazeDimension > 45) { yMazeDimension = 45; }

		Maze *newMaze = new Maze(xMazeDimension, yMazeDimension, startingLevel);
		newMaze->RandomizeMaze();
		newMaze->DisplayCurrentLives(amountOfLife);
		newMaze->DisplayCurrentBombCount(amountOfBombs);
		newMaze->DisplayCurrentScore(score);
		newMaze->DisplayCurrentLevel(startingLevel);
	
		//** Populating enemies starting from level #2
		nBots = 1;
		while (nBots*2 <= startingLevel) {
			if (newMaze->AddBot(rand()%xMazeDimension,
								rand()%yMazeDimension,
								rand()%7+1)) {
				nBots++;
			}
		}

		dead = false;
		newMaze->DrawMaze();
	
		bool fin = false;
		while (!fin && !dead) {
			fin = newMaze->PlayerMove();
			if (newMaze->GetRemainingLives() <= 0) {
				score = newMaze->GetRemainingScore() + newMaze->GetCalculatedBonus();
				goto END;
			}

			#ifdef DEBUG
				if (newMaze->return_debugV_nextMaze() == -1) {
					goto NEXTMAZE;
				}
			#endif

			dead = newMaze->KillAllBots();
			newMaze->DrawMaze();
		}
		
#ifdef DEBUG
NEXTMAZE:
#endif
		startingLevel++;

		//** Calculate level score
		score = newMaze->GetRemainingScore() + newMaze->GetCalculatedBonus();	
		amountOfLife = newMaze->GetRemainingLives();
		amountOfBombs = newMaze->GetRemainingBombCount();
		newMaze->~Maze();
	}

END:

	cout<<"\t\t\t\e[33m\e[31mGame Over\e[0m\n\n\n";
	cin.get();
	cin.get();
	
	scores_OverwriteLowestScore();
	scores_SortHighscores();
	scores_SaveBest();

	//** Reset score and level before next run
	score = 0;
	startingLevel = 1;
};

void displayMenu() throw() {
    cout<<"\t\e[33m\e[1m	Welcome to \"Deepest Descent\" ver." << version <<"\n";
	cout<<"(n) - New Game\n";
	cout<<"(h) - Highscores\n";
	cout<<"(o) - Options\n";
	cout<<"(a) - About\n";
	cout<<"(q) - Exit\n\n\e[0m";
};

void displayOptions() throw() {
	cout<<"Do you wish to hear a beep upon takin hit? (y/n):\n";
	char choise;
	cin >> choise;
	cin.get();

	if (choise == 'y') {
		option_PlayBeep = true;
		playBeep();
	} else if (choise == 'n') {
		option_PlayBeep = false;
	} else {
		return;
	}
};

void displayStory() throw() {
	/*
		Story so far
	*/
	cout<<"\e[33mSecluded dwarf embarks on a descent into a deepest corners of a cavern... (Press \'\e[34mEnter\e[33m\' to continue)";
	cin.get();
	cin.get();
	cout<<"Cavern once called \"Home\" by his kin... (Press \'\e[34mEnter\e[33m\' to continue)";
	cin.get();
	cout<<"Caverns now filled with various critters, and relics of the past... (Press \'\e[34mEnter\e[33m\' to continue)";
	cin.get();
	cout<<"His path is dangerous and his motives are unclear... (Press \'\e[34mEnter\e[33m\' to continue)";
	cin.get();
	cout<<"He may gain fame and glory... (Press \'\e[34mEnter\e[33m\' to continue)";
	cin.get();
	cout<<"Or find his demise... (Press \'\e[34mEnter\e[33m\' to continue) \e[0m\n";
	cin.get();

	/*
		How to Play
	*/
	cout<<"\e[33mHow to play:\n";
	cout<<" - avoid critters as much as possible\n";
	cout<<" - pick \e[31m\u2665\e[33m and \e[95m\u2620\e[33m to increase your chances on survival\n";
	cout<<" - reach \e[32m\u261F\e[33m to descent to lower level\n";
	cout<<" - \e[95m\u0024\e[33m and \e[35m\u00B7\e[33m give you money. The more money, the more fame?\e[0m\n";
	cout<<"\n\e[33m(Press \'\e[34mEnter\e[33m\' to continue)\e[0m\n";

	cin.get();
	/*
		Controls
	*/
	cout<<"\e[33mControls:\n";
	cout<<" - Use \'\e[1m\e[34mw\e[0m\e[33m\', \'\e[1m\e[34ma\e[0m\e[33m\', \'\e[1m\e[34ms\e[0m\e[33m\' and \'\e[1m\e[34md\e[0m\e[33m\' to move around\n";
	cout<<" - Use \'\e[1m\e[34mf\e[0m\e[33m\' to place a bomb. Just don't stand near when it will explode!\n";
	cout<<" - Use \'\e[1m\e[34me\e[0m\e[33m\' to descent even deeper when you have reached the exit.\n";
	cout<<" - Sometimes life gets sooo hard that you would like to end all this. Just press \'\e[1m\e[34mq\e[0m\e[33m\' then\e[0m\n";
	cout<<"\n\e[33m(Press \'\e[34mEnter\e[33m\' to continue)\e[0m\n";

	cin.get();
	/*
		Critters bestiary
	*/
	cout<<"\e[33mBestiary:\e[0m\n";
	cout<<"\e[1m\e[31mS\e[0m\e[33m - Skeletons are stupid, blind, deaf and barely can walk. But still can hurt you if you are not careful enough. These old bones can sometimes lose a bomb they carry around as a part of an old habit.\e[0m\n";
	cout<<"\n\e[33m(Press \'\e[34mEnter\e[33m\' to continue)\e[0m\n";
	cin.get();
	cout<<"\e[1m\e[31mw\e[0m\e[33m - Slimes are dangerous when they are legion. And they sure tend to split in half a lot!\e[0m\n";
	cout<<"\n\e[33m(Press \'\e[34mEnter\e[33m\' to continue)\e[0m\n";
	cin.get();
	cout<<"\e[1m\e[31mG\e[0m\e[33m - Scaredy Goblins are scared of you even more than you are of them. When confused they drop charged bombs below their feet!\e[0m\n";
	cout<<"\n\e[33m(Press \'\e[34mEnter\e[33m\' to continue)\e[0m\n";
	cin.get();
	cout<<"\e[1m\e[31mB\e[0m\e[33m - Cave Bears are apex predators of all known caverns. They sense and hunt you down wherever you are.\e[0m\n";
	cout<<"\n\e[33m(Press \'\e[34mEnter\e[33m\' to continue)\e[0m\n";
	cin.get();
	cout<<"\e[1m\e[31mN\e[0m\e[33m - Necromancers are not very smart, but they are keen on resurrecting any corpse they are to encounter. Clumsy by nature they tend to drop useful potions along the way.\e[0m\n";
	cout<<"\n\e[33m(Press \'\e[34mEnter\e[33m\' to continue)\e[0m\n";
	cin.get();
	cout<<"\e[1m\e[31mb\e[0m\e[33m - Cave Bats don't have strong senses but with some incomprehensible way they can disappear and appear so close to you that you might need a fresh pair of pants!\e[0m\n";
	cout<<"\n\e[33m(Press \'\e[34mEnter\e[33m\' to continue)\e[0m\n";
	cin.get();
	cout<<"\e[1m\e[31mD\e[0m\e[33m - Dark Dwarfes are demiurges of caverns. They destroy old passages and create new ones. They also can dislocate exit in a whim! Poor cartographers... they never will be out of work with this guys.\e[0m\n";
	cout<<"\n\e[33m(Press \'\e[34mEnter\e[33m\' to continue)\e[0m\n";

	cin.get();
};

void scores_LoadBest() throw() {
	char *tmp = new char[20];

	//** Try reading file with the results
	ifstream fout(results_file);
	
	//** If file can't be opened. We create new one, with empty records
	if (!fout.is_open()) {
		scores_CreateEmptyResultsFile();
	}

	//** Reat results file
	ifstream fou(results_file);
	fou >> tmp;
	
	scoreList = new Highscore;
	scoreList->nickname = new char[(int)strlen(tmp)];
	strcpy(scoreList->nickname, tmp);
	
	fou >> scoreList->score;
	fou >> scoreList->level;

	scoreList->next = NULL;
	Highscore *temp = scoreList;

	short i = 9;
	while(i) {
		fou >> tmp;
		temp->next = new Highscore;
		
		temp->next->nickname = new char[(int)strlen(tmp)];
		strcpy(temp->next->nickname, tmp);
		
		fou >> temp->next->score;
		fou >> temp->next->level;
		temp->next->next = NULL;
		temp = temp->next;
		
		i--;
	}
	
	delete[]tmp;
	scores_SortHighscores();
};

void scores_SaveBest() throw() {
	ofstream fin(results_file, ios_base::binary);
	Highscore *sv = scoreList;
	
	short i = 10;
	while (i) {
		fin << sv->nickname << "\t\t" << sv->score << "\t\t" << sv->level << endl;
		sv = sv->next;
		
		i--;
	}
};

void scores_CreateEmptyResultsFile() throw() {
	ofstream file;
	file.open(results_file);

	ofstream fin(results_file);
	for (int index = 0; index < 10; index++) {
		fin << "Empty\t\t" << 0 << "\t\t" << 0 << endl;
	}
};

void scores_DisplayHighscores() throw(){
	cout << "\t\t\tTop 10 Highscores\n";
	cout << setw(20) << std::right << "Name" << "\t\t" << "Score" << "\t\t" << "Level" << endl << endl;

	Highscore *tmp = scoreList;
	while (tmp != NULL) {
		cout << setw(20) << std::right << tmp->nickname << "\t\t" << tmp->score << "\t\t" << tmp->level << endl;
		tmp = tmp->next;
	}

	cin.get();
	cin.get();
};

/*
	This function overwrites last entry in local scores (memory only) if player scores higher
*/
void scores_OverwriteLowestScore() throw() {
	Highscore *tmp = scoreList;

	while (tmp->next != NULL) {
		tmp = tmp->next;	
	}

	if (tmp->score < score) {
		tmp->score = score;
		tmp->level = startingLevel-1;
		
		char *name = new char[20];

		cout << "Enter your name: ";
		cin.getline(name, 20);
		
		if (name[0] == '\0') {
			strcpy(name, "Anonymouse");
		}

		tmp->nickname = new char[(int)strlen(name)];
		strcpy(tmp->nickname, name);
		
		/*
			Next two lines prove to be super solution in case if entered string is longer than expected.
			This resulted in redundant inputs later on.
		*/
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		delete[]name;
	}
};

void scores_SortHighscores()throw(){
	Highscore *tmp = scoreList;
	Highscore *pop = scoreList;
	Highscore *rare = NULL;
	Highscore *prev = scoreList;
	Highscore *rar = NULL;

	int h_m = 0;

	while (tmp != NULL) {
		h_m++;
		tmp = tmp->next;
	}

	for (int index = 0; index <h_m; index++) {
		while (pop->next != NULL) {
			if ((pop->score < pop->next->score) && pop == scoreList) {
				rare = pop->next->next;
				rar = scoreList;
				scoreList = pop->next;
				rar->next = NULL;
				scoreList->next = rar;
				scoreList->next->next = rare;
			} else {
				if (pop->score < pop->next->score) {
					rare = pop;
					rar = pop->next;
					rare->next = pop->next->next;
					rar->next = rare;
					prev->next = rar;
					pop = prev;
				}
			}
			
			prev = pop;
			pop = pop->next;
		}

		pop = scoreList;
	}
};

void scores_ClearScoreListFromMemory() throw() {
	Highscore *del = scoreList;
	while (scoreList != NULL) {
		del = scoreList;
		scoreList = scoreList->next;
		delete del;
	}
	
	delete scoreList;
};