#ifndef CPU_h
#define CPU_h

#include "Player.h"

class CPU : protected Player {
public:
    CPU(std::string& teamColor);
    
    void move();
    
private:
    void initialize_pieces();
};

#endif /* CPU_h */
