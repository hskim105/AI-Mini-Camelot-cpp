#include "CPU.h"
using namespace std;

CPU::CPU(string& teamColor) : enemy(nullptr), board(nullptr), game(nullptr), ALPHA_VAL(-1000), BETA_VAL(1000), MAX_DEPTH(1000){
    //Assign team color (CPU should be BLACK)
    color = teamColor;
    
    //Initialize Human initial_position
    initial_position.insert(pair<int, vector<int> >(4, {2,3,4,5}));
    initial_position.insert(pair<int, vector<int> >(5, {3,4}));
    
    //Initialize pieces
    initialize_pieces();
}

CPU::CPU(CPU* theCPU) : enemy(nullptr), board(nullptr), game(nullptr), ALPHA_VAL(-1000), BETA_VAL(1000), MAX_DEPTH(1000){
    //Copy color
    this->color = theCPU->color;
    //Copy pieces
    for(size_t nIndex = 0; nIndex < theCPU->pieces.size(); nIndex++){
        this->pieces.push_back(theCPU->pieces[nIndex]);
    }
}

void CPU::setEnemy(Human* theEnemy){
    //Set the new passed enemy as current enemy
    enemy = theEnemy;
}

void CPU::setBoard(Board* theBoard){
    //Set the new passed board as current board
    board = theBoard;
}

void CPU::setGame(Game* theGame){
    //Set the new passed board as current game
    game = theGame;
}

void CPU::setPieces(vector<Player::Piece> newPieces){
    //Clean out current pieces
    pieces.clear();
    //Loop through the new pieces vector and assign it to current pieces vector
    for(size_t nIndex = 0; nIndex < newPieces.size(); nIndex++){
        pieces.push_back(newPieces[nIndex]);
    }
}

void CPU::move(){
    Node* rootNode;
    AlphaBetaStats* abStats = new AlphaBetaStats();

    //Start time
    time_t startTime = time(nullptr);
    time_t endTime = time(nullptr);
    for(size_t currentDepth = 0; currentDepth < MAX_DEPTH; currentDepth++){
        uint elapsedTime = difftime(endTime, startTime);
        //While the elapsed time is less than the time limit
        if( elapsedTime <= TIME_LIMIT){
            rootNode = alphaBeta(board, &startTime, currentDepth, abStats);
            time(&endTime);
        }
        else{
            break;
        }

    }

    //Perform move
    if(rootNode->resultNode != nullptr){
        //Clean out the human pieces on the board
        for(size_t nIndex = 0; nIndex < enemy->getPieces().size(); nIndex++){
            board->setPosition(enemy->getPieces()[nIndex].row, enemy->getPieces()[nIndex].column, board->getEmptyVal());
        }
        //Set the new enemy pieces data into oppnent's piece
        enemy->setPieces(rootNode->resultNode->humanPieces);

        //Clean out cpu pieces on the board
        for(size_t nIndex = 0; nIndex < this->getPieces().size(); nIndex++){
            board->setPosition(this->getPieces()[nIndex].row, this->getPieces()[nIndex].column, board->getEmptyVal());
        }
        //Set new pieces data into current pieces
        this->setPieces(rootNode->resultNode->cpuPieces);
    }

    //Print alpha beta stats
    printStats(abStats);

    //Cleanup
    abStats = nullptr;
    delete abStats;
    deleteNodes(rootNode);
}

string& CPU::getTeamColor(){
    //Return color by reference
    return color;
}

const vector< pair<int, int> >& CPU::getCastles(){
    //Return castle values by reference
    return castles;
}
vector<Player::Piece>& CPU::getPieces(){
    //Return vector of piece by reference
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

CPU::Node* CPU::alphaBeta(Board* theBoard, time_t* startTime, uint theDepth, AlphaBetaStats* theStats){
    Node* rootNode = new Node(theBoard, ALPHA_VAL, BETA_VAL, 0, pieces, enemy->getPieces());

    //Assign result of the MAX-Value function to v
    maxValue(rootNode, startTime, theDepth, theStats);

    return rootNode;
}

int CPU::maxValue(Node* theNode, time_t* startTime, uint theDepth, AlphaBetaStats* theStats){
    //Update stats
    if(theNode->depth > theStats->maxDepthReached){
        theStats->maxDepthReached = theNode->depth;
    }

    //If terminal state, return the utility value of the state
    int utilVal = game->checkWin(theNode->gameBoard, theNode->humanPieces, theNode->cpuPieces);
    if(utilVal != Game::OnGoing){
        return utilVal;
    }

    //If cutoff state, then return eval(state)
    if(theNode->depth >= theDepth || difftime(time(nullptr), *startTime) > TIME_LIMIT){
        return evaluationFxn(theNode, theNode->cpuPieces);
    }

    int localV = -1000;

    //possibleActions[0] = capturing moves
    //possibleActions[1] = cantering moves
    //possibleActions[2] = plain moves
    vector<valid_moves> possibleActions {3};

    //Store all possible actions into a vector of valid moves
    findAllValidMoves(possibleActions, theNode, theNode->cpuPieces);

    //Perform capturing move
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

                //Update stats
                theStats->totalNodesGenerated += 1;

                //LocalV is less than the result: localV = max(localV, minValue(minNode, startTime, theDepth, theStats))
                if(localV < minValue(minNode, startTime, theDepth, theStats)){
                    localV = minValue(minNode, startTime, theDepth, theStats);
                    if(theNode->resultNode != nullptr){
                        deleteNodes(theNode->resultNode);
                    }
                    theNode->resultNode = minNode;
                }
                else{   //Delete the node since it will not hold the best possible move
                        //Cleanup
                    deleteNodes(minNode);
                }

                if(localV >= theNode->beta){
                    //Prune
                    //Update stats
                    theStats->nMaxPrune += 1;

                    return localV;
                }
                theNode->alpha = max(theNode->alpha, localV);
            }
        }
    }
    else{
        for(size_t nIndex = 1; nIndex < possibleActions.size(); nIndex++){
            //Perform either cantering or plain
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

                        //Update stats
                        theStats->totalNodesGenerated += 1;

                        //LocalV is less than the result: localV = max(localV, minValue(minNode, startTime, theDepth, theStats))
                        if(localV < minValue(minNode, startTime, theDepth, theStats)){
                            localV = minValue(minNode, startTime, theDepth, theStats);
                            if(theNode->resultNode != nullptr){
                                deleteNodes(theNode->resultNode);
                            }
                            theNode->resultNode = minNode;
                        }
                        else{   //Delete the node since it will not hold the best possible move
                                //Cleanup
                            deleteNodes(minNode);
                        }

                        if(localV >= theNode->beta){
                            //Prune

                            //Update stats
                            theStats->nMaxPrune += 1;

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

int CPU::minValue(Node* theNode, time_t* startTime, uint theDepth, AlphaBetaStats* theStats){
    if(theNode->depth > theStats->maxDepthReached){
        theStats->maxDepthReached = theNode->depth;
    }
    
    //If terminal state, return the utility value of the state
    int utilVal = game->checkWin(theNode->gameBoard, theNode->humanPieces, theNode->cpuPieces);
    if(utilVal != Game::OnGoing){
        return utilVal;
    }

    //If cutoff state, then return eval(state)
    if(theNode->depth >= theDepth || difftime(time(nullptr), *startTime) > TIME_LIMIT){
        return evaluationFxn(theNode, theNode->humanPieces) * -1;
    }
    int localV = 1000;

    //possibleActions[0] = capturing moves
    //possibleActions[1] = cantering moves
    //possibleActions[2] = plain moves
    vector<valid_moves> possibleActions {3};

    //Store all possible actions into a vector of valid moves
    findAllValidMoves(possibleActions, theNode, theNode->humanPieces);

    //Perform capturing move
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

                //Update stats
                theStats->totalNodesGenerated += 1;

                //LocalV is greater than the result: localV = min(localV, maxValue(maxNode, startTime, theDepth, theStats))
                if(localV > maxValue(maxNode, startTime, theDepth, theStats)){
                    localV = maxValue(maxNode, startTime, theDepth, theStats);
                    if(theNode->resultNode != nullptr){
                        deleteNodes(theNode->resultNode);
                    }
                    theNode->resultNode = maxNode;
                }
                else{   //Delete the node since it will not hold the best possible move
                    //Cleanup
                    deleteNodes(maxNode);
                }

                if(localV <= theNode->alpha){
                    //Prune
                    //Update stats
                    theStats->nMinPrune += 1;

                    return localV;
                }
                theNode->beta = min(theNode->beta, localV);
            }
        }
    }
    //Perform either cantering or plain
    else{
        for(size_t nIndex = 1; nIndex < possibleActions.size(); nIndex++){
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

                        //Update stats
                        theStats->totalNodesGenerated += 1;

                        //LocalV is greater than the result: localV = min(localV, maxValue(maxNode, startTime, theDepth, theStats))
                        if(localV > maxValue(maxNode, startTime, theDepth, theStats)){
                            localV = maxValue(maxNode, startTime, theDepth, theStats);
                            if(theNode->resultNode != nullptr){
                                deleteNodes(theNode->resultNode);
                            }
                            theNode->resultNode = maxNode;
                        }
                        else{   //Delete the node since it will not hold the best possible move
                            //Cleanup
                            deleteNodes(maxNode);
                        }

                        if(localV <= theNode->alpha){
                            //Prune
                            //Update stats
                            theStats->nMinPrune += 1;

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
    //Loop through and clone each piece from source to clone
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
        //Set current position as the chosen row and column
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
        //Set current position as the chosen row and column
        pieceItr->row = chosenRow;
        pieceItr->column = chosenCol;
    }
}

int CPU::evaluationFxn(Node* theNode, vector<Piece>& myPieces){
    int localV = 0;
    vector<valid_moves> allValidMoves {3};

    findAllValidMoves(allValidMoves, theNode, myPieces);

    //The more capturing move you have, the better
    //The more closer to the castle, the better
    uint totalCapturingMoves = 0;
    uint totalCanteringMoves = 0;
    uint totalPlainMove = 0;

    for(size_t nIndex = 0; nIndex < allValidMoves.size(); nIndex++){
        valid_moves theList = allValidMoves[nIndex];
        for(validItr listItr = theList.begin(); listItr != theList.end(); listItr++){
            if(nIndex == 0){
                totalCapturingMoves += listItr->second.size();
            }
            else if(nIndex == 1){
                totalCanteringMoves += listItr->second.size();
            }
            else if(nIndex == 2){
                totalPlainMove += listItr->second.size();
            }
        }
    }

    localV += totalCapturingMoves * 16;
    localV += totalCanteringMoves;
    localV += totalPlainMove;

    //The closer your pieces are to enemy castle ROW, the less points you lose for eval fxn
    //The closer your pieces are to enemy castle COL, the less poitns you lose for eval fxn
    int closeEnemyCastle = 0;
    if(myPieces[0].team == "WHITE"){
        //Human
        for(size_t pieceIndex = 0; pieceIndex < myPieces.size(); pieceIndex++){
            for(size_t nIndex = 0; nIndex < this->getCastles().size(); nIndex++){
                int castleRow = this->getCastles()[nIndex].first;
                int castleCol = this->getCastles()[nIndex].second;

                int rowDiff = abs(myPieces[pieceIndex].row - castleRow);
                int colDiff = abs(myPieces[pieceIndex].column - castleCol);

                closeEnemyCastle += (rowDiff + colDiff);
            }
        }
    }
    else if(myPieces[0].team == "BLACK"){
        //CPU
        for(size_t pieceIndex = 0; pieceIndex < myPieces.size(); pieceIndex++){
            for(size_t nIndex = 0; nIndex < enemy->getCastles().size(); nIndex++){
                int castleRow = enemy->getCastles()[nIndex].first;
                int castleCol = enemy->getCastles()[nIndex].second;

                int rowDiff = abs(myPieces[pieceIndex].row - castleRow);
                int colDiff = abs(myPieces[pieceIndex].column - castleCol);

                closeEnemyCastle += (rowDiff + colDiff);
            }
        }
    }

    localV -= (closeEnemyCastle * 7);

    return localV;
}

void CPU::deleteNodes(Node* theNode){
    //Delete all the chained nodes
    while(theNode != nullptr){
        Node* tempNode = theNode;
        theNode = theNode->resultNode;
        delete tempNode;
    }
}

void CPU::printStats(AlphaBetaStats* theStats){
    //Print stats
    cout << "===========================================" << endl;
    cout << "                   Stats                   " << endl;
    cout << "===========================================" << endl;
    cout << "Max depth of game tree reached: " << theStats->maxDepthReached << endl;
    cout << "Total number of nodes genrated: " << theStats->totalNodesGenerated << endl;
    cout << "Number of times MAX-VALUE pruned: " << theStats->nMaxPrune << endl;
    cout << "Number of times MIN-VALUE pruned: " << theStats->nMinPrune << endl;
    cout << endl;
}

void CPU::findAllValidMoves(vector<valid_moves>& allMoves, Node* theNode, vector<Piece>& myPieces){
    //Store all possible actions into a vector of valid moves
    for(size_t nPiece = 0; nPiece < myPieces.size(); nPiece++){
        game->checkValidity(theNode->gameBoard, myPieces[nPiece], &(allMoves[0]), &(allMoves[1]), &(allMoves[2]));
    }
}
