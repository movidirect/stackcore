// Stackcore
// Copyright (C) 2023-2026 Jose R Arenas

#include "BotAI.h"
#include "Game.h"
#include "Utils.h"
#include <cstdlib>

#include <algorithm>

BotAI::BotAI(Game* g) : game(g) {
    reset();
}

void BotAI::reset() {
    botTimer = 0;
    aiTargetCalculated = false;
    botCommands = {false, false, false, false, false, false, false, false, false, false, false};
    currentRotX = 0;
    currentRotY = 0;
    currentRotZ = 0;
}

float BotAI::evaluateBoardState(const std::vector<Cube>& droppedCubes, const bool baseGrid[9][9][14]) {
    // OPTIMIZATION: Copy base grid O(1) and add dropped cubes
    bool currentGrid[9][9][14];
    std::copy(&baseGrid[0][0][0], &baseGrid[0][0][0] + 9 * 9 * 14, &currentGrid[0][0][0]);
    
    for (const auto& c : droppedCubes) {
        int x = static_cast<int>(Utils::round(c.x)) + 4;
        int y = static_cast<int>(Utils::round(c.y)) + 4;
        int z = static_cast<int>(Utils::round(c.z)) + 4;
        if (x >= 0 && x < 9 && y >= 0 && y < 9 && z >= 0 && z < 14) {
            currentGrid[x][y][z] = true;
        }
    }

    auto isOccupied = [&](int x, int y, int z) {
        int gx = x + 4;
        int gy = y + 4;
        int gz = z + 4;
        if (gx >= 0 && gx < 9 && gy >= 0 && gy < 9 && gz >= 0 && gz < 14) {
            return currentGrid[gx][gy][gz];
        }
        return false;
    };
    float score = 0.0f;
    int maxZ = -5;
    int holes = 0;
    int bumpiness = 0;
    int blocksInBottomLevels = 0;
    
    // Height profile per column
    int heights[9][9] = {0};
    
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            heights[x][y] = -5;
        }
    }

    // Analyze columns to find holes and heights
    for (int x = -4; x <= 4; ++x) {
        for (int y = -4; y <= 4; ++y) {
            bool blockFound = false;
            int colHeight = -5;
            
            for (int z = 4; z >= -4; --z) {
                if (isOccupied(x, y, z)) {
                    if (!blockFound) {
                        blockFound = true;
                        colHeight = z;
                        if (z > maxZ) maxZ = z; 
                    }
                } else {
                    if (blockFound) {
                        holes += 1; 
                        holes += (colHeight - z); 
                    }
                }
            }
            heights[x+4][y+4] = colHeight;
        }
    }
    
    // Analyze bumpiness
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            if (x < 8) bumpiness += std::abs(heights[x][y] - heights[x+1][y]);
            if (y < 8) bumpiness += std::abs(heights[x][y] - heights[x][y+1]);
        }
    }
    
    // Evaluate how deep the pieces landed this turn
    float piecesDepthScore = 0.0f;
    for (const auto& cube : droppedCubes) {
        float z = Utils::round(cube.z);
        piecesDepthScore += (5.0f - z) * 500.0f; 
    }

    // Evaluate completed or filled levels at the bottom
    int fullLinesBonus = 0;
    for (int z = -4; z <= maxZ; ++z) {
        int blocksInLevel = 0;
        for (int x = -4; x <= 4; ++x) {
            for (int y = -4; y <= 4; ++y) {
                if (isOccupied(x, y, z)) {
                    blocksInLevel++;
                }
            }
        }
        
        if (z == -4) blocksInBottomLevels += blocksInLevel;
        if (z == -3) blocksInBottomLevels += (blocksInLevel / 2); 
        
        if (blocksInLevel == 81) {
            fullLinesBonus += 10000; 
        } else {
            fullLinesBonus += (blocksInLevel * blocksInLevel); 
        }
    }

    float heightWeight    = -500.0f; 
    float holesWeight     = -2000.0f; // MASSIVE penalty but not infinite to allow play
    float bumpinessWeight = -100.0f;  
    float bottomFillWeight= 200.0f;  
    
    score += (maxZ + 4) * heightWeight;
    score += holes * holesWeight;
    score += bumpiness * bumpinessWeight;
    score += fullLinesBonus;
    score += piecesDepthScore;
    score += blocksInBottomLevels * bottomFillWeight;

    return score;
}

void BotAI::calculateBestMove() {
    float bestScore = -999999.0f;
    aiTargetX = 0.0f;
    aiTargetY = 0.0f;
    aiTargetRotX = 0;
    aiTargetRotY = 0;
    aiTargetRotZ = 0;

    // OPTIMIZATION: Cache board state in 3D array O(1)
    bool baseGrid[9][9][14] = {false};
    for (const auto& pos : game->board->getOccupiedPositions()) {
        int x = std::get<0>(pos) + 4;
        int y = std::get<1>(pos) + 4;
        int z = std::get<2>(pos) + 4;
        if (x >= 0 && x < 9 && y >= 0 && y < 9 && z >= 0 && z < 14) {
            baseGrid[x][y][z] = true;
        }
    }

    // OPTIMIZATION: Cache geometries to avoid simulating redundant rotations
    std::set<std::vector<std::tuple<int, int, int>>> seenGeometries;

    for (int rX = 0; rX < 4; ++rX) {
        for (int rY = 0; rY < 4; ++rY) {
            for (int rZ = 0; rZ < 4; ++rZ) {
                
                Block testBlock(0.0f, 0.0f, game->initZ, 0); 
                testBlock.cubes = game->block->cubes;
                
                // Apply rotations ensuring the real physics engine allows it
                bool rotationPossible = true;
                for(int i=0; i<rX; i++) { if (!testBlock.tryRotateX(game->SCENE_LIMIT, game->board)) rotationPossible = false; }
                for(int i=0; i<rY; i++) { if (!testBlock.tryRotateY(game->SCENE_LIMIT, game->board)) rotationPossible = false; }
                for(int i=0; i<rZ; i++) { if (!testBlock.tryRotateZ(game->SCENE_LIMIT, game->board)) rotationPossible = false; }
                
                // If rotation combo is not physically possible at spawn, discard it
                if (!rotationPossible) continue;
                
                if (!testBlock.isWithinBounds(game->SCENE_LIMIT)) continue;
                
                // Generate unique signature (geometry) for this rotation
                std::vector<std::tuple<int, int, int>> geometrySignature;
                for (const auto& c : testBlock.cubes) {
                    geometrySignature.push_back(std::make_tuple(
                        static_cast<int>(Utils::round(c.x)),
                        static_cast<int>(Utils::round(c.y)),
                        static_cast<int>(Utils::round(c.z))
                    ));
                }
                std::sort(geometrySignature.begin(), geometrySignature.end());
                
                // If we already tried this exact shape, skip it
                if (seenGeometries.find(geometrySignature) != seenGeometries.end()) {
                    continue; 
                }
                seenGeometries.insert(geometrySignature);
                
                for (float dx = -4.0f; dx <= 4.0f; dx += 1.0f) {
                    for (float dy = -4.0f; dy <= 4.0f; dy += 1.0f) {
                        Block transBlock = testBlock;
                        transBlock.move(dx, dy, 0.0f);

                        // Check initial horizontal collisions
                        if (!transBlock.isWithinBounds(game->SCENE_LIMIT) || game->board->checkCollision(&transBlock, 0.0f)) {
                            continue;
                        }

                        // Simulate free fall (ghost block logic)
                        while (true) {
                            if (game->checkCollisionWithWalls(&transBlock, game->MANUAL_DROP_SPEED) ||
                                game->board->checkCollision(&transBlock, game->MANUAL_DROP_SPEED)) {
                                break;
                            }
                            transBlock.move(0.0f, 0.0f, game->MANUAL_DROP_SPEED);
                        }
                        
                        // Evaluate resulting board
                        float score = evaluateBoardState(transBlock.cubes, baseGrid);
                        
                        // Tie-break: prefer moves that place the block deeper
                        float minDroppedZ = 4.0f;
                        for(const auto& c : transBlock.cubes) {
                            if(c.z < minDroppedZ) minDroppedZ = c.z;
                        }
                        score -= minDroppedZ * 0.5f; 

                        // Save best move
                        if (score > bestScore) {
                            bestScore = score;
                            aiTargetX = dx;
                            aiTargetY = dy;
                            aiTargetRotX = rX;
                            aiTargetRotY = rY;
                            aiTargetRotZ = rZ;
                        }
                    }
                }
            }
        }
    }
    
    aiTargetCalculated = true;
    currentRotX = 0;
    currentRotY = 0;
    currentRotZ = 0;
}

void BotAI::update()
{
    // Clear previous commands
    botCommands = {false, false, false, false, false, false, false, false, false, false, false};
    
    if (game->gameIsOver || game->gameIsPaused || !game->block) return;

    if (!aiTargetCalculated) {
        calculateBestMove();
        return; // Wait one frame after computing
    }

    botTimer++;
    
    // Act every N frames so AI animation is visible
    // Set to 4 so it is fast and gravity does not catch up before placement
    if (botTimer % 4 != 0) return; 

    // 1. Execute rotations first
    if (currentRotX != aiTargetRotX) {
        botCommands.rotX = true;
        currentRotX = (currentRotX + 1) % 4;
        return;
    }
    if (currentRotY != aiTargetRotY) {
        botCommands.rotY = true;
        currentRotY = (currentRotY + 1) % 4;
        return;
    }
    if (currentRotZ != aiTargetRotZ) {
        botCommands.rotZ = true;
        currentRotZ = (currentRotZ + 1) % 4;
        return;
    }

    // 2. Execute translations (X and Y)
    // Compare with floating-point tolerance
    if (game->currentBlockX < aiTargetX - 0.1f) {
        botCommands.right = true;
        return;
    } else if (game->currentBlockX > aiTargetX + 0.1f) {
        botCommands.left = true;
        return;
    }

    if (game->currentBlockY < aiTargetY - 0.1f) {
        botCommands.up = true;
        return;
    } else if (game->currentBlockY > aiTargetY + 0.1f) {
        botCommands.down = true;
        return;
    }

    // 3. Drop (release when aligned)
    botCommands.drop = true;
    aiTargetCalculated = false; // Force recompute for next piece
}
