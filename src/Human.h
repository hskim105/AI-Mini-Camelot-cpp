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

class CPU;
class Board;

class Human : protected Player {
private:
    void initialize_pieces();
    CPU* enemy;
    Board* board;
    static const std::vector< std::pair<int, int> > castles;
    enum PositionValues {Border_Value, CPU_Value, Human_Value, Empty_Value, Error_Value};
    
public:
    typedef std::map<int, std::vector<std::pair<int, int> > > valid_moves;
    typedef valid_moves::iterator validItr;
    typedef std::vector<Piece>::iterator vecPieceItr;

    //Default constructor
    Human(std::string& teamColor);

    void setEnemy(CPU* theEnemy);

    void setBoard(Board* theBoard);

    void move();
    
    //Getter for pieces vector
    std::vector<Piece>& getPieces();

    //Getter for position of castles
    const std::vector< std::pair<int, int> >& getCastles();

    //Getter for team color
    std::string& getTeamColor();

    //Find all valid positions for plain, cantering, and capturing move for a specific piece
    void checkValidity(Piece thePiece, valid_moves* capturingList, valid_moves* canteringList, valid_moves* plainList);

    //Checks what value is store a position
    PositionValues checkPositionValue(std::string& posVal);

    //Add the current row and col values into the passed in list
    void addMovesToList(valid_moves* theList, Piece thePiece, int rowVal, int colVal);

    //Find possible value of the jump position
    int checkJumpAdjacentVal(int nVal, int originVal);

    //Find value between two positions
    int findBetweenVal(int firstVal, int secondVal);

    //Print possible moves based on the parameter
    void printMoveChoices(valid_moves* theList);

    //Human chooses which piece to move and which position to put
    void chooseMove(valid_moves* theList, int& chosenPiece, int& chosenRow, int& chosenCol);

    //Human performs where to move which piece to which position
    void performMove(valid_moves* theList);

    //Human performs capturing move
    void performCapture(valid_moves* theList);

    //Find a piece that has a specific number in the vector<Piece>
    vecPieceItr findPiece(vecPieceItr startItr, vecPieceItr endItr, int theNumber);
    //Find a piece that has a specific (row,col) in the vector<Piece>
    vecPieceItr findPiece(vecPieceItr startItr, vecPieceItr endItr, int theRow, int theCol);

};

#endif /* Human_h */
