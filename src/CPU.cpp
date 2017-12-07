#include "CPU.h"
using namespace std;

CPU::CPU(string& teamColor) : enemy(nullptr), board(nullptr), game(nullptr), alpha(-1000), beta(1000){
    //Assign team color (CPU should be BLACK)
    color = teamColor;
    
    //Initialize Human initial_position
    initial_position.insert(pair<int, vector<int> >(4, {2,3,4,5}));
    initial_position.insert(pair<int, vector<int> >(5, {3,4}));
    
    //Initialize pieces
    initialize_pieces();
}

void CPU::setEnemy(Human* theEnemy){
    enemy = theEnemy;
}

void CPU::setBoard(Board* theBoard){
    board = theBoard;
}

void CPU::setGame(Game* theGame){
    game = theGame;
}

void CPU::move(){
    cout << "CPU move" << endl;
}

string& CPU::getTeamColor(){
    return color;
}

const vector< pair<int, int> >& CPU::getCastles(){
    return castles;
}
vector<Player::Piece>& CPU::getPieces(){
    return pieces;
}

const vector< pair<int, int> > CPU::castles = {{0, 3}, {0, 4}};

void CPU::initialize_pieces(){
    int nCount = 0;
    for(const auto &nKey : initial_position){
        for(const auto nVal : nKey.second){
            pieces.push_back(createPiece(nCount, color, nKey.first, nVal));
            nCount++;
        }
    }
}

void CPU::alphaBeta(Board* theBoard){
    //Assign result of the MAX-Value function to v
    int v = maxValue(theBoard, alpha, beta);

}

int CPU::maxValue(Board* theBoard, int nAlpha, int nBeta){
    //If terminal state, return the utility value of the state
    //If cutoff state, then return eval(state)
    int localV = -1000;

    return localV;
}

int CPU::minValue(Board* theBoard, int nAlpha, int nBeta){
    //If terminal state, return the utility value of the state
    //If cutoff state, then return eval(state)
    int localV = 1000;

    return localV;
}
