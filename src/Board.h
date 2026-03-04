#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <set>
#include <tuple>
#include "Block.h"
#include "Utils.h"

class Board {
public:
    Board();
    ~Board();

    void reset();
    bool checkCollision(Block* block, float dz) const;
    void parkBlock(Block* block);
    int clearFullLevels();
    int calculateStackPosition() const;
    
    const std::vector<Block*>& getParkedBlocks() const { return parkedBlocks; }
    const std::set<std::tuple<int, int, int>>& getOccupiedPositions() const { return occupiedPositions; }

private:
    std::set<std::tuple<int, int, int>> occupiedPositions;
    std::vector<Block*> parkedBlocks;

    // Constants related to the board
    const float SCENE_LIMIT = 4.5f;
    const int GRID_SIZE = 9;
    const int MIN_Z_COORDINATE = -4;
};

#endif // BOARD_H
