#include "Board.h"
using namespace std;


Board::Board(){
    //When the Board class is first instantiated, the initiate board will be called
    initializeBoard();
}

Board::Board(Board* theBoard){
    //Loop through the row
    for(size_t nRow = 0; nRow < MAX_ROW; nRow++){
        vector<string> tempVec;
        //Loop through the column associated with nRow
        for(size_t nCol = 0; nCol < MAX_COLUMN; nCol++){
            //Store the contents of theBoard's board at (row,column)
            tempVec.push_back(theBoard->board[nRow][nCol]);
        }
        board.push_back(tempVec);
    }
}
void Board::printBoard(){
    //Print column guide
    cout << '\t';
    for(size_t colGuide = 0; colGuide < MAX_COLUMN; colGuide++){
        cout << colGuide << '\t';
    }
    cout << endl;

    //Loop through the row
    for(size_t nRow = 0; nRow < MAX_ROW; nRow++){
        cout << nRow << '\t';
        //Loop through the column associated with nRow
        for(size_t nCol = 0; nCol < MAX_COLUMN; nCol++){
            //Print out the contents of the board at (row,column)
            cout << board[nRow][nCol] << '\t';
        }
        cout << endl;
    }
}

void Board::updateBoard(Human* theHuman, CPU* theCPU){
    //Reference to the two players' pieces
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
    //Returns the address of the board
    return &board;
}

const string& Board::getBorderVal(){
    //Returns the reference of the BORDER_VAL
    return BORDER_VAL;
}

const string& Board::getEmptyVal(){
    //Returns the reference of the EMPTY_VAL
    return EMPTY_VAL;   //Checks what value is store a position
    PositionValues checkPositionValue(std::string& posVal);

}

const int Board::getMaxRow(){
    return MAX_ROW;
}

const int Board::getMaxCol(){
    return MAX_COLUMN;
}

void Board::setPosition(int rowVal, int colVal, const string& posVal){
    //Update the value of the position at (row,col) with the passed in parameter
    board[rowVal][colVal] = posVal;
}

Board::PositionValues Board::checkPositionValue(int rowVal, int colVal){
    string posVal = board[rowVal][colVal];

    if(posVal == BORDER_VAL){
        //Invalid position. (Border)
        return Border_Value;
    }
    else if(posVal[0] == 'B'){
        //CPU Piece
        return CPU_Value;
    }
    else if(posVal[0] == 'W'){
        //Human Piece
        return Human_Value;
    }
    else if (posVal == EMPTY_VAL){
        //Empty position
        return Empty_Value;
    }
    else{
        //Error
        return Error_Value;
    }
}


const int Board::MAX_ROW = 14;
const int Board::MAX_COLUMN = 8;
const string Board::BORDER_VAL("XX");
const string Board::EMPTY_VAL("__");
const map<int, vector<int> > Board::bad_positions = {{0, {0,1,2,5,6,7}},
                                                     {1, {0,1,6,7}},
                                                     {2, {0,7}},
                                                     {11, {0,7}},
                                                     {12, {0,1,6,7}},
                                                     {13, {0,1,2,5,6,7}}
                                                    };

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
