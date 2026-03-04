#include "Board.h"
#include <algorithm>
#include <map>

Board::Board() {}

Board::~Board() {
    reset();
}

void Board::reset() {
    for (auto& parkedBlock : parkedBlocks) {
        if (parkedBlock) {
            delete parkedBlock;
            parkedBlock = nullptr;
        }
    }
    parkedBlocks.clear();
    occupiedPositions.clear();
}

bool Board::checkCollision(Block* block, float dz) const {
    if (!block) return false;
    for (const auto& cube : block->cubes) {
        int nextZ = static_cast<int>(Utils::round(cube.z + dz));
        if (occupiedPositions.count(std::make_tuple(
                static_cast<int>(Utils::round(cube.x)), 
                static_cast<int>(Utils::round(cube.y)), 
                nextZ)) > 0) {
            return true;
        }
    }
    return false;
}

void Board::parkBlock(Block* block) {
    if (!block) return;
    
    // Align and record positions
    for (auto& cube : block->cubes) {
        cube.x = Utils::round(cube.x);
        cube.y = Utils::round(cube.y);
        cube.z = Utils::round(cube.z);
        occupiedPositions.insert(std::make_tuple(
            static_cast<int>(cube.x), 
            static_cast<int>(cube.y), 
            static_cast<int>(cube.z)));
    }
    parkedBlocks.push_back(block);
}

int Board::clearFullLevels() {
    int clearedCount = 0;
    
    // Evaluate levels from bottom (MIN_Z) to top
    int z = MIN_Z_COORDINATE;
    while (z < MIN_Z_COORDINATE + GRID_SIZE) {
        int count = 0;
        for (int x = -4; x <= 4; ++x) {
            for (int y = -4; y <= 4; ++y) {
                if (occupiedPositions.count(std::make_tuple(x, y, z)) > 0) {
                    count++;
                }
            }
        }

        if (count == GRID_SIZE * GRID_SIZE) { // 81
            clearedCount++;
            
            // 1. Remove cubes exactly at this Z
            for (auto& block : parkedBlocks) {
                if (!block) continue;
                block->cubes.erase(
                    std::remove_if(block->cubes.begin(), block->cubes.end(),
                        [z](const Cube& c) { return static_cast<int>(Utils::round(c.z)) == z; }),
                    block->cubes.end()
                );
            }

            // 2. Drop all cubes above this Z down by 1 level
            for (auto& block : parkedBlocks) {
                if (!block) continue;
                for (auto& cube : block->cubes) {
                    if (static_cast<int>(Utils::round(cube.z)) > z) {
                        cube.z -= 1.0f;
                    }
                }
            }

            // 3. Rebuild occupiedPositions immediately so the next loop iteration sees the dropped cubes
            occupiedPositions.clear();
            for (const auto& block : parkedBlocks) {
                if (!block) continue;
                for (const auto& cube : block->cubes) {
                    occupiedPositions.insert(std::make_tuple(
                        static_cast<int>(Utils::round(cube.x)),
                        static_cast<int>(Utils::round(cube.y)),
                        static_cast<int>(Utils::round(cube.z))
                    ));
                }
            }
            
            // Do NOT increment z here. The layer above just dropped into this z,
            // so we need to evaluate the current z again!
        } else {
            // Not full, move to the next layer up
            z++;
        }
    }

    // Clean up empty blocks
    if (clearedCount > 0) {
        parkedBlocks.erase(
            std::remove_if(parkedBlocks.begin(), parkedBlocks.end(),
                [](Block* b) {
                    if (b && b->cubes.empty()) {
                        delete b;
                        return true;
                    }
                    return false;
                }
            ),
            parkedBlocks.end()
        );
    }

    return clearedCount;
}

int Board::calculateStackPosition() const {
    if (parkedBlocks.empty()) {
        return -1;
    }
    
    int maxZ = MIN_Z_COORDINATE;
    for (const auto& parkedBlock : parkedBlocks) {
        if (parkedBlock) {
            for (const auto& cube : parkedBlock->cubes) {
                maxZ = std::max(maxZ, static_cast<int>(Utils::round(cube.z)));
            }
        }
    }
    return Utils::mapDepth(maxZ);
}
