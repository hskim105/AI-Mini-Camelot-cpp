#ifndef Game_h
#define Game_h

#include<iostream>

#include "Player.h"
#include "Human.h"
#include "CPU.h"
#include "Board.h"

class Human;
class CPU;
class Board;

class Game{

public:
    typedef std::map<int, std::vector<std::pair<int, int> > > valid_moves;
    typedef valid_moves::iterator validItr;
    typedef std::vector<Player::Piece>::iterator vecPieceItr;

    enum WinValue {
        HumanWin = -1000,
        Draw = 0,
        CPUWin = 1000,
        OnGoing
    };

    //Default constructor
    //When creating a new game object, you automatically start the game
    Game(Human* theHuman, CPU* theCPU, Board* theBoard);

    //Game loop will handle the actual play of the game
    void gameLoop();

    //Checks if a player won
    WinValue checkWin(Board* theBoard, std::vector<Player::Piece> humanPieces, std::vector<Player::Piece> cpuPieces);


    /////////////////////////
    //Find all valid positions for plain, cantering, and capturing move for a specific piece
    void checkValidity(Board* theBoard, Player::Piece thePiece, valid_moves* capturingList, valid_moves* canteringList, valid_moves* plainList);

    //Add the current row and col values into the passed in list
    void addMovesToList(valid_moves* theList, Player::Piece thePiece, int rowVal, int colVal);

    //Find possible value of the jump position
    int checkJumpAdjacentVal(int nVal, int originVal);

    //Find value between two positions
    int findBetweenVal(int firstVal, int secondVal);

    //Print possible moves based on the parameter
    void printMoveChoices(valid_moves* theList, std::string& teamColor);

    //Find a piece that has a specific number in the vector<Piece>
    vecPieceItr findPiece(vecPieceItr startItr, vecPieceItr endItr, int theNumber);
    //Find a piece that has a specific (row,col) in the vector<Piece>
    vecPieceItr findPiece(vecPieceItr startItr, vecPieceItr endItr, int theRow, int theCol);

    //////////////////////////////
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

    void welcomeMessage();
};
#endif /* Game_h */
