#ifndef Human_h
#define Human_h

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<utility>
#include<limits>

#include "Player.h"
#include "CPU.h"
#include "Board.h"
#include "Game.h"

//Forward declarations
class CPU;
class Board;
class Game;

class Human : protected Player {
private:
    void initialize_pieces();
    CPU* enemy;
    Board* board;
    Game* game;
    static const std::vector< std::pair<int, int> > castles;
    
public:
    typedef std::map<int, std::vector<std::pair<int, int> > > valid_moves;

    //Default constructor
    Human(std::string& teamColor);

    //Copy constructor
    Human(Human* theHuman);

    //Setter methods for...
    //Enemy
    void setEnemy(CPU* theEnemy);
    //Board
    void setBoard(Board* theBoard);
    //Game
    void setGame(Game* theGame);
    //Piece
    void setPieces(std::vector<Piece> newPieces);

    //Move method for performing a move
    void move();
    
    //Getter for pieces vector
    std::vector<Piece>& getPieces();

    //Getter for position of castles
    const std::vector< std::pair<int, int> >& getCastles();

    //Getter for team color
    std::string& getTeamColor();

    //Human chooses which piece to move and which position to put
    void chooseMove(valid_moves* theList, int& chosenPiece, int& chosenRow, int& chosenCol);

    //Human performs where to move which piece to which position
    void performMove(valid_moves* theList);

    //Human performs capturing move
    void performCapture(valid_moves* theList);
};

#endif /* Human_h */
