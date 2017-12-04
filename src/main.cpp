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
    Board gameBoard = Board();
    Human human = Human(HUMAN_TEAM);
    CPU cpu = CPU(CPU_TEAM);

    //Start game
    bool game_running = Game::startGame(&human, &cpu, &gameBoard);
    bool human_turn = Game::playFirst();

    //Game loop
    Game::gameLoop(&human, &cpu, &gameBoard, game_running, human_turn);
}
