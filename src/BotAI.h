// Stackcore
// Copyright (C) 2023-2025 Jose R Arenas

#ifndef BOTAI_H
#define BOTAI_H

#include <vector>
#include "Cube.h"

// Forward declaration
class Game;

class BotAI {
public:
    struct Commands {
        bool up, down, left, right;
        bool rotX, rotXRev, rotY, rotYRev, rotZ, rotZRev;
        bool drop;
    };

    BotAI(Game* game);
    void update();
    void reset();
    Commands getCommands() const { return botCommands; }

private:
    Game* game;
    Commands botCommands;
    
    int botTimer;
    bool aiTargetCalculated;
    float aiTargetX;
    float aiTargetY;
    int aiTargetRotX;
    int aiTargetRotY;
    int aiTargetRotZ;
    int currentRotX;
    int currentRotY;
    int currentRotZ;

    void calculateBestMove();
    float evaluateBoardState(const std::vector<Cube>& droppedCubes);
};

#endif // BOTAI_H
