#include "Game.h"
using namespace std;

namespace Game{

    bool startGame(Human* theHuman, CPU* theCPU, Board* theBoard){
        //Assign enemy to each players
        theHuman->setEnemy(theCPU);
        theHuman->setBoard(theBoard);
        theCPU->setEnemy(theHuman);

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
            //If someone run, set gameRunning to false
        }
    }
}
