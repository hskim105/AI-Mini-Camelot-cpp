#ifndef CPU_h
#define CPU_h

#include<thread>
#include<ctime>
#include<queue>

#include "Player.h"
#include "Human.h"
#include "Board.h"
#include "Game.h"

class Human;
class Board;
class Game;

class CPU : protected Player {
public:
    typedef std::map<int, std::vector<std::pair<int, int> > > valid_moves;

    struct Node{
        Board*  gameBoard;
        int alpha;
        int beta;
        uint depth;
        std::queue<Node*> childNodes;
        std::vector<Piece> cpuPieces;
        std::vector<Piece> humanPieces;

        Node(Board*  theGameBoard, int nAlpha, int nBeta, uint nDepth,  std::vector<Piece> theCpuPieces,  std::vector<Piece> theHumanPieces)
        : gameBoard(theGameBoard), alpha(nAlpha), beta(nBeta), depth(nDepth), cpuPieces(theCpuPieces), humanPieces(theHumanPieces) {}
    };

    //Default constructor
    CPU(std::string& teamColor);

    //Copy constructor
    CPU(CPU* theCPU);

    void setEnemy(Human* theEnemy);

    void setBoard(Board* theBoard);

    void setGame(Game* theGame);

    void move();

    std::string& getTeamColor();
    
    const std::vector< std::pair<int, int> >& getCastles();

    std::vector<Piece>& getPieces();
    
private:
    Human* enemy;
    Board* board;
    Game* game;
    const int ALPHA_VAL;
    const int BETA_VAL;

    static const std::vector< std::pair<int, int> > castles;
    static const int TIME_LIMIT;

    void initialize_pieces();

    void alphaBeta(Board* theBoard);
    int maxValue(Node* theNode);
    int minValue(Node* theNode);
};

#endif /* CPU_h */
