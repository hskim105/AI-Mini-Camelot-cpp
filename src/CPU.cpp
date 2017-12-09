#include "CPU.h"
using namespace std;

CPU::CPU(string& teamColor) : enemy(nullptr), board(nullptr), game(nullptr), ALPHA_VAL(-1000), BETA_VAL(1000) {
    //Assign team color (CPU should be BLACK)
    color = teamColor;
    
    //Initialize Human initial_position
    initial_position.insert(pair<int, vector<int> >(4, {2,3,4,5}));
    initial_position.insert(pair<int, vector<int> >(5, {3,4}));
    
    //Initialize pieces
    initialize_pieces();
}

CPU::CPU(CPU* theCPU) : enemy(nullptr), board(nullptr), game(nullptr), ALPHA_VAL(-1000), BETA_VAL(1000){
    //Copy color
    this->color = theCPU->color;
    //Copy pieces
    for(size_t nIndex = 0; nIndex < theCPU->pieces.size(); nIndex++){
        this->pieces.push_back(theCPU->pieces[nIndex]);
    }
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
    alphaBeta(board);
//    bool startPrune = true;
//    time_t startTime = time(nullptr);
//    time_t endTime = time(nullptr);
//    while(difftime(endTime, startTime) <= TIME_LIMIT){
//        if(startPrune){
//            cout << difftime(endTime, startTime) << endl;
//            startPrune = false;
//        }
//
//        time(&endTime);
//    }
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
const int CPU::TIME_LIMIT = 10;

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
    Node* rootNode = new Node(theBoard, ALPHA_VAL, BETA_VAL, 0, pieces, enemy->getPieces());
    //Assign result of the MAX-Value function to v
    int v = maxValue(rootNode);

}

int CPU::maxValue(Node* theNode){
    //If terminal state, return the utility value of the state
    int utilVal = game->checkWin(theNode->gameBoard, theNode->humanPieces, theNode->cpuPieces);
    if(utilVal != Game::None){
        return utilVal;
    }
    //If cutoff state, then return eval(state)
    int localV = -1000;

    //TODO: CHECK VALIDITY OF CPU. FOR ALL VALUES IN  ALL THE LIST, CREATE A NEW NODE THAT WILL STORE THE STATE OF THE BOARD AND DEPTH, AND PIECES

    //possibleActions[0] = capturing moves
    //possibleActions[1] = cantering moves
    //possibleActions[2] = plain moves
    vector<valid_moves> possibleActions {3};

    for(size_t cpu_piece = 0; cpu_piece < theNode->cpuPieces.size(); cpu_piece++){
        game->checkValidity(theNode->gameBoard, theNode->cpuPieces[cpu_piece], &(possibleActions[0]), &(possibleActions[1]), &(possibleActions[2]));
    }

    //test
    for(int x = 0; x < possibleActions.size(); x++){
        game->printMoveChoices(&(possibleActions[x]), color);
    }
    return localV;
}

int CPU::minValue(Node* theNode){
    //If terminal state, return the utility value of the state
    int utilVal = game->checkWin(theNode->gameBoard, theNode->humanPieces, theNode->cpuPieces);
    if(utilVal != Game::None){
        return utilVal;
    }
    //If cutoff state, then return eval(state)
    int localV = 1000;

    //TODO: CHECK VALIDITY OF CPU. FOR ALL VALUES IN  ALL THE LIST, CREATE A NEW NODE THAT WILL STORE THE STATE OF THE BOARD AND DEPTH, AND PIECES

    //possibleActions[0] = capturing moves
    //possibleActions[1] = cantering moves
    //possibleActions[2] = plain moves
    vector<valid_moves> possibleActions {3};

    for(size_t human_piece = 0; human_piece < theNode->humanPieces.size(); human_piece++){
        game->checkValidity(theNode->gameBoard, theNode->humanPieces[human_piece], &(possibleActions[0]), &(possibleActions[1]), &(possibleActions[2]));
    }


    return localV;
}
