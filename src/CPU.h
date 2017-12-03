#ifndef CPU_h
#define CPU_h

#include "Player.h"
#include "Human.h"
class Human;

class CPU : protected Player {
public:
    CPU(std::string& teamColor);

    void setEnemy(Human* theEnemy);

    void move();
    
    std::vector<Piece>& getPieces();
    
private:
    Human* enemy;
    void initialize_pieces();
};

#endif /* CPU_h */
