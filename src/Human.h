#ifndef Human_h
#define Human_h

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<utility>

#include "Player.h"
#include "Board.h"

class Board;

class Human : protected Player {
private:
    void initialize_pieces();
    Board* board;
    enum PositionValues {Border_Value, CPU_Value, Human_Value, Empty_Value, Error_Value};
    
public:
    Human(std::string& teamColor, Board* theBoard);
    
    void move();
    
    //Getter for pieces vector
    std::vector<Piece>& getPieces();
    
    typedef std::map<int, std::vector<std::pair<int, int> > > valid_moves;
    typedef valid_moves::iterator validItr;

    //Find all valid positions for plain, cantering, and capturing move for a specific piece
    void checkValidity(Piece thePiece, valid_moves* capturingList, valid_moves* canteringList, valid_moves* plainList);

    //Checks what value is store a position
    PositionValues checkPositionValue(std::string& posVal);

    //Add the current row and col values into the passed in list
    void addMovesToList(valid_moves* theList, Piece thePiece, int rowVal, int colVal);

    //Find possible value of the jump position
    int checkJumpAdjacent(int nVal, int originVal);

    //Print possible moves based on the parameter
    void printMoveChoices(valid_moves* theList);

    //Human performs where to move which piece to which position
    void performMove(valid_moves* theList);

    //Human performs capturing move
};

#endif /* Human_h */
