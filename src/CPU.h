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

    std::string& getTeamColor();
    
    const std::vector< std::pair<int, int> >& getCastles();

    std::vector<Piece>& getPieces();
    
private:
    Human* enemy;
    static const std::vector< std::pair<int, int> > castles;
    void initialize_pieces();
};

#endif /* CPU_h */
