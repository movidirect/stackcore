// Stackcore
// Copyright (C) 2023-2025 Jose R Arenas

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

#include "Utils.h"
#include "Block.h"
#include <iostream>
#include <tuple>
#include <cstdlib>

Block::Block(float startX, float startY, float startZ) 
    : blockCount(0), cubeCount(0)
{
    generateRandomShape(startX, startY, startZ);
}

// New constructor that accepts a specific block type
Block::Block(float startX, float startY, float startZ, int blockType) 
    : blockCount(0), cubeCount(0)
{
    generateSpecificShape(startX, startY, startZ, blockType);
}

void Block::generateRandomShape(float startX, float startY, float startZ) {
    // Generate a random pattern between 0 and 8 to select the block shape
    int pattern = rand() % 9;

    //int pattern = 2;

    // Generate the block pattern based on the value of `pattern`
    switch (pattern) {
        case 0: // Single cube
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            break;
        case 1: // I-shaped with 2 cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            break;
        case 2: //I-shaped with 3 cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 2.0f, startY, startZ, 1.0f);
            break;
        case 3: // L-shaped block with three cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX, startY + 1.0f, startZ, 1.0f);
            break;
       case 4: // L-shaped block with four cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 2.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX, startY + 1.0f, startZ, 1.0f);
            break;
        case 5: // Compact 2x2 cube (4 cubes)
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX, startY + 1.0f, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY + 1.0f, startZ, 1.0f);
            break;
        case 6: // T-shaped with 4 cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 2.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY + 1.0f, startZ, 1.0f);
            break;    
        case 7: // I-shaped with 4 cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 2.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 3.0f, startY, startZ, 1.0f);
            break;
        case 8: // Cross-shaped with 5 cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 2.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY + 1.0f, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY - 1.0f, startZ, 1.0f);
            break;    
        default:
            break;
    }
    blockCount++;
    cubeCount = cubes.size();
}

// Generate a specific shape based on blockType
void Block::generateSpecificShape(float startX, float startY, float startZ, int blockType) {
    // Generate the block pattern based on the specified blockType
    switch (blockType) {
        case 0: // Single cube
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            break;
        case 1: // I-shaped with 2 cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            break;
        case 2: //I-shaped with 3 cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 2.0f, startY, startZ, 1.0f);
            break;
        case 3: // L-shaped block with three cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX, startY + 1.0f, startZ, 1.0f);
            break;
       case 4: // L-shaped block with four cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 2.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX, startY + 1.0f, startZ, 1.0f);
            break;
        case 5: // Compact 2x2 cube (4 cubes)
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX, startY + 1.0f, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY + 1.0f, startZ, 1.0f);
            break;
        case 6: // T-shaped with 4 cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 2.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY + 1.0f, startZ, 1.0f);
            break;    
        case 7: // I-shaped with 4 cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 2.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 3.0f, startY, startZ, 1.0f);
            break;
        case 8: // Cross-shaped with 5 cubes
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 2.0f, startY, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY + 1.0f, startZ, 1.0f);
            cubes.emplace_back(startX + 1.0f, startY - 1.0f, startZ, 1.0f);
            break;    
        default:
            // Default to single cube if invalid type
            cubes.emplace_back(startX, startY, startZ, 1.0f);
            break;
    }
    blockCount++;
    cubeCount = cubes.size();
}

/*
void Block::calculateCenter(float& centerX, float& centerY, float& centerZ) const {
    centerX = 0.0f;
    centerY = 0.0f;
    centerZ = 0.0f;

    for (const auto& cube : cubes) {
        centerX += cube.x;
        centerY += cube.y;
        centerZ += cube.z;
    }

    centerX /= cubes.size();
    centerY /= cubes.size();
    centerZ /= cubes.size();
}
*/

void Block::rotateX() {
    if (cubes.empty()) return;

    float pivotY = cubes[0].y;
    float pivotZ = cubes[0].z;

    for (auto& cube : cubes) {
        float y = cube.y;
        float z = cube.z;

        float newY = -(z - pivotZ) + pivotY;
        float newZ =  (y - pivotY) + pivotZ;

        cube.y = Utils::round(newY);
        cube.z = Utils::round(newZ);
    }
}


void Block::rotateY() {
    if (cubes.empty()) return;

    float pivotX = cubes[0].x;
    float pivotZ = cubes[0].z;

    for (auto& cube : cubes) {
        float x = cube.x;
        float z = cube.z;

        float newX =  (z - pivotZ) + pivotX;
        float newZ = -(x - pivotX) + pivotZ;

        cube.x = Utils::round(newX);
        cube.z = Utils::round(newZ);
    }
}



void Block::rotateZ() {
    if (cubes.empty()) return;

    float pivotX = cubes[0].x;
    float pivotY = cubes[0].y;

    for (auto& cube : cubes) {
        float x = cube.x;
        float y = cube.y;

        float newX = -(y - pivotY) + pivotX;
        float newY =  (x - pivotX) + pivotY;

        cube.x = Utils::round(newX);
        cube.y = Utils::round(newY);
    }
}


void Block::render(bool wireframe) const {
    
    for (const auto& cube : cubes) {
        cube.render(wireframe);
    }
}

void Block::move(float dx, float dy, float dz) {
    for (auto& cube : cubes) {
        cube.move(dx, dy, dz);
    }
}

// Check if all cubes in the block are within the scene limits
bool Block::isWithinBounds(float sceneLimit) const {
    for (const auto& cube : cubes) {
        if (cube.x < -sceneLimit || cube.x > sceneLimit || 
            cube.y < -sceneLimit || cube.y > sceneLimit ||
            cube.z < -sceneLimit || cube.z > sceneLimit) {
            return false;
        }
    }
    return true;
}

// Safe rotation methods that check bounds before applying rotation
bool Block::tryRotateX(float sceneLimit) {
    if (cubes.empty()) return false;

    // Create a temporary copy of cube positions
    std::vector<Cube> tempCubes = cubes;
    
    float pivotY = tempCubes[0].y;
    float pivotZ = tempCubes[0].z;

    // Apply rotation to temporary copy
    for (auto& cube : tempCubes) {
        float y = cube.y;
        float z = cube.z;

        float newY = -(z - pivotZ) + pivotY;
        float newZ =  (y - pivotY) + pivotZ;

        cube.y = Utils::round(newY);
        cube.z = Utils::round(newZ);
    }

    // Check if the rotated block would be within bounds
    for (const auto& cube : tempCubes) {
        if (cube.x < -sceneLimit || cube.x > sceneLimit || 
            cube.y < -sceneLimit || cube.y > sceneLimit ||
            cube.z < -sceneLimit || cube.z > sceneLimit) {
            return false; // Rotation would go out of bounds
        }
    }

    // If we reach here, rotation is safe - apply it to the actual block
    cubes = tempCubes;
    return true;
}

bool Block::tryRotateY(float sceneLimit) {
    if (cubes.empty()) return false;

    // Create a temporary copy of cube positions
    std::vector<Cube> tempCubes = cubes;
    
    float pivotX = tempCubes[0].x;
    float pivotZ = tempCubes[0].z;

    // Apply rotation to temporary copy
    for (auto& cube : tempCubes) {
        float x = cube.x;
        float z = cube.z;

        float newX =  (z - pivotZ) + pivotX;
        float newZ = -(x - pivotX) + pivotZ;

        cube.x = Utils::round(newX);
        cube.z = Utils::round(newZ);
    }

    // Check if the rotated block would be within bounds
    for (const auto& cube : tempCubes) {
        if (cube.x < -sceneLimit || cube.x > sceneLimit || 
            cube.y < -sceneLimit || cube.y > sceneLimit ||
            cube.z < -sceneLimit || cube.z > sceneLimit) {
            return false; // Rotation would go out of bounds
        }
    }

    // If we reach here, rotation is safe - apply it to the actual block
    cubes = tempCubes;
    return true;
}

bool Block::tryRotateZ(float sceneLimit) {
    if (cubes.empty()) return false;

    // Create a temporary copy of cube positions
    std::vector<Cube> tempCubes = cubes;
    
    float pivotX = tempCubes[0].x;
    float pivotY = tempCubes[0].y;

    // Apply rotation to temporary copy
    for (auto& cube : tempCubes) {
        float x = cube.x;
        float y = cube.y;

        float newX = -(y - pivotY) + pivotX;
        float newY =  (x - pivotX) + pivotY;

        cube.x = Utils::round(newX);
        cube.y = Utils::round(newY);
    }

    // Check if the rotated block would be within bounds
    for (const auto& cube : tempCubes) {
        if (cube.x < -sceneLimit || cube.x > sceneLimit || 
            cube.y < -sceneLimit || cube.y > sceneLimit ||
            cube.z < -sceneLimit || cube.z > sceneLimit) {
            return false; // Rotation would go out of bounds
        }
    }

    // If we reach here, rotation is safe - apply it to the actual block
    cubes = tempCubes;
    return true;
}

