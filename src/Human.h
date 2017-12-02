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

    void checkValidity(Piece thePiece, valid_moves* capturingList, valid_moves* canteringList, valid_moves* plainList);

    PositionValues checkPositionValue(std::string& posVal);

    void addMovesToList(valid_moves* theList, Piece thePiece, int rowVal, int colVal);
};

#endif /* Human_h */
