#ifndef Game_h
#define Game_h

#include<iostream>

#include "Human.h"
#include "CPU.h"
#include "Board.h"

namespace Game{
    enum WinValue {
        HumanWin = -1000,
        Draw = 0,
        CPUWin = 1000,
        None
    };

    //This function starts the game
    bool startGame(Human* theHuman, CPU* theCPU, Board* theBoard);

    //Asks player if he wanst to play first
    bool playFirst();

    //Game loop will handle the actual play of the game
    void gameLoop(Human* theHuman, CPU* theCPU, Board* theBoard, bool gameRunning, bool humanTurn);

    //Checks if a player won
    WinValue checkWin(Human* theHuman, CPU* theCPU, Board* theBoard);

    //Player occupies both of opponent's castles
    bool winCondition1(Board* theBoard, const std::vector< std::pair<int, int> >& oppenentCastle, std::string& teamColor);

    //Player captures all of opponent's piece while retaining at least 2 pieces
    bool winCondition2(std::vector<Player::Piece>& myPieces, std::vector<Player::Piece>& enemyPieces);

    //Both players have 1 or less piece
    bool drawCondition(std::vector<Player::Piece>& myPieces, std::vector<Player::Piece>& enemyPieces);
}
#endif /* Game_h */
