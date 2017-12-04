#ifndef Game_h
#define Game_h

#include<iostream>

#include "Human.h"
#include "CPU.h"
#include "Board.h"

namespace Game{

    //This function starts the game
    bool startGame(Human* theHuman, CPU* theCPU, Board* theBoard);

    //Asks player if he wanst to play first
    bool playFirst();

    //Game loop will handle the actual play of the game
    void gameLoop(Human* theHuman, CPU* theCPU, Board* theBoard, bool gameRunning, bool humanTurn);
}
#endif /* Game_h */
