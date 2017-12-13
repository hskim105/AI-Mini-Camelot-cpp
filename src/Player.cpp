#include "Player.h"
using namespace std;

vector<Player::Piece>& Player::getPieces(){
    //Return the vector of pieces by reference
    return pieces;
}

Player::Piece Player::createPiece(int nNumber, string& theTeam, int nRow, int nColumn){
    //Assign data from parameters to a piece's attributes
    Piece tempPiece;
    tempPiece.number = nNumber;
    tempPiece.team = theTeam;
    tempPiece.row = nRow;
    tempPiece.column = nColumn;
    return tempPiece;
}
