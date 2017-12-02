#include "Human.h"
using namespace std;

void Human::initialize_pieces(){
    int nCount = 0;
    for(const auto &nKey : initial_position){
        for(const auto nVal : nKey.second){
            pieces.push_back(createPiece(nCount, color, nKey.first, nVal));
            nCount++;
        }
    }
}

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
    valid_moves valid_plain;
    valid_moves valid_cantering;
    valid_moves valid_capturing;
    
    //Loop through each piece and check all valid positions for that piece
    for(size_t human_piece = 0; human_piece < pieces.size(); human_piece++){
        checkValidity(pieces[human_piece], &valid_capturing, &valid_cantering, &valid_plain);
    }
    //Check if capturing move is there
    //if so, human must perform a capturing move
    //else, human can choose either cantering or plain move
    
}

vector<Player::Piece>& Human::getPieces(){
    return pieces;
}

void Human::checkValidity(Player::Piece thePiece, Human::valid_moves* capturingList, Human::valid_moves* canteringList, Human::valid_moves* plainList){
    for(size_t rowVal = thePiece.row - 1; rowVal <= thePiece.row + 1; rowVal++){
        for(size_t colVal = thePiece.column - 1; colVal <= thePiece.column + 1; colVal++){
            if(rowVal == thePiece.row && colVal == thePiece.column){
                ;   //Do nothing
            }
            else{
                cout << rowVal << ',' << colVal << endl;
                //Value of the board position at (rowVal, colVal)
                vector< vector<string> >* gameBoard = board->getBoard();
                string posVal = (*gameBoard)[rowVal][colVal];
                
                int positionType = checkPositionValue(posVal);
                
                switch (positionType) {
                    case Border_Value:
                        //Invalid position (Border)
                        break;
                    case CPU_Value:
                        cout << "CPU piece" << endl;
                        //Do another check for capturing
                        break;
                    case Human_Value:
                        cout << "Human piece" << endl;
                        //Do another check for cantering
                        break;
                    case Empty_Value:
                        cout << "Empty" << endl;
                        //Add to plainList
                        break;
                    case Error_Value:
                        //Error
                        cout << "Error" << endl;
                        break;
                }
            }
        }
    }
    cout << endl;
}

Human::PositionValues Human::checkPositionValue(string& posVal){
    if(posVal == board->getBorderVal()){
        //Invalid position. (Border)
        return Border_Value;
    }
    else if(posVal[0] == 'B'){
        //CPU Piece
        return CPU_Value;
    }
    else if(posVal[0] == 'W'){
        //Human Piece
        return Human_Value;
    }
    else if (posVal == board->getEmptyVal()){
        //Empty position
        return Empty_Value;
    }
    else{
        //Error
        return Error_Value;
    }
}
