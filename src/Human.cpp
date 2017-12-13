#include "Human.h"
using namespace std;

void Human::initialize_pieces(){
    //Get data from initial position list and set the pieces accordingly
    int nCount = 0;
    for(const auto &nKey : initial_position){
        for(const auto nVal : nKey.second){
            pieces.push_back(createPiece(nCount, color, nKey.first, nVal));
            nCount++;
        }
    }
}

//Defining where the Human player's castles are on the board
const vector< pair<int, int> > Human::castles = {{13, 3}, {13, 4}};

Human::Human(string& teamColor) : enemy(nullptr), board(nullptr), game(nullptr){
    //Assign team color (Human should be WHITE)
    color = teamColor;
    
    //Initialize Human initial_position
    initial_position.insert(pair<int, vector<int> >(8, {3,4}));
    initial_position.insert(pair<int, vector<int> >(9, {2,3,4,5}));
    
    //Initialize pieces
    initialize_pieces();
}

Human::Human(Human* theHuman): enemy(nullptr), board(nullptr), game(nullptr){
    //Copy color
    this->color = theHuman->color;
    //Copy pieces
    for(size_t nIndex = 0; nIndex < theHuman->pieces.size(); nIndex++){
        this->pieces.push_back(theHuman->pieces[nIndex]);
    }

}

void Human::setEnemy(CPU* theEnemy){
    //Set the new passed enemy as current enemy
    enemy = theEnemy;
}

void Human::setBoard(Board* theBoard){
    //Set the new passed board as current board
    board = theBoard;
}

void Human::setGame(Game* theGame){
    //Set the new passed board as current game
    game = theGame;
}

void Human::setPieces(vector<Player::Piece> newPieces){
    //Clean out current pieces
    pieces.clear();
    //Loop through the new pieces vector and assign it to current pieces vector
    for(size_t nIndex = 0; nIndex < newPieces.size(); nIndex++){
        pieces.push_back(newPieces[nIndex]);
    }
}


void Human::move(){
    //List of moves that will contain plain, cantering, and capturing moves
    valid_moves valid_plain;
    valid_moves valid_cantering;
    valid_moves valid_capturing;
    
    //Loop through each piece and check all valid positions for that piece
    for(size_t human_piece = 0; human_piece < pieces.size(); human_piece++){
        game->checkValidity(board, pieces[human_piece], &valid_capturing, &valid_cantering, &valid_plain);
    }

    //Check if capturing move is available, since capturing move is mandatory
    if(valid_capturing.size() != 0){
        cout << "Capturing move available. You must perform this action." << endl;
        cout << "Here are the possible capturing moves:" << endl;
        game->printMoveChoices(&valid_capturing, color);
        performCapture(&valid_capturing);
    }
    //Choose either cantering or plain move
    else{
        //Variable that will store user input relating to move type (Cantering or Plain)
        string move_response;

        //Loop until getting a correct response of either cantering or plain move
        while(1){
            cout << "Enter 'C' for cantering move or enter 'P' for plain move: ";
            cin >> move_response;
            if(toupper(move_response[0]) == 'C'){
                cout << "Cantering move chosen" << endl;
                //If there is a possible cantering move
                if(valid_cantering.size() != 0){
                    cout << "Here are the possible cantering moves:" << endl;
                    //Print out all possible cantering moves
                    game->printMoveChoices(&valid_cantering, color);
                    //Perform a cantering move
                    performMove(&valid_cantering);
                    break;
                }
                else{   //No available cantering move
                    cout << "There are no possible cantering moves." << endl;
                }
            }
            else if(toupper(move_response[0]) == 'P'){
                cout << "Plain move chosen" << endl;
                //If there is a possible plain move
                if(valid_plain.size() != 0){
                    cout << "Here are the possible plain moves:" << endl;
                    //Print out all possible plain move
                    game->printMoveChoices(&valid_plain, color);
                    //Perform a plain move
                    performMove(&valid_plain);
                    break;
                }
                else{   //No available plain move
                    cout << "There are no possible plain moves." << endl;
                }
            }
            else{   //Wrong input
                cout << "Choose either 'C' or 'P'." << endl;
            }

            //Ignore leftover inputs in the stream
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
}

vector<Player::Piece>& Human::getPieces(){
    //Return vector of piece by reference
    return pieces;
}

const vector< pair<int, int> >& Human::getCastles(){
    //Return castle values by reference
    return castles;
}

string& Human::getTeamColor(){
    //Return color by reference
    return color;
}

void Human::chooseMove(valid_moves* theList, int& chosenPiece, int& chosenRow, int& chosenCol){
    while(1){
        cout << "Please choose a piece: W";
        //Check if input type is correct
        while(!(cin >> chosenPiece)){   //Loop until correct input type is entered
            cin.clear();
            cout << "Wrong input type. Input a number!" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please choose a piece: W";
        }

        //Ignore leftover inputs in the stream
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        //Check if chosenPiece is inside possible moves list
        Game::validItr chosenItr = theList->end();
        for(Game::validItr listItr = theList->begin(); listItr != theList->end(); listItr++){
            if(listItr->first == chosenPiece){
                chosenItr = listItr;
                break;
            }
        }

        //Chech if chosen number is valid
        if(chosenItr != theList->end()){
            cout << "Please enter row, column: ";
            while(!(cin >> chosenRow >> chosenCol)){    //Loop until correct input type is entered
                cin.clear();
                cout << "Wrong input type. Input a number!" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Please enter row, column: ";
            }

            //Ignore leftover inputs in the stream
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            //Correct input type
            //Find if row, column match the ones inside the possible positions
            vector<pair<int, int> >::iterator chosenPairItr = chosenItr->second.end();

            for(vector<pair<int, int> >::iterator pairItr = chosenItr->second.begin(); pairItr != chosenItr->second.end(); pairItr++){
                if(pairItr->first == chosenRow && pairItr->second == chosenCol){
                    chosenPairItr = pairItr;
                    break;
                }
            }

            if(chosenPairItr != chosenItr->second.end()){
                break;
            }
            else{   //Choosen a invalid (row,column)
                cout << "Invalid (row, column). Restarting pick." << endl;
            }
        }
        else{   //Chosen number not a valid piece
            cout << "Choose a valid number for the piece" << endl;
        }
    }
}

void Human::performMove(valid_moves* theList){
    int chosenPiece, chosenRow, chosenCol;

    //Choose a move
    chooseMove(theList, chosenPiece, chosenRow, chosenCol);

    //Find the piece that is associated with the chosen piece, row, col
    Game::vecPieceItr pieceItr = game->findPiece(pieces.begin(), pieces.end(), chosenPiece);
    //If found...
    if(pieceItr != pieces.end()){
        //Set old position as empty
        board->setPosition(pieceItr->row, pieceItr->column, board->getEmptyVal());
        //Set current position as the chosen row and column
        pieceItr->row = chosenRow;
        pieceItr->column = chosenCol;
    }
}

void Human::performCapture(valid_moves* theList){
    int chosenPiece, chosenRow, chosenCol;

    //Choose a move
    chooseMove(theList, chosenPiece, chosenRow, chosenCol);

    //Find the piece that is associated with the chosen piece, row, col
    Game::vecPieceItr pieceItr = game->findPiece(pieces.begin(), pieces.end(), chosenPiece);
    if(pieceItr != pieces.end()){
        //Get enemy piece's row and column
        int capturedRow = game->findBetweenVal(chosenRow, pieceItr->row);
        int capturedCol = game->findBetweenVal(chosenCol, pieceItr->column);

        //Delete the captured piece from the board
        board->setPosition(capturedRow, capturedCol, board->getEmptyVal());

        //Get the enemy piece's iterator at (capturedRow, capturedCol)
        Game::vecPieceItr enemyPieceItr = game->findPiece(enemy->getPieces().begin(), enemy->getPieces().end(), capturedRow, capturedCol);
        if(enemyPieceItr != enemy->getPieces().end()){
            //Remove the captured piece
            enemy->getPieces().erase(enemyPieceItr);
        }

        //Set old position as empty
        board->setPosition(pieceItr->row, pieceItr->column, board->getEmptyVal());
        //Set current position as the chosen row and column
        pieceItr->row = chosenRow;
        pieceItr->column = chosenCol;
    }
}
