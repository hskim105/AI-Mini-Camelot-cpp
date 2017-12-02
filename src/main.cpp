#include<iostream>
#include<string>

#include "Board.h"
#include "Human.h"
using namespace std;

bool play_first(){
    char response;
    while(1){
        cout << "Would you like to play first? (Y/N): ";
        cin >> response;
        switch (toupper(response)){
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
    string HUMAN_TEAM = "WHITE";
    string CPU_TEAM = "BLACK";
    
    //Initialize game
    Board game_board = Board();
    Human human = Human(HUMAN_TEAM, &game_board);
    CPU cpu = CPU(CPU_TEAM);

    game_board.updateBoard(&human, &cpu);
    
    //Start game
    bool game_running = true;
    bool human_turn = play_first();
    while(game_running){
        if(human_turn){
            cout << "Human turn" << endl;
            human.move();
            human_turn = false;
        }
        else{
            cout << "CPU turn" << endl;
            //CPU move
            human_turn = true;
        }
        game_board.printBoard();
    }
}
