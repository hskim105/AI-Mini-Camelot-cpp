#ifndef Game_h
#define Game_h

#include<iostream>

#include "Human.h"
#include "CPU.h"
#include "Board.h"

class Game{

public:
    enum WinValue {
        HumanWin = -1000,
        Draw = 0,
        CPUWin = 1000,
        None
    };

    //Default constructor
    //When creating a new game object, you automatically start the game
    Game(Human* theHuman, CPU* theCPU, Board* theBoard);

    //Game loop will handle the actual play of the game
    void gameLoop();

    //Checks if a player won
    WinValue checkWin(Human* theHuman, CPU* theCPU, Board* theBoard);

private:
    Human* humanPlayer;
    CPU* cpuPlayer;
    Board* gameBoard;
    bool gameStatus;
    bool humanTurn;

    //Asks player if he wanst to play first
    bool playFirst();

    //Player occupies both of opponent's castles
    bool winCondition1(Board* theBoard, const std::vector< std::pair<int, int> >& oppenentCastle, std::string& teamColor);

    //Player captures all of opponent's piece while retaining at least 2 pieces
    bool winCondition2(std::vector<Player::Piece>& myPieces, std::vector<Player::Piece>& enemyPieces);

    //Both players have 1 or less piece
    bool drawCondition(std::vector<Player::Piece>& myPieces, std::vector<Player::Piece>& enemyPieces);
};
#endif /* Game_h */
