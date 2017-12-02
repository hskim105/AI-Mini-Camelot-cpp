#include "Human.h"
using namespace std;

Human::Human(string& teamColor, Board* theBoard) : board(theBoard){
    //Assign team color (Human should be WHITE)
    color = teamColor;
    
    //Initialize Human initial_position
    initial_position.insert(pair<int, vector<int> >(8, {3,4}));
    initial_position.insert(pair<int, vector<int> >(9, {2,3,4,5}));
    
    //Initialize pieces
    initialize_pieces();
}

void Human::move(){
    cout << "Human move" << endl;
    //Check if capturing move is there
    //if so, human must perform a capturing move
    //else, human can choose either cantering or plain move
    plainMove();
    
}

vector<Player::Piece>& Human::getPieces(){
    return pieces;
}

Human::valid_moves Human::plainMove(){
    valid_moves possibleMoves;
    for(size_t human_piece = 0; human_piece < pieces.size(); human_piece++){
        checkAdjacent(&possibleMoves, pieces[human_piece]);
    }
    return possibleMoves;
}

void Human::checkAdjacent(Human::valid_moves* vmList, Player::Piece thePiece){
    for(size_t rowVal = thePiece.row - 1; rowVal <= thePiece.row + 1; rowVal++){
        for(size_t colVal = thePiece.column - 1; colVal <= thePiece.column + 1; colVal++){
            if(rowVal == thePiece.row && colVal == thePiece.column){
                ;
            }
            else{
                cout << rowVal << ',' << colVal << endl;
                string posVal = board->getBoard()[rowVal][colVal];
                if(posVal == "XX"){
                    cout << "Invalid position. (Border)" << endl;
                }
                else if(posVal[0] == 'W'){
                    cout << "Human piece" << endl;
                }
                else if(posVal[0] == 'B'){
                    cout << "CPU piece" << endl;
                }
                else if (posVal == "__"){
                    cout << "Empty" << endl;
                    //Add to vmList
                }
                else{
                    cout << "Error" << endl;
                }
            }
        }
    }
    cout << endl;
}

void Human::initialize_pieces(){
    int nCount = 0;
    for(const auto &nKey : initial_position){
        for(const auto nVal : nKey.second){
            pieces.push_back(createPiece(nCount, color, nKey.first, nVal));
            nCount++;
        }
    }
}
