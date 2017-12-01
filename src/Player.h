#ifndef Player_h
#define Player_h

#include<iostream>
#include<string>
#include<vector>
#include<map>

class Player{
private:
    struct Piece{
        int number;
        std::string team;
        int row;
        int column;
        bool captured;
    };
    
public:
    virtual void move() = 0;
    
protected:
    std::string color;
    std::vector<Piece> pieces;
    std::map<int, std::vector<int> > initial_position;
    
    Piece createPiece(int nNumber, std::string& theTeam, int nRow, int nColumn, bool isCaptured=0);
};

#endif /* Player_h */
