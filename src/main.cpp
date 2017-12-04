#include<iostream>
#include<string>

#include "Board.h"
#include "Human.h"
using namespace std;

//Ask human player if he wants to first
bool play_first(){
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

int main(){
    //Set color for each player
    string HUMAN_TEAM = "WHITE";
    string CPU_TEAM = "BLACK";

    //Initialize game
    Board game_board = Board();
    Human human = Human(HUMAN_TEAM);
    CPU cpu = CPU(CPU_TEAM);
    //Assign enemy to each players
    human.setEnemy(&cpu);
    human.setBoard(&game_board);
    cpu.setEnemy(&human);

    //Update board
    game_board.updateBoard(&human, &cpu);

    //Start game
    bool game_running = true;
    bool human_turn = play_first();
    game_board.printBoard();

    //Game loop
    while(game_running){
        if(human_turn){ //If it is human's turn
            cout << "Human turn:" << endl;
            human.move();
            human_turn = false;
        }
        else{           //If it is CPU's turn
            cout << "CPU turn:" << endl;
            cpu.move();
            human_turn = true;
        }
        cout << endl;
        game_board.updateBoard(&human, &cpu);
        game_board.printBoard();
        cout << endl;
        //CHECK WINNING FUNCTION
    }
}
