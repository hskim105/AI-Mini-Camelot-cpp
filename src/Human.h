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
public:
    Human(std::string& teamColor, Board* theBoard);
    
    void move();
    
    //Getter for pieces vector
    std::vector<Piece>& getPieces();
    
    typedef std::map<int, std::vector<std::pair<int, int>>> valid_moves;
    
    valid_moves plainMove();
    valid_moves canteringMove();
    valid_moves capturingMove();
    
    void checkAdjacent(valid_moves* vmList, Piece thePiece);
    
private:
    void initialize_pieces();
    Board* board;
};

#endif /* Human_h */
