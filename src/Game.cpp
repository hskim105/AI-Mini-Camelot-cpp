#include "Game.h"
using namespace std;

Game::Game(Human* theHuman, CPU* theCPU, Board* theBoard) : humanPlayer(theHuman), cpuPlayer(theCPU), gameBoard(theBoard), gameStatus(true) {
    //Assign enemy to each players
    theHuman->setEnemy(theCPU);
    theCPU->setEnemy(theHuman);

    //Assign board to both players
    theHuman->setBoard(theBoard);
    theCPU->setBoard(theBoard);

    //Set game to both players
    theHuman->setGame(this);
    theCPU->setGame(this);

    //Update board
    theBoard->updateBoard(theHuman->getPieces(), theCPU->getPieces());
    welcomeMessage();
    //Print board
    theBoard->printBoard();

    humanTurn = playFirst();
}

void Game::gameLoop(){
    while(gameStatus){
        if(humanTurn){ //If it is human's turn
            cout << "Human turn:" << endl;
            //Human performs a move
            humanPlayer->move();
            humanTurn = false;
        }
        else{          //If it is CPU's turn
            cout << "CPU turn:" << endl;
            //CPU performs a move
            cpuPlayer->move();
            humanTurn = true;
        }
        cout << endl;
        //Update the game board and print it
        gameBoard->updateBoard(humanPlayer->getPieces(), cpuPlayer->getPieces());
        gameBoard->printBoard();
        cout << endl;

        //Check game status by using the checkWin function
        WinValue winValue = checkWin(gameBoard, humanPlayer->getPieces(), cpuPlayer->getPieces());
        if(winValue != OnGoing){
            if(winValue == HumanWin){
                cout << "Human won!" << endl;
            }
            else if(winValue == CPUWin){
                cout << "CPU won!" << endl;
            }
            else if (winValue == Draw){
                cout << "Draw!" << endl;
            }
            gameStatus = false;
        }
    }
}

Game::WinValue Game::checkWin(Board* theBoard, vector<Player::Piece> humanPieces, vector<Player::Piece> cpuPieces){
    //Check conditions for human or cpu to win or draw conditions
    bool humanWinCondition1 = winCondition1(theBoard, cpuPlayer->getCastles(), humanPlayer->getTeamColor());
    bool humanWinCondition2 = winCondition2(humanPieces, cpuPieces);
    bool cpuWinCondition1 = winCondition1(theBoard, humanPlayer->getCastles(), cpuPlayer->getTeamColor());
    bool cpuWinCondition2 = winCondition2(cpuPieces, humanPieces);
    bool theDrawCondition = drawCondition(humanPieces, cpuPieces);

    //Check if human beat cpu
    if(humanWinCondition1 || humanWinCondition2){
        return HumanWin;
    }
    //Check if cpu beat human
    else if (cpuWinCondition1 || cpuWinCondition2){
        return CPUWin;
    }
    //Check if there is a draw
    else if (theDrawCondition){
        return Draw;
    }
    else{
        //Continue playing
        return OnGoing;
    }
}

void Game::checkValidity(Board* theBoard, Player::Piece thePiece, valid_moves* capturingList, valid_moves* canteringList, valid_moves* plainList){
    //Loop through adjacent 9 position, having the current row, col as the center
    //rowVal is +/- 1 from the piece's row value
    //if rowVal is less than 0, set rowVal = 0 since there cannot be negative coordinate on the board
    int tempRow = thePiece.row - 1;
    for(size_t rowVal = (tempRow<0) ? 0 : tempRow; rowVal <= thePiece.row + 1; rowVal++){
        //colVal is +/- 1 from the piece's column value
        int tempCol = thePiece.column - 1;
        //if colVal is less than 0, then set colVal = 0 since there cannot be negative coordinate on the board
        for(size_t colVal = (tempCol < 0) ? 0 : tempCol; colVal <= thePiece.column + 1; colVal++){
            //Do nothing if (rowVal, colVal) is same as (row, column)
            if((rowVal == thePiece.row && colVal == thePiece.column) || rowVal >= theBoard->getMaxRow() || colVal >= theBoard->getMaxCol()){
                ;   //Do nothing
            }
            else{   //Check all possible moves for the rest of the 8 adjacent positions
                //Get position type (from enum) of the value at (rowVal, colVal)
                int positionType = theBoard->checkPositionValue(rowVal, colVal);

                //According to the position type, perform a specific function
                switch (positionType){
                    case Board::Border_Value:{
                        //Invalid position (Border) Do nothing
                        break;
                    }
                    case Board::CPU_Value:{
                        //Get coordinates if piece were to perform a jump (jumpRow, jumpCol)
                        int jumpRow = checkJumpAdjacentVal(rowVal, thePiece.row);
                        int jumpCol = checkJumpAdjacentVal(colVal, thePiece.column);

                        //If jumpRow and jumpCol are within the board bounds [0, MAX_ROW), [0, MAX_COL)
                        if(jumpRow >= 0 && jumpRow < theBoard->getMaxRow() && jumpCol >= 0 && jumpCol < theBoard->getMaxCol()){
                            //Get position type (from enum) of the value at (jumpRow, jumpCol)
                            int jumpPositionType = theBoard->checkPositionValue(jumpRow, jumpCol);
                            //Only add (jumpRow, jumpCol) to the list if the position value is empty
                            if(jumpPositionType == Board::Empty_Value){
                                //For Human, add to capturing list
                                if(thePiece.team[0] == 'W'){
                                    addMovesToList(capturingList, thePiece, jumpRow, jumpCol);
                                }
                                //For CPU, add to cantering list
                                else if(thePiece.team[0] == 'B'){
                                    addMovesToList(canteringList, thePiece, jumpRow, jumpCol);
                                }
                            }
                        }
                        break;
                    }
                    case Board::Human_Value:{
                        //Get coordinates if piece were to perform a jump (jumpRow, jumpCol)
                        int jumpRow = checkJumpAdjacentVal(rowVal, thePiece.row);
                        int jumpCol = checkJumpAdjacentVal(colVal, thePiece.column);

                        //If jumpRow and jumpCol are within the board bounds [0, MAX_ROW), [0, MAX_COL)
                        if(jumpRow >= 0 && jumpRow < theBoard->getMaxRow() && jumpCol >= 0 && jumpCol < theBoard->getMaxCol()){
                            //Get position type (from enum) of the value at (jumpRow, jumpCol)
                            int jumpPositionType = theBoard->checkPositionValue(jumpRow, jumpCol);

                            //Only add (jumpRow, jumpCol) to the list if the position value is empty
                            if(jumpPositionType == Board::Empty_Value){
                                //For Human, add to cantering list
                                if(thePiece.team[0] == 'W'){
                                    addMovesToList(canteringList, thePiece, jumpRow, jumpCol);
                                }
                                //For CPU, add to capturing list
                                else if(thePiece.team[0] == 'B'){
                                    addMovesToList(capturingList, thePiece, jumpRow, jumpCol);
                                }
                            }
                        }
                        break;
                    }
                    case Board::Empty_Value:{
                        //Add (rowVal, colVal) to the plain list
                        addMovesToList(plainList, thePiece, rowVal, colVal);
                        break;
                    }
                    case Board::Error_Value:{
                        //Error (Should never get this)
                        cout << "Error: Out of bounds" << endl;
                        break;
                    }
                }
            }
        }
    }
}

void Game::addMovesToList(valid_moves* theList, Player::Piece thePiece, int rowVal, int colVal){
    validItr listItr = theList->find(thePiece.number);
    //if the map key already exists, just add the current (rowVal, colVal) to the vector
    if(listItr != theList->end()){
        (*theList)[thePiece.number].push_back({rowVal,colVal});
    }
    else{   //Key doesn't exist, create a new key and add the current (rowVal, colVal) to the vector
        theList->insert(pair<int, vector< pair<int, int> > >(thePiece.number, {{rowVal, colVal}}));
    }
}

int Game::checkJumpAdjacentVal(int nVal, int originVal){
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

int Game::findBetweenVal(int firstVal, int secondVal){
    //betweenVal will either be +/- 1 from the nVal
    int betweenVal = firstVal;

    //Difference is used to check if the betweenVal should be moved back (-1) or moved forward (+1)
    int nDiff = firstVal - secondVal;

    if(nDiff < 0){
        betweenVal++;
    }
    else if(nDiff > 0){
        betweenVal--;
    }
    else{
        ;   //Do nothing
    }
    return betweenVal;
}

void Game::printMoveChoices(valid_moves* theList, string& teamColor){
    //Loop through the list and show the piece number and all its possible moves
    for(validItr listItr = theList->begin(); listItr != theList->end(); listItr++){
        cout << "Choices for " << teamColor[0] << listItr->first << endl;   //Show the piece number
        for(vector<pair<int,int> >::iterator pairItr = listItr->second.begin(); pairItr != listItr->second.end(); pairItr++){
            //Show the piece's all possible moves
            cout << '(' << pairItr->first << ',' << pairItr->second << ')' << endl;
        }
        cout << endl;
    }
}



Game::vecPieceItr Game::findPiece(vecPieceItr startItr, vecPieceItr endItr, int theNumber){
    //Find a piece in the vector of pieces by its number
    for(vecPieceItr pieceItr = startItr; pieceItr != endItr; pieceItr++){
        if(pieceItr->number == theNumber){
            return pieceItr;
        }
    }
    return endItr;
}

Game::vecPieceItr Game::findPiece(vecPieceItr startItr, vecPieceItr endItr, int theRow, int theCol){
    //Find a piece in the vector of pieces by its row, col
    for(vecPieceItr pieceItr = startItr; pieceItr != endItr; pieceItr++){
        if(pieceItr->row == theRow && pieceItr->column == theCol){
            return pieceItr;
        }
    }
    return endItr;
}

bool Game::playFirst(){
    //Store the response of the input
    string response;
    while(1){
        cout << "Would you like to play first? (Y/N): ";
        cin >> response;
        switch (toupper(response[0])){
            case 'Y':
                return true;
            case 'N':
                return false;
            default:
                cout << "Choose either 'Y' or 'N'." << endl;
                break;
        }

        //Ignore leftover inputs in the stream
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

bool Game::winCondition1(Board* theBoard, const vector< pair<int, int> >& oppenentCastle, string& teamColor){
    int trueCounter = 0;
    //Check if both of the castles are occupied
    for(size_t nIndex = 0; nIndex < oppenentCastle.size(); nIndex++){
        int castleRow = oppenentCastle[nIndex].first;
        int castleCol = oppenentCastle[nIndex].second;

        Board::PositionValues castleVal = theBoard->checkPositionValue(castleRow, castleCol);
        char teamColorVal = teamColor[0];

        if( castleVal == teamColorVal){
            trueCounter++;
        }
    }
    return (trueCounter == oppenentCastle.size()) ? true : false;
}

bool Game::winCondition2(vector<Player::Piece>& myPieces, vector<Player::Piece>& enemyPieces){
    //Check if player has at least two pieces and wiped out opponent's pieces
    return (myPieces.size() >= 2 && enemyPieces.size() == 0) ? true : false;
}

bool Game::drawCondition(vector<Player::Piece>& myPieces, vector<Player::Piece>& enemyPieces){
    //Check if both players have 1 piece left
    return (myPieces.size() <= 1 && enemyPieces.size() <= 1) ? true : false;
}

void Game::welcomeMessage(){
    //Welcome banner
    cout << "============================================" << endl;
    cout << "                Mini Camelot                " << endl;
    cout << "============================================" << endl;
    cout << endl;
}

