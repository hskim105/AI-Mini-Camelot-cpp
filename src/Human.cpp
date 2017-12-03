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

void Human::checkValidity(Player::Piece thePiece, valid_moves* capturingList, valid_moves* canteringList, valid_moves* plainList){
    //Loop through adjacent 9 position, having the current row, col as the center
    //rowVal is +/- 1 from the piece's row value
    for(size_t rowVal = thePiece.row - 1; rowVal <= thePiece.row + 1; rowVal++){
        //colVal is +/- 1 from the piece's column value
        for(size_t colVal = thePiece.column - 1; colVal <= thePiece.column + 1; colVal++){
            //Do nothing if (rowVal, colVal) is same as (row, column)
            if(rowVal == thePiece.row && colVal == thePiece.column){
                ;   //Do nothing
            }
            else{   //Check all possible moves for the rest of the 8 adjacent positions

                cout << rowVal << ',' << colVal << endl;    //TODO: Debug. Remove later

                //Retrieve the game board as a pointer
                vector< vector<string> >* gameBoard = board->getBoard();

                //Value of the board position at (rowVal, colVal)
                string posVal = (*gameBoard)[rowVal][colVal];

                //Get position type (from enum) of the value at (rowVal, colVal)
                int positionType = checkPositionValue(posVal);

                //According to the position type, perform a specific function
                switch (positionType){
                    case Border_Value:{
                        //Invalid position (Border) Do nothing
                        break;
                    }
                    case CPU_Value:{
                        //Check conditions for capturing move
                        cout << "CPU piece" << endl;    //TODO: Debug. Remove later

                        //Get coordinates if piece were to perform a jump (jumpRow, jumpCol)
                        int jumpRow = checkJumpAdjacent(rowVal, thePiece.row);
                        int jumpCol = checkJumpAdjacent(colVal, thePiece.column);

                        //Get the value at the position (jumpRow, jumpCol)
                        string jumpVal = (*gameBoard)[jumpRow][jumpCol];
                        //Get position type (from enum) of the value at (jumpRow, jumpCol)
                        int jumpPositionType = checkPositionValue(jumpVal);

                        //Only add (jumpRow, jumpCol) to the capturing list if the position value is empty
                        if(jumpPositionType == Empty_Value){
                            addMovesToList(capturingList, thePiece, jumpRow, jumpCol);
                        }
                        break;
                    }
                    case Human_Value:{
                        //Check conditions for cantering move
                        cout << "Human piece" << endl;  //TODO: Debug. Remove later

                        //Get coordinates if piece were to perform a jump (jumpRow, jumpCol)
                        int jumpRow = checkJumpAdjacent(rowVal, thePiece.row);
                        int jumpCol = checkJumpAdjacent(colVal, thePiece.column);

                        //Get the value at the position (jumpRow, jumpCol)
                        string jumpVal = (*gameBoard)[jumpRow][jumpCol];
                        //Get position type (from enum) of the value at (jumpRow, jumpCol)
                        int jumpPositionType = checkPositionValue(jumpVal);

                        //Only add (jumpRow, jumpCol) to the cantering list if the position value is empty
                        if(jumpPositionType == Empty_Value){
                            addMovesToList(canteringList, thePiece, jumpRow, jumpCol);
                        }
                        break;
                    }
                    case Empty_Value:{
                        cout << "Empty" << endl;    //TODO: Debug. Remove later
                        //Add (rowVal, colVal) to the plain list
                        addMovesToList(plainList, thePiece, rowVal, colVal);
                        break;
                    }
                    case Error_Value:{
                        //Error (Should never get this)
                        cout << "Error" << endl;    //TODO: Debug. Remove later
                        break;
                    }
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

void Human::addMovesToList(valid_moves* theList, Piece thePiece, int rowVal, int colVal){
    validItr listItr = theList->find(thePiece.number);
    //if the map key already exists, just add the current (rowVal, colVal) to the vector
    if(listItr != theList->end()){
        (*theList)[thePiece.number].push_back({rowVal,colVal});
    }
    else{   //Key doesn't exist, create a new key and add the current (rowVal, colVal) to the vector
        theList->insert(pair<int, vector< pair<int, int> > >(thePiece.number, {{rowVal, colVal}}));
    }
}

int Human::checkJumpAdjacent(int nVal, int originVal){
    //jumpVal will either be +/- 1 from the nVal
    int jumpVal = nVal;

    //Difference is used to check if the jumpVal should be moved badk (-1) or moved forward (+1)
    int nDiff = nVal - originVal;

    if(nDiff < 0){
        jumpVal--;
    }
    else if(nDiff > 0){
        jumpVal++;
    }
    else{
        ;   //Do nothing
    }
    return jumpVal;
}
