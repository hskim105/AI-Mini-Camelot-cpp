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

Human::Human(string& teamColor, Board* theBoard) : enemy(nullptr), board(theBoard){
    //Assign team color (Human should be WHITE)
    color = teamColor;
    
    //Initialize Human initial_position
    initial_position.insert(pair<int, vector<int> >(8, {3,4}));
    initial_position.insert(pair<int, vector<int> >(9, {2,3,4,5}));
    
    //Initialize pieces
    initialize_pieces();
}

void Human::setEnemy(CPU* theEnemy){
    enemy = theEnemy;
}

void Human::move(){
//    cout << "Human move" << endl;       //TODO: Debug. Remove later
    valid_moves valid_plain;
    valid_moves valid_cantering;
    valid_moves valid_capturing;
    
    //Loop through each piece and check all valid positions for that piece
    for(size_t human_piece = 0; human_piece < pieces.size(); human_piece++){
        checkValidity(pieces[human_piece], &valid_capturing, &valid_cantering, &valid_plain);
    }

    //Check if capturing move is available, since capturing move is mandatory
    if(valid_capturing.size() != 0){
        cout << "Capturing move available. You must perform this action." << endl;
        cout << "Here are the possible capturing moves:" << endl;
        printMoveChoices(&valid_capturing);
    }
    else{   //Choose either cantering or plain move
        string move_response;

        while(1){
            cout << "Enter 'C' for cantering move or enter 'P' for plain move: ";
            cin >> move_response;
            if(toupper(move_response[0]) == 'C'){
                cout << "Cantering move chosen" << endl;    //TODO: Debug. Remove later
                cout << "Here are the possible cantering moves:" << endl;
                printMoveChoices(&valid_cantering);
                performMove(&valid_cantering);
                break;
            }
            else if(toupper(move_response[0]) == 'P'){
                cout << "Plain move chosen" << endl;        //TODO: Debug. Remove later
                cout << "Here are the possible plain moves:" << endl;
                printMoveChoices(&valid_plain);
                performMove(&valid_plain);
                break;
            }
            else{
                cout << "Choose either 'C' or 'P'." << endl;
            }
        }
    }
    
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

//                cout << rowVal << ',' << colVal << endl;    //TODO: Debug. Remove later

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
//                        cout << "CPU piece" << endl;    //TODO: Debug. Remove later

                        //Get coordinates if piece were to perform a jump (jumpRow, jumpCol)
                        int jumpRow = checkJumpAdjacentVal(rowVal, thePiece.row);
                        int jumpCol = checkJumpAdjacentVal(colVal, thePiece.column);

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
//                        cout << "Human piece" << endl;  //TODO: Debug. Remove later

                        //Get coordinates if piece were to perform a jump (jumpRow, jumpCol)
                        int jumpRow = checkJumpAdjacentVal(rowVal, thePiece.row);
                        int jumpCol = checkJumpAdjacentVal(colVal, thePiece.column);

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
//                        cout << "Empty" << endl;    //TODO: Debug. Remove later
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
//    cout << endl;   //TODO: Debug. Remove later
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

int Human::checkJumpAdjacentVal(int nVal, int originVal){
    //jumpVal will either be +/- 1 from the nVal
    int jumpVal = nVal;

    //Difference is used to check if the jumpVal should be moved back (-1) or moved forward (+1)
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

void Human::printMoveChoices(valid_moves* theList){
    for(validItr listItr = theList->begin(); listItr != theList->end(); listItr++){
        cout << "Choices for W" << listItr->first << endl;
        for(vector<pair<int,int> >::iterator pairItr = listItr->second.begin(); pairItr != listItr->second.end(); pairItr++){
            cout << '(' << pairItr->first << ',' << pairItr->second << ')' << endl;
        }
        cout << endl;
    }
}

void Human::chooseMove(valid_moves* theList, int& chosenPiece, int& chosenRow, int& chosenCol){
    while(1){
        cout << "Please choose a piece: W";
        //Check if input type is correct
        while(!(cin >> chosenPiece)){
            cin.clear();
            cout << "Wrong input type. Input a number!" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please choose a piece: W";
        }

        //Check if chosenPiece is inside possible moves list
        validItr chosenItr = theList->end();
        for(validItr listItr = theList->begin(); listItr != theList->end(); listItr++){
            if(listItr->first == chosenPiece){
                chosenItr = listItr;
                break;
            }
        }

        //Chosen number is valid
        if(chosenItr != theList->end()){
            cout << "Please enter row, column: ";
            while(!(cin >> chosenRow >> chosenCol)){
                cin.clear();
                cout << "Wrong input type. Input a number!" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter row, column: ";
            }
            //Correct input type
            //Find if row, column match the ones inside the possible positions
            vector<pair<int, int> >::iterator chosenPairItr = chosenItr->second.end();

            for(vector<pair<int, int> >::iterator pairItr = chosenItr->second.begin(); pairItr != chosenItr->second.end(); pairItr++){
                if(pairItr->first == chosenRow && pairItr->second == chosenCol){
                    chosenPairItr = pairItr;
                    break;
                }
            }

            if(chosenPairItr != chosenItr->second.end()){
                break;
            }
            else{   //Choosen a invalid (row,column)
                cout << "Invalid (row, column). Restarting pick." << endl;
            }
        }
        else{   //Chosen number not a valid piece
            cout << "Choose a valid number for the piece" << endl;
        }
    }
}

void Human::performMove(valid_moves* theList){
    int chosenPiece, chosenRow, chosenCol;

    //Choose a move
    chooseMove(theList, chosenPiece, chosenRow, chosenCol);

    vecPieceItr pieceItr = findPieceNumber(pieces.begin(), pieces.end(), chosenPiece);
    if(pieceItr != pieces.end()){
        //Set old position as empty
        board->setPosition(pieceItr->row, pieceItr->column, board->getEmptyVal());
        pieceItr->row = chosenRow;
        pieceItr->column = chosenCol;
    }
}

void Human::performCapture(valid_moves* theList){
    int chosenPiece, chosenRow, chosenCol;

    //Choose a move
    chooseMove(theList, chosenPiece, chosenRow, chosenCol);

    vecPieceItr pieceItr = findPieceNumber(pieces.begin(), pieces.end(), chosenPiece);
    if(pieceItr != pieces.end()){
        //Delete the captured piece
        
        //Set old position as empty
        board->setPosition(pieceItr->row, pieceItr->column, board->getEmptyVal());
        pieceItr->row = chosenRow;
        pieceItr->column = chosenCol;
    }
}

Human::vecPieceItr Human::findPieceNumber(vecPieceItr startItr, vecPieceItr endItr, int theNumber){
    for(vecPieceItr pieceItr = startItr; pieceItr != endItr; pieceItr++){
        if(pieceItr->number == theNumber){
            return pieceItr;
        }
    }
    return endItr;
}
