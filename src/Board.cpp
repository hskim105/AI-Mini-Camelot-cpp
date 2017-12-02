#include "Board.h"
using namespace std;


Board::Board(){
    initializeBoard();
}

void Board::printBoard(){
    //Loop through and print the board
    for(size_t nRow = 0; nRow < MAX_ROW; nRow++){
        for(size_t nCol = 0; nCol < MAX_COLUMN; nCol++){
            cout << board[nRow][nCol] << '\t';
        }
        cout << endl;
    }
}

void Board::updateBoard(Human* theHuman, CPU* theCPU){
    vector<Player::Piece> humanPieces = theHuman->getPieces();
    vector<Player::Piece> cpuPieces = theCPU->getPieces();
    
    //Set CPU pieces on the board
    for(size_t cpu_piece = 0; cpu_piece < cpuPieces.size(); cpu_piece++){
        board[cpuPieces[cpu_piece].row][cpuPieces[cpu_piece].column] = 'B' + to_string(cpuPieces[cpu_piece].number);
    }
    
    //Set Human pieces on the board
    for(size_t human_piece = 0; human_piece < humanPieces.size(); human_piece++){
        board[humanPieces[human_piece].row][humanPieces[human_piece].column] = 'W' + to_string(humanPieces[human_piece].number);
    }
}

vector< vector<string> >* Board::getBoard(){
    return &board;
}

const string& Board::getBorderVal(){
    return BORDER_VAL;
}

const string& Board::getEmptyVal(){
    return EMPTY_VAL;
}

const int Board::MAX_ROW = 14;
const int Board::MAX_COLUMN = 8;
const string Board::BORDER_VAL("XX");
const string Board::EMPTY_VAL("__");

void Board::initializeBoard(){
    //Loop through rows
    for(size_t nRow = 0; nRow < MAX_ROW; nRow++){
        vector<string> tempRow;
        //Loop through nRow's columns
        for(size_t nCol = 0; nCol < MAX_COLUMN; nCol++){
            //Check to see if nRow is in bad_positions
            mapItr keyItr = bad_positions.find(nRow);
            //If nRow is in bad_positions keys and nCol has a corresponding value in that key, it is a bad positions
            if(keyItr != bad_positions.end() && find(keyItr->second.begin(), keyItr->second.end(), nCol) != keyItr->second.end()){
                tempRow.push_back(BORDER_VAL);
            }
            else{   //Valid position
                tempRow.push_back(EMPTY_VAL);
            }
        }
        //Add nRow's complete column to board
        board.push_back(tempRow);
    }
}
