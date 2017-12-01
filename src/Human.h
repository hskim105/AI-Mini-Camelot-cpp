#ifndef Human_h
#define Human_h

#include<iostream>
#include<string>
#include<vector>
#include<map>

#include "Player.h"

class Human : protected Player {
public:
    Human(std::string& teamColor);
    
    void move();
    
    std::vector<Piece>& getPieces();
    
private:
    void initialize_pieces();
};

#endif /* Human_h */
