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
    
    void printBoard();
    
    void updateBoard(Human* theHuman, CPU* theCPU);
private:
    static const int MAX_ROW = 14;
    static const int MAX_COLUMN = 8;
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
