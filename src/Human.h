#ifndef Human_h
#define Human_h

#include "Player.h"

class Human : protected Player {
public:
    Human(std::string& teamColor);
    
    void move();
    
private:
    void initialize_pieces();
};

#endif /* Human_h */
