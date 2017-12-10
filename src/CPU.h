#ifndef CPU_h
#define CPU_h

#include<thread>
#include<ctime>

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
    typedef valid_moves::iterator validItr;

    struct Node{
        Board*  gameBoard;
        int alpha;
        int beta;
        uint depth;
        Node* resultNode;
        std::vector<Piece> cpuPieces;
        std::vector<Piece> humanPieces;

        Node(Board*  theGameBoard, int nAlpha, int nBeta, uint nDepth,  std::vector<Piece> theCpuPieces,  std::vector<Piece> theHumanPieces)
        : gameBoard(theGameBoard), alpha(nAlpha), beta(nBeta), depth(nDepth), resultNode(nullptr), cpuPieces(theCpuPieces), humanPieces(theHumanPieces) {}
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
    std::vector<Piece> clonePieces(std::vector<Piece> sourcePiece);
    void performMove(Board* theBoard, std::vector<Piece>* myPiece, int chosenPiece, int chosenRow, int chosenCol);
    void performCapture(Board* theBoard, std::vector<Piece>* myPiece, std::vector<Piece>* enemyPiece, int chosenPiece, int chosenRow, int chosenCol);
};

#endif /* CPU_h */
