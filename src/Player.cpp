#include "Player.h"
using namespace std;

vector<Player::Piece>& Player::getPieces(){
    return pieces;
}

Player::Piece Player::createPiece(int nNumber, string& theTeam, int nRow, int nColumn){
    Piece tempPiece;
    tempPiece.number = nNumber;
    tempPiece.team = theTeam;
    tempPiece.row = nRow;
    tempPiece.column = nColumn;
    return tempPiece;
}
