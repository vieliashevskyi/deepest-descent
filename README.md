# Deepest Descent ver. 1.0.0 for macOS

## About the Game
Game design was inspired by limitations (both graphical and technical) of early personal computers. This is achieved by few selling points:
* Game is played in system Terminal (Console) only
* Maze is redrawn as a new output after each turn, giving you a chance to scroll through the history of maze states
* Art style is simplistic and was heavily inspired by Dwarfs Fortress aesthetics
* Player is able to type in a sequence of moves that will be executed one-by-one
  * This is a reason for requirement to tap **Enter** to perform a move after input. This is partially to simulate input style on older devices
* Completely chaotic level generation. It has so few content and enemy constraints that you can encounter truly idiotic mazes
* Local leaderboard. Compete against your friends and family and hope that luck will be on your side
* 7 enemies with different movement patterns and super powers to encounter, defeat or elude

## How to Run and Play
#### Run Build uploaded to Itch (macOS):
* Download game from [itch.io](https://vieliashevskyi.itch.io/deepest-descent)
* Unzip it
* Head to Terminal
* Navigate to folder with unzipped contents
* type: **"./Deepest\ Descent"**

#### Run Build uploaded to Releases section on GitHub (macOS):
* Download zip-file with macOS executable from release section
* Unzip it
* Head to Terminal
* Navigate to folder with unzipped contents
* type: **"./Deepest\ Descent"**

#### Build game from sources (macOS):
* Clone source code
* Build project using Visual Code and attached configuration
* Run newly compiled executable

#### Build game from sources (Linux):
* Clone source code
* Add following includes inside **main.cpp**:
```cpp
#include <string.h>
#include <iomanip>
#include <limits>
```
* Build project using following command:
```
gcc -g main.cpp -g MazeProcessor.cpp -g ./Bots/*.cpp -o "Deepest Descent" -lncurses -lstdc++
```
* If you encounter message `bash: ./Deepest\ Descent: permission denied`. You need to execute following command before you can run the game with super user privileges:
```
sudo chmod u+x "Deepest Descent"
```
* Run newly compiled executable

#### Play in few words:
* Use **w**, **a**, **s** and **d** to move **up**, **left**, **down** and **right** respectively
* Use **f** to plant a bomb
* Use **e** to enter next level when you will reach exit point or "skip movement phase"

For more details, bestiary and lore head to **About** section inside the game.

## Playthrough Example
![Quick playthrough](/Img/deepest-descent.gif)

## Level Examples
![Level #1 example](/Img/deepest-descent-lvl-01.png)
![Level #2 example](/Img/deepest-descent-lvl-02.png)
![Level #3 example](/Img/deepest-descent-lvl-03.png)
![Level #4 example](/Img/deepest-descent-lvl-04.png)
![Level #5 example](/Img/deepest-descent-lvl-05.png)

## Different Looks in Different Terminal Themes
![Different colour schemes](/Img/deepest-descent-consoles.png)

Hope you will have fun!
