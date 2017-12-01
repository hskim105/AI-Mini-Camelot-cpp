#ifndef CPU_h
#define CPU_h

#include "Player.h"

class CPU : protected Player {
public:
    CPU(std::string& teamColor);
    
    void move();
    
    std::vector<Piece>& getPieces();
    
private:
    void initialize_pieces();
};

#endif /* CPU_h */
