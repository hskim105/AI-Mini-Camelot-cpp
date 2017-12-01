#include "Board.h"
using namespace std;


Board::Board(){
    initializeBoard();
}

void Board::printBoard(){
    for(size_t nRow = 0; nRow < MAX_ROW; nRow++){
        for(size_t nCol = 0; nCol < MAX_COLUMN; nCol++){
            cout << board[nRow][nCol] << '\t';
        }
        cout << endl;
    }
}

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
                tempRow.push_back("XX");
            }
            else{   //Valid position
                tempRow.push_back("__");
            }
        }
        //Add nRow's complete column to board
        board.push_back(tempRow);
    }
}
