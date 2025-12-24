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

#ifndef GAME_H
#define GAME_H

#include "SDLManager.h"
#include <set>
#include "Block.h"
#include "State.h"
#include "Data.h"

class Game {
    public:
        Game();
        ~Game();
        void run();
        bool init();
    private:
        SDLManager sdlManager;
        
        void handleEvents();
        void update();
        void render();
        void clean();
        void moveBlockWithCollision(float dx, float dy);
        bool checkCollisionWithWalls(Block* block, float dz);
        bool checkCollisionWithParkedBlocks(Block* block, float dz);
        bool isDepthLevelFull(int depthLevel);
        bool isAnyDepthLevelFull();
        void dropBlock();
        int calculateStackPosition();
        void resetGame(bool clearAll = false);
        void playSound(int sound);
        void saveHighScore();
        void updateLevel();  // New function for level progression
        void generateNextBlocks();  // Generate preview blocks
        int getNextBlockType();     // Get next block type from queue
        Block* calculateGhostBlock();  // Calculate ghost block position

        int score;
        int highScore;
        int blockCount;
        int cubeCount;
        int stackPosition;
        bool gameIsOver;
        bool gameIsPaused;
        bool soundEnabled;
        bool isRunning;
        
        // Configuration variables
        bool showGhostBlock;         // Toggle ghost block visibility
        bool showNextBlocks;         // Toggle next block preview
        float ghostBlockAlpha;       // Ghost block transparency
        int difficultyMode;          // 0=Easy, 1=Normal, 2=Hard
        
        // New variables for progressive difficulty system
        int currentLevel;
        float currentFallSpeed;
        int linesCleared;
        int targetLinesForNextLevel;
        
        float cameraX;
        float cameraY;
        float cameraZ;
        float cameraAngleX;
        float cameraAngleY;
        float initZ;
        
        // Game constants to replace magic numbers
        const float SCENE_LIMIT = 4.5f;
        const float SCENE_BACK_LIMIT = -4.5f;       // Back wall of the playing field  
        const float INITIAL_Z_POSITION = 4.5f;      // Entry point for new blocks
        const int GRID_SIZE = 9;                    // Size of the playing grid (9x9x9)
        const float CELL_SIZE = 1.0f;               // Size of each grid cell
        const int MIN_Z_COORDINATE = -4;            // Minimum Z coordinate for depth calculations
        const float AUTOMATIC_FALL_SPEED = -0.02f;  // Speed of automatic block movement
        const float MANUAL_DROP_SPEED = -1.0f;      // Speed when manually dropping blocks
        const int LEVEL_CLEAR_BONUS = 1000;         // Score bonus for clearing a complete level
        const int MAX_STACK_HEIGHT = 9;             // Maximum stack height before game over
        
        // New constants for level progression
        const float SPEED_INCREASE_FACTOR = 1.15f;  // Multiplier for speed increase per level
        const float MAX_FALL_SPEED = -0.5f;         // Maximum fall speed limit
        const int LINES_PER_LEVEL = 10;             // Lines needed to advance to next level
        const int LEVEL_SCORE_MULTIPLIER = 100;     // Additional score per level

        Mix_Music *parkedSound;
        Mix_Music *keyPressSound;
        Mix_Music *gameOverSound;
        Mix_Music *levelUpSound;     // New sound for level progression
        Mix_Music *lineClearSound;   // New sound for line clearing
        Uint32 lastFrameTime;
        
        std::set<std::tuple<int, int, int>> occupiedPositions;
        std::vector<Block*> parkedBlocks;  
        State *state;
        Data *data;
        Block* block;
        
        // Preview system for next blocks
        std::vector<int> nextBlockTypes;  // Queue of next block types
        static const int PREVIEW_COUNT = 3;  // Number of blocks to preview

        enum Sounds {
            PARKED_SOUND,
            KEY_PRESS_SOUND,
            GAME_OVER_SOUND,
            LEVEL_UP_SOUND,      // New sound enum
            LINE_CLEAR_SOUND     // New sound enum
        };        
};

#endif // GAME_H

