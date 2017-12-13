#ifndef CPU_h
#define CPU_h

#include<ctime>
#include <stdlib.h>

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

    //Struct that will represent a node (used for alpha beta pruning)
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

    //Struct that will hold attributes related to alpha beta pruning statistics
    struct AlphaBetaStats {
        uint maxDepthReached = 0;
        uint totalNodesGenerated = 1;
        uint nMaxPrune = 0;
        uint nMinPrune = 0;

        AlphaBetaStats(){}

        AlphaBetaStats(uint theMaxDepthReached, uint theTotalNodesGenerated, uint theMaxPrune, uint theMinPrune)
        : maxDepthReached(theMaxDepthReached), totalNodesGenerated(theTotalNodesGenerated), nMaxPrune(theMaxPrune), nMinPrune(theMinPrune) {}
    };

    //Default constructor
    CPU(std::string& teamColor);

    //Copy constructor
    CPU(CPU* theCPU);

    //Setter methods for...
    //Enemy
    void setEnemy(Human* theEnemy);
    //Board
    void setBoard(Board* theBoard);
    //Game
    void setGame(Game* theGame);
    //Piece
    void setPieces(std::vector<Piece> newPieces);

    //Move method for performing a move
    void move();

    //Getter for team color
    std::string& getTeamColor();

    //Getter for position of castles
    const std::vector< std::pair<int, int> >& getCastles();

    //Getter for pieces vector
    std::vector<Piece>& getPieces();
    
private:
    Human* enemy;
    Board* board;
    Game* game;
    const int ALPHA_VAL;
    const int BETA_VAL;
    const int MAX_DEPTH;

    static const std::vector< std::pair<int, int> > castles;
    static const int TIME_LIMIT;

    void initialize_pieces();

    //Alpha beta search method
    Node* alphaBeta(Board* theBoard, time_t* startTime, uint theDepth, AlphaBetaStats* theStats);
    //Max value method
    int maxValue(Node* theNode, time_t* startTime, uint theDepth, AlphaBetaStats* theStats);
    //Min value method
    int minValue(Node* theNode, time_t* startTime, uint theDepth, AlphaBetaStats* theStats);
    //Clone existing vector of pieces
    std::vector<Piece> clonePieces(std::vector<Piece> sourcePiece);
    //Performs a cantering or plain move
    void performMove(Board* theBoard, std::vector<Piece>* myPiece, int chosenPiece, int chosenRow, int chosenCol);
    //Performs a capturing move
    void performCapture(Board* theBoard, std::vector<Piece>* myPiece, std::vector<Piece>* enemyPiece, int chosenPiece, int chosenRow, int chosenCol);
    //Performs the evaluation function
    int evaluationFxn(Node* theNode, std::vector<Piece>& myPieces);
    //Delete chain of nodes
    void deleteNodes(Node* theNode);
    //Prints alpha beta pruning stats
    void printStats(AlphaBetaStats* theStats);
    //Find all valid moves
    void findAllValidMoves(std::vector<valid_moves>& allMoves, Node* theNode, std::vector<Piece>& myPieces);
};

#endif /* CPU_h */
