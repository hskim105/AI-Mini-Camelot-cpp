#include<iostream>
#include<string>

#include "Board.h"
#include "Human.h"
#include "CPU.h"
#include "Game.h"
using namespace std;



int main(){
    //Set color for each player
    string HUMAN_TEAM = "WHITE";
    string CPU_TEAM = "BLACK";

    //Initialize game
    Board* gameBoard = new Board();
    Human* human = new Human(HUMAN_TEAM);
    CPU* cpu = new CPU(CPU_TEAM);

    //Start game
    Game* game = new Game(human, cpu, gameBoard);

    //Game loop
    game->gameLoop();
}
