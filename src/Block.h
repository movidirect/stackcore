// Stackcore
// Copyright (C) 2023-2026 Jose R Arenas

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <cstdlib>
#include "Cube.h"

class Board;

class Block {
public:
    std::vector<Cube> cubes;

    Block(float startX, float startY, float startZ);
    Block(float startX, float startY, float startZ, int blockType);
    void render(bool wireframe) const;
    void move(float dx, float dy, float dz);
    void rotateX();
    void rotateY();
    void rotateZ();
    
    // New methods for safe rotation with bounds checking
    bool tryRotateX(float sceneLimit, const Board* board = nullptr);
    bool tryRotateY(float sceneLimit, const Board* board = nullptr);
    bool tryRotateZ(float sceneLimit, const Board* board = nullptr);
    bool isWithinBounds(float sceneLimit) const;
    
    int getBlockCount() const { return blockCount; }
    int getCubeCount() const { return cubeCount; }
private:
    void generateRandomShape(float startX, float startY, float startZ);
    void generateSpecificShape(float startX, float startY, float startZ, int blockType);
    int blockCount;
    int cubeCount;
     
};

#endif // BLOCK_H
