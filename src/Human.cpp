#include "Human.h"
using namespace std;

void Human::initialize_pieces(){
    int nCount = 0;
    for(const auto &nKey : initial_position){
        for(const auto nVal : nKey.second){
            pieces.push_back(createPiece(nCount, color, nKey.first, nVal));
            nCount++;
        }
    }
}

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
    enemy = theEnemy;
}

void Human::setBoard(Board* theBoard){
    board = theBoard;
}

void Human::setGame(Game* theGame){
    game = theGame;
}

void Human::move(){
//    cout << "Human move" << endl;       //TODO: Debug. Remove later
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
    else{   //Choose either cantering or plain move
        string move_response;

        while(1){
            cout << "Enter 'C' for cantering move or enter 'P' for plain move: ";
            cin >> move_response;
            if(toupper(move_response[0]) == 'C'){
                cout << "Cantering move chosen" << endl;    //TODO: Debug. Remove later
                //If there is a possible cantering move
                if(valid_cantering.size() != 0){
                    cout << "Here are the possible cantering moves:" << endl;
                    game->printMoveChoices(&valid_cantering, color);
                    performMove(&valid_cantering);
                    break;
                }
                else{
                    cout << "There are no possible cantering moves." << endl;
                }
            }
            else if(toupper(move_response[0]) == 'P'){
                cout << "Plain move chosen" << endl;        //TODO: Debug. Remove later
                //If there is a possible plain move
                if(valid_plain.size() != 0){
                    cout << "Here are the possible plain moves:" << endl;
                    game->printMoveChoices(&valid_plain, color);
                    performMove(&valid_plain);
                    break;
                }
                else{
                    cout << "There are no possible plain moves." << endl;
                }
            }
            else{
                cout << "Choose either 'C' or 'P'." << endl;
            }

            //Ignore leftover inputs in the stream
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
}

vector<Player::Piece>& Human::getPieces(){
    return pieces;
}

const vector< pair<int, int> >& Human::getCastles(){
    return castles;
}

string& Human::getTeamColor(){
    return color;
}

void Human::chooseMove(valid_moves* theList, int& chosenPiece, int& chosenRow, int& chosenCol){
    while(1){
        cout << "Please choose a piece: W";
        //Check if input type is correct
        while(!(cin >> chosenPiece)){
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

        //Chosen number is valid
        if(chosenItr != theList->end()){
            cout << "Please enter row, column: ";
            while(!(cin >> chosenRow >> chosenCol)){
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

    Game::vecPieceItr pieceItr = game->findPiece(pieces.begin(), pieces.end(), chosenPiece);
    if(pieceItr != pieces.end()){
        //Set old position as empty
        board->setPosition(pieceItr->row, pieceItr->column, board->getEmptyVal());
        pieceItr->row = chosenRow;
        pieceItr->column = chosenCol;
    }
}

void Human::performCapture(valid_moves* theList){
    int chosenPiece, chosenRow, chosenCol;

    //Choose a move
    chooseMove(theList, chosenPiece, chosenRow, chosenCol);

    Game::vecPieceItr pieceItr = game->findPiece(pieces.begin(), pieces.end(), chosenPiece);
    if(pieceItr != pieces.end()){
        //Delete the captured piece
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
        pieceItr->row = chosenRow;
        pieceItr->column = chosenCol;
    }
}
