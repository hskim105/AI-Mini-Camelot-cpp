#ifndef CPU_h
#define CPU_h

#include "Player.h"
#include "Human.h"
#include "Board.h"

class Human;
class Board;

class CPU : protected Player {
public:
    CPU(std::string& teamColor);

    void setEnemy(Human* theEnemy);

    void setBoard(Board* theBoard);

    void move();

    std::string& getTeamColor();
    
    const std::vector< std::pair<int, int> >& getCastles();

    std::vector<Piece>& getPieces();
    
private:
    Human* enemy;
    Board* board;
    int alpha, beta;
    static const std::vector< std::pair<int, int> > castles;
    void initialize_pieces();

    void alphaBeta(Board* theBoard);
    int maxValue(Board* theBoard, int nAlpha, int nBeta);
    int minValue(Board* theBoard, int nAlpha, int nBeta);
};

#endif /* CPU_h */
