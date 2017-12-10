#ifndef Player_h
#define Player_h

#include<iostream>
#include<string>
#include<vector>
#include<map>

class Player{
public:
    struct Piece{
        int number;
        std::string team;
        int row;
        int column;
    };
    
    virtual void move() = 0;
    
    virtual std::vector<Piece>& getPieces();
    
protected:
    std::string color;
    std::vector<Piece> pieces;
    std::map<int, std::vector<int> > initial_position;
    
    Piece createPiece(int nNumber, std::string& theTeam, int nRow, int nColumn);

};

#endif /* Player_h */
