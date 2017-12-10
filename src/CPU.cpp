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

    //possibleActions[0] = capturing moves
    //possibleActions[1] = cantering moves
    //possibleActions[2] = plain moves
    vector<valid_moves> possibleActions {3};

    //Store all possible actions into a vector of valid moves
    for(size_t cpu_piece = 0; cpu_piece < theNode->cpuPieces.size(); cpu_piece++){
        game->checkValidity(theNode->gameBoard, theNode->cpuPieces[cpu_piece], &(possibleActions[0]), &(possibleActions[1]), &(possibleActions[2]));
    }

    //TODO: Test code. Remove later
    for(int x = 0; x < possibleActions.size(); x++){
        game->printMoveChoices(&(possibleActions[x]), color);
    }

    for(size_t nIndex = 0; nIndex < possibleActions.size(); nIndex++){
        //Perform capturing move
        if(nIndex == 0){
            if(possibleActions[0].size() != 0){
                //Perform caputring move
                valid_moves* theList = &(possibleActions[0]);
                for(validItr listItr = theList->begin(); listItr != theList->end(); listItr++){
                    for(vector<pair<int,int> >::iterator pairItr = listItr->second.begin(); pairItr != listItr->second.end(); pairItr++){
                        //Copy existing board class
                        Board* childBoard = new Board(theNode->gameBoard);

                        //Copy cpu pieces and human pieces
                        vector<Piece> cpuClone = clonePieces(theNode->cpuPieces);
                        vector<Piece> humanClone = clonePieces(theNode->humanPieces);

                        int chosenPiece = listItr->first;
                        int chosenRow = pairItr->first;
                        int chosenCol = pairItr->second;

                        performCapture(childBoard, &cpuClone, &humanClone, chosenPiece, chosenRow, chosenCol);

                        //Update board
                        childBoard->updateBoard(humanClone, cpuClone);

                        //Create a new child node and add it to parent's childNodes list
                        Node* minNode = new Node(childBoard, ALPHA_VAL, BETA_VAL, theNode->depth+1, cpuClone, humanClone);

                        theNode->childNodes.push(minNode);  //TODO: Is this necessary?????

                        localV = max(localV, minValue(minNode));
                        if(localV >= theNode->beta){
                            //Prune
                            return localV;
                        }
                        theNode->alpha = max(theNode->alpha, localV);
                    }
                }
            }
        }
        //Perform either cantering or plain
        else{
            //Perform either cantering/plain move
            if(possibleActions[nIndex].size() != 0){
                //Perform cantering move or plain move
                valid_moves* theList = &(possibleActions[nIndex]);
                for(validItr listItr = theList->begin(); listItr != theList->end(); listItr++){
                    for(vector<pair<int,int> >::iterator pairItr = listItr->second.begin(); pairItr != listItr->second.end(); pairItr++){
                        //Copy existing board class
                        Board* childBoard = new Board(theNode->gameBoard);

                        //Copy cpu pieces and human pieces
                        vector<Piece> cpuClone = clonePieces(theNode->cpuPieces);
                        vector<Piece> humanClone = clonePieces(theNode->humanPieces);

                        int chosenPiece = listItr->first;
                        int chosenRow = pairItr->first;
                        int chosenCol = pairItr->second;

                        performMove(childBoard, &cpuClone, chosenPiece, chosenRow, chosenCol);

                        //Update board
                        childBoard->updateBoard(humanClone, cpuClone);

                        //Create a new child node and add it to parent's childNodes list
                        Node* minNode = new Node(childBoard, ALPHA_VAL, BETA_VAL, theNode->depth+1, cpuClone, humanClone);

                        theNode->childNodes.push(minNode);

                        localV = max(localV, minValue(minNode));
                        if(localV >= theNode->beta){
                            //Prune
                            return localV;
                        }
                        theNode->alpha = max(theNode->alpha, localV);
                    }
                }
            }
        }
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

    //possibleActions[0] = capturing moves
    //possibleActions[1] = cantering moves
    //possibleActions[2] = plain moves
    vector<valid_moves> possibleActions {3};

    //Store all possible actions into a vector of valid moves
    for(size_t human_piece = 0; human_piece < theNode->humanPieces.size(); human_piece++){
        game->checkValidity(theNode->gameBoard, theNode->humanPieces[human_piece], &(possibleActions[0]), &(possibleActions[1]), &(possibleActions[2]));
    }

    //TODO: Test code. Remove later
    for(int x = 0; x < possibleActions.size(); x++){
        game->printMoveChoices(&(possibleActions[x]), color);
    }

    for(size_t nIndex = 0; nIndex < possibleActions.size(); nIndex++){
        //Perform capturing move
        if(nIndex == 0){
            if(possibleActions[0].size() != 0){
                //Perform caputring move
                valid_moves* theList = &(possibleActions[0]);
                for(validItr listItr = theList->begin(); listItr != theList->end(); listItr++){
                    for(vector<pair<int,int> >::iterator pairItr = listItr->second.begin(); pairItr != listItr->second.end(); pairItr++){
                        //Copy existing board class
                        Board* childBoard = new Board(theNode->gameBoard);

                        //Copy cpu pieces and human pieces
                        vector<Piece> cpuClone = clonePieces(theNode->cpuPieces);
                        vector<Piece> humanClone = clonePieces(theNode->humanPieces);

                        int chosenPiece = listItr->first;
                        int chosenRow = pairItr->first;
                        int chosenCol = pairItr->second;

                        performCapture(childBoard, &humanClone, &cpuClone, chosenPiece, chosenRow, chosenCol);

                        //Update board
                        childBoard->updateBoard(humanClone, cpuClone);

                        //Create a new child node and add it to parent's childNodes list
                        Node* maxNode = new Node(childBoard, ALPHA_VAL, BETA_VAL, theNode->depth+1, cpuClone, humanClone);

                        theNode->childNodes.push(maxNode);  //TODO: Is this necessary?????

                        localV = min(localV, maxValue(maxNode));
                        if(localV <= theNode->alpha){
                            //Prune
                            return localV;
                        }
                        theNode->beta = min(theNode->beta, localV);
                    }
                }
            }
        }
        //Perform either cantering or plain
        else{
            //Perform either cantering/plain move
            if(possibleActions[nIndex].size() != 0){
                //Perform cantering move or plain move
                valid_moves* theList = &(possibleActions[nIndex]);
                for(validItr listItr = theList->begin(); listItr != theList->end(); listItr++){
                    for(vector<pair<int,int> >::iterator pairItr = listItr->second.begin(); pairItr != listItr->second.end(); pairItr++){
                        //Copy existing board class
                        Board* childBoard = new Board(theNode->gameBoard);

                        //Copy cpu pieces and human pieces
                        vector<Piece> cpuClone = clonePieces(theNode->cpuPieces);
                        vector<Piece> humanClone = clonePieces(theNode->humanPieces);

                        int chosenPiece = listItr->first;
                        int chosenRow = pairItr->first;
                        int chosenCol = pairItr->second;

                        performMove(childBoard, &humanClone, chosenPiece, chosenRow, chosenCol);

                        //Update board
                        childBoard->updateBoard(humanClone, cpuClone);

                        //Create a new child node and add it to parent's childNodes list
                        Node* maxNode = new Node(childBoard, ALPHA_VAL, BETA_VAL, theNode->depth+1, cpuClone, humanClone);

                        theNode->childNodes.push(maxNode);  //TODO: Is this necessary?????

                        localV = min(localV, maxValue(maxNode));
                        if(localV <= theNode->alpha){
                            //Prune
                            return localV;
                        }
                        theNode->beta = min(theNode->beta, localV);
                    }
                }
            }
        }
    }
    return localV;
}

vector<Player::Piece> CPU::clonePieces(vector<Piece> sourcePiece){
    vector<Piece> clonePiece;
    for(size_t nIndex = 0; nIndex < sourcePiece.size(); nIndex++){
        clonePiece.push_back(sourcePiece[nIndex]);
    }
    return clonePiece;
}

void CPU::performMove(Board* theBoard, vector<Piece>* myPiece, int chosenPiece, int chosenRow, int chosenCol){
    Game::vecPieceItr pieceItr = game->findPiece(myPiece->begin(), myPiece->end(), chosenPiece);
    if(pieceItr != myPiece->end()){
        //Set old position as empty
        theBoard->setPosition(pieceItr->row, pieceItr->column, theBoard->getEmptyVal());
        pieceItr->row = chosenRow;
        pieceItr->column = chosenCol;
    }
}

void CPU::performCapture(Board* theBoard, vector<Piece>* myPiece, vector<Piece>* enemyPiece, int chosenPiece, int chosenRow, int chosenCol){
    Game::vecPieceItr pieceItr = game->findPiece(myPiece->begin(), myPiece->end(), chosenPiece);

    if(pieceItr != myPiece->end()){
        //Delete the captured piece
        //Get enemy piece's row and column
        int capturedRow = game->findBetweenVal(chosenRow, pieceItr->row);
        int capturedCol = game->findBetweenVal(chosenCol, pieceItr->column);

        //Delete the captured piece from the board
        theBoard->setPosition(capturedRow, capturedCol, theBoard->getEmptyVal());

        //Get the enemy piece's iterator at (capturedRow, capturedCol)
        Game::vecPieceItr enemyPieceItr = game->findPiece(enemyPiece->begin(),enemyPiece->end(), capturedRow, capturedCol);
        if(enemyPieceItr != enemyPiece->end()){
            //Remove the captured piece
            enemyPiece->erase(enemyPieceItr);
        }

        //Set old position as empty
        theBoard->setPosition(pieceItr->row, pieceItr->column, theBoard->getEmptyVal());
        pieceItr->row = chosenRow;
        pieceItr->column = chosenCol;
    }
}
