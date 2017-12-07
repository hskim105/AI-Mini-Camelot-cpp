#include "Game.h"
using namespace std;

namespace Game{

    bool startGame(Human* theHuman, CPU* theCPU, Board* theBoard){
        //Assign enemy to each players
        theHuman->setEnemy(theCPU);
        theCPU->setEnemy(theHuman);

        //Assign board to both players
        theHuman->setBoard(theBoard);
        theCPU->setBoard(theBoard);

        //Update board
        theBoard->updateBoard(theHuman, theCPU);
        //Print board
        theBoard->printBoard();

        //Start the game after all the prep work is done
        return true;
    }

    bool playFirst(){
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

    void gameLoop(Human* theHuman, CPU* theCPU, Board* theBoard, bool gameRunning, bool humanTurn){
        while(gameRunning){
            if(humanTurn){ //If it is human's turn
                cout << "Human turn:" << endl;
                theHuman->move();
                humanTurn = false;
            }
            else{           //If it is CPU's turn
                cout << "CPU turn:" << endl;
                theCPU->move();
                humanTurn = true;
            }
            cout << endl;
            theBoard->updateBoard(theHuman, theCPU);
            theBoard->printBoard();
            cout << endl;
            //CHECK WINNING FUNCTION
            WinValue winValue = checkWin(theHuman, theCPU, theBoard);
            if(winValue != None){
                gameRunning = false;
            }
        }
    }

    WinValue checkWin(Human* theHuman, CPU* theCPU, Board* theBoard){

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

    bool winCondition1(Board* theBoard, const vector< pair<int, int> >& oppenentCastle, string& teamColor){
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

    bool winCondition2(vector<Player::Piece>& myPieces, vector<Player::Piece>& enemyPieces){
        return (myPieces.size() >= 2 && enemyPieces.size() == 0) ? true : false;
    }

    bool drawCondition(vector<Player::Piece>& myPieces, vector<Player::Piece>& enemyPieces){
        return (myPieces.size() <= 1 && enemyPieces.size() <= 1) ? true : false;
    }
}
