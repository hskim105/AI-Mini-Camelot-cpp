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
    theBoard->updateBoard(theHuman, theCPU);
    //Print board
    theBoard->printBoard();

    humanTurn = playFirst();
}

void Game::gameLoop(){
    while(gameStatus){
        if(humanTurn){ //If it is human's turn
            cout << "Human turn:" << endl;
            humanPlayer->move();
            humanTurn = false;
        }
        else{           //If it is CPU's turn
            cout << "CPU turn:" << endl;
            cpuPlayer->move();
            humanTurn = true;
        }
        cout << endl;
        gameBoard->updateBoard(humanPlayer, cpuPlayer);
        gameBoard->printBoard();
        cout << endl;

        //CHECK WINNING FUNCTION
        WinValue winValue = checkWin(humanPlayer, cpuPlayer, gameBoard);
        if(winValue != None){
            gameStatus = false;
        }
    }
}

Game::WinValue Game::checkWin(Human* theHuman, CPU* theCPU, Board* theBoard){

    bool humanWinCondition1 = winCondition1(theBoard, theCPU->getCastles(), theHuman->getTeamColor());
    bool humanWinCondition2 = winCondition2(theHuman->getPieces(), theCPU->getPieces());
    bool cpuWinCondition1 = winCondition1(theBoard, theHuman->getCastles(), theCPU->getTeamColor());
    bool cpuWinCondition2 = winCondition2(theCPU->getPieces(), theHuman->getPieces());
    bool theDrawCondition = drawCondition(theHuman->getPieces(), theCPU->getPieces());
    //Check if human beat cpu
    if(humanWinCondition1 || humanWinCondition2){
        cout << "Human won!" << endl;
        return HumanWin;
    }
    //Check if cpu beat human
    else if (cpuWinCondition1 || cpuWinCondition2){
        cout << "CPU won!" << endl;
        return CPUWin;
    }
    //Check if there is a draw
    else if (theDrawCondition){
        cout << "Draw!" << endl;
        return Draw;
    }
    else{
        //Continue playing
        return None;
    }
}

bool Game::playFirst(){
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
    return (myPieces.size() >= 2 && enemyPieces.size() == 0) ? true : false;
}

bool Game::drawCondition(vector<Player::Piece>& myPieces, vector<Player::Piece>& enemyPieces){
    return (myPieces.size() <= 1 && enemyPieces.size() <= 1) ? true : false;
}



