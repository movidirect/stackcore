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

#include "Data.h"
#include "Block.h"
#include "Utils.h"
#include <tuple>

// Function to read the high score from a binary file
std::string Data::getHighScore()
{
    int highScore;

    // Open the binary file "game.dat" in input mode
    std::ifstream rf("game.dat", std::ios::in | std::ios::binary);
    if (!rf) {
        std::cerr << "Could not open the file for reading" << std::endl;
        return "0";
    }

    // Read the high score from the binary file
    rf.read(reinterpret_cast<char*>(&highScore), sizeof(highScore));
    rf.close(); 

    // Convert the high score to a string and return it
    return std::to_string(highScore);
}

// Function to write the high score to a binary file
void Data::writeData(int highScore)
{
    // Open the binary file "game.dat" in output mode
    std::ofstream wf("game.dat", std::ios::out | std::ios::binary);
    if (!wf) {
        std::cerr << "Could not open the file for writing" << std::endl;
        return;
    }

    // Write the high score to the binary file
    wf.write(reinterpret_cast<const char*>(&highScore), sizeof(highScore));
    wf.close();
}


void Data::loadGameState(
    int& score, 
    int& blockCount,
    int& cubeCount,
    int& highScore, 
    bool& gameIsOver, 
    bool& gameIsPaused, 
    std::set<std::tuple<int, int, int>>& occupiedPositions, 
    std::vector<Block*>& parkedBlocks)
{
    std::ifstream file("game_state.dat", std::ios::binary);
    if (!file){
        std::cerr << "Could not open the file for reading" << std::endl;
        return;
    }
    
    // Limpiar estado actual con verificación de null pointers
    for (auto block : parkedBlocks) {
        if (block) {  // Check for null pointer before delete
            delete block;
        }
    }
    parkedBlocks.clear();
    occupiedPositions.clear();
    
    // Cargar variables de estado
    file.read(reinterpret_cast<char*>(&score), sizeof(score));
    file.read(reinterpret_cast<char*>(&blockCount), sizeof(blockCount));
    file.read(reinterpret_cast<char*>(&cubeCount), sizeof(cubeCount));
    file.read(reinterpret_cast<char*>(&highScore), sizeof(highScore));
    file.read(reinterpret_cast<char*>(&gameIsOver), sizeof(gameIsOver));
    file.read(reinterpret_cast<char*>(&gameIsPaused), sizeof(gameIsPaused));
    
    // Cargar bloques parqueados
    size_t blockCountt;
    file.read(reinterpret_cast<char*>(&blockCountt), sizeof(blockCountt));
    
    for (size_t i = 0; i < blockCountt; i++) {
        size_t cubeCountt;
        file.read(reinterpret_cast<char*>(&cubeCountt), sizeof(cubeCountt));
        
        Block* block = new Block(0.0f, 0.0f, 0.0f); // Crear nuevo bloque con floats
        block->cubes.clear(); // Limpiar cubos por defecto
        
        for (size_t j = 0; j < cubeCountt; j++) {
            float x, y, z;
            file.read(reinterpret_cast<char*>(&x), sizeof(float));
            file.read(reinterpret_cast<char*>(&y), sizeof(float));
            file.read(reinterpret_cast<char*>(&z), sizeof(float));
            
            block->cubes.emplace_back(x, y, z, 1.0f);
            occupiedPositions.insert(std::make_tuple(
                static_cast<int>(Utils::round(x)), 
                static_cast<int>(Utils::round(y)), 
                static_cast<int>(Utils::round(z))
            ));
        }
        
        parkedBlocks.push_back(block);
    }
    
    file.close();
}


 void Data::saveGameState(
    int score, 
    int blockCount,
    int cubeCount,
    int highScore, 
    bool gameIsOver, 
    bool gameIsPaused, 
    const std::vector<Block*>& parkedBlocks)
{

    std::ofstream file("game_state.dat", std::ios::binary);
    
    // Guardar variables de estado
    file.write(reinterpret_cast<char*>(&score), sizeof(score));
    file.write(reinterpret_cast<char*>(&blockCount), sizeof(blockCount));
    file.write(reinterpret_cast<char*>(&cubeCount), sizeof(cubeCount));
    file.write(reinterpret_cast<char*>(&highScore), sizeof(highScore));
    file.write(reinterpret_cast<char*>(&gameIsOver), sizeof(gameIsOver));
    file.write(reinterpret_cast<char*>(&gameIsPaused), sizeof(gameIsPaused));
    
    // Guardar bloques parqueados con verificación de null pointers
    size_t blockCountt = parkedBlocks.size();
    file.write(reinterpret_cast<char*>(&blockCountt), sizeof(blockCountt));
    
    for (const auto& block : parkedBlocks) {
        if (block) {  // Check for null pointer before accessing cubes
            size_t cubeCountt = block->cubes.size();
            file.write(reinterpret_cast<char*>(&cubeCountt), sizeof(cubeCountt));
            
            for (const auto& cube : block->cubes) {
                float x = cube.x;
                float y = cube.y;
                float z = cube.z;
                file.write(reinterpret_cast<char*>(&x), sizeof(float));
                file.write(reinterpret_cast<char*>(&y), sizeof(float));
                file.write(reinterpret_cast<char*>(&z), sizeof(float));
            }
        }
    }
    
    file.close();
}