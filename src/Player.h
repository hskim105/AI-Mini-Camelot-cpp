#ifndef Player_h
#define Player_h

#include<iostream>
#include<string>
#include<vector>
#include<map>

class Player{
public:
    //Struct that will represent a piece on the board
    struct Piece{
        int number;
        std::string team;
        int row;
        int column;
    };
    
    virtual void move() = 0;

    //Getter for the vector of pieces
    virtual std::vector<Piece>& getPieces();
    
protected:
    std::string color;          //Team color
    std::vector<Piece> pieces;  //Vector of pieces (the total number of pieces each player gets)
    std::map<int, std::vector<int> > initial_position; //Initial starting positions of the pieces
    
    Piece createPiece(int nNumber, std::string& theTeam, int nRow, int nColumn); //Create a piece with data from the parameters

};

#endif /* Player_h */
