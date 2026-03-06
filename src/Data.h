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

#ifndef DATA_H
#define DATA_H
#include <iostream>
#include <fstream>
using namespace std;
#include <set>
#include <vector>

class Block; // Forward declaration of Block class

class Data
{
    public:
        Data(){};
        void writeData(int highScore);
        string getHighScore();
        void loadGameState(
            int& score, 
            int& blockCount,
            int& cubeCount,
            int& highScore, 
            bool& gameIsOver, 
            bool& gameIsPaused, 
            std::set<std::tuple<int, int, int>>& occupiedPositions, 
            std::vector<Block*>& parkedBlocks
        );
        void saveGameState(
            int score, 
            int blockCount,
            int cubeCount,
            int highScore, 
            bool gameIsOver, 
            bool gameIsPaused, 
            //std::set<std::tuple<int, int, int>>& occupiedPositions, 
            const std::vector<Block*>& parkedBlocks
        );
    private:
       ofstream wf;
};
#endif