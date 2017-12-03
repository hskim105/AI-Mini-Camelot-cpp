#ifndef Board_h
#define Board_h

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<algorithm>

#include "Human.h"
#include "CPU.h"

class Human;
class CPU;

class Board{
public:
    //Default constructor
    Board();
    
    //Print current state of the board
    void printBoard();
    
    //Update the board with new human and cpu pieces
    void updateBoard(Human* theHuman, CPU* theCPU);
    
    //Get the current state of the board
    std::vector< std::vector<std::string> >* getBoard();
    
    //Get Border value
    const std::string& getBorderVal();
    //Get Empty value
    const std::string& getEmptyVal();

    //Set a value for a position on the board
    void setPosition(int rowVal, int colVal, const std::string& posVal);
private:
    static const int MAX_ROW;
    static const int MAX_COLUMN;
    static const std::string BORDER_VAL;
    static const std::string EMPTY_VAL;
    
    const std::map<int, std::vector<int> > bad_positions = {{0, {0,1,2,5,6,7}},
                                                            {1, {0,1,6,7}},
                                                            {2, {0,7}},
                                                            {11, {0,7}},
                                                            {12, {0,1,6,7}},
                                                            {13, {0,1,2,5,6,7}}};
    std::vector< std::vector<std::string> > board;
    
    typedef std::map<int, std::vector<int> >::const_iterator mapItr;
    
    void initializeBoard();
};

#endif /* Board_h */
