#ifndef Board_h
#define Board_h

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>

#include "Player.h"

class Board{
public:
    typedef std::map<int, std::vector<int> >::const_iterator mapItr;

    enum PositionValues {Border_Value = 'X',
                         CPU_Value = 'B',
                         Human_Value = 'W',
                         Empty_Value = '_',
                         Error_Value};

    //Default constructor
    Board();

    //Copy constructor
    Board(Board* theBoard);
    
    //Print current state of the board
    void printBoard();

    //Update the board with new human and cpu pieces
    void updateBoard(std::vector<Player::Piece>& humanPieces, std::vector<Player::Piece>& cpuPieces);
    
    //Get the current state of the board
    std::vector< std::vector<std::string> >* getBoard();
    //Get Border value
    const std::string& getBorderVal();
    //Get Empty value
    const std::string& getEmptyVal();
    //Get max row value
    const int getMaxRow();
    //Get max col value
    const int getMaxCol();

    //Set a value for a position on the board
    void setPosition(int rowVal, int colVal, const std::string& posVal);

    //Checks what value is store a position
    PositionValues checkPositionValue(int rowVal, int colVal);

private:
    static const int MAX_ROW;
    static const int MAX_COLUMN;
    static const std::string BORDER_VAL;
    static const std::string EMPTY_VAL;

    //Collection of bad postions that will act as the border of the board
    static const std::map<int, std::vector<int> > bad_positions;

    //The container that will act as the game board
    std::vector< std::vector<std::string> > board;

    //Initializes the board with BORDER_VAL and EMPTY_VAL
    void initializeBoard();
};

#endif /* Board_h */
