// Stackcore
// Copyright (C) 2025 Jose R Arenas

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
#include "Game.h"
#include <iostream>
#include <tuple>
#include <limits>
#include <map>
#include <ctime>

Game::Game()
{
    // Initialize random seed once for the entire program
    srand(static_cast<unsigned int>(time(0)));
    
    isRunning = false;
    soundEnabled=true;
    block = nullptr;
    cameraX = -0.9f;
    cameraY = 0.0f;
    cameraZ = -16.0f;
    cameraAngleX = 0.0f;
    cameraAngleY = 0.0f;
    initZ = INITIAL_Z_POSITION;
    lastFrameTime = 0;
    stackPosition = -1;  // -1 means no blocks parked, 0+ means blocks up to that level
    score = 0;
    blockCount = 0;
    cubeCount = 0;
    gameIsOver = false;
    gameIsPaused = false;
    
    // Initialize configuration variables
    showGhostBlock = true;
    showNextBlocks = true;
    ghostBlockAlpha = 0.5f;
    difficultyMode = 1; // Normal difficulty by default
    
    // Initialize level progression system
    currentLevel = 1;
    currentFallSpeed = AUTOMATIC_FALL_SPEED;
    linesCleared = 0;
    targetLinesForNextLevel = LINES_PER_LEVEL;
    
    data = new Data();
    highScore = std::stoi(data->getHighScore());
    
    // Initialize preview system
    generateNextBlocks();
} 

Game::~Game() 
{
    clean();
}

bool Game::init() 
{
    // Initialize the SDL manager
    if (!sdlManager.init()) {
        std::cerr << "Error initializing SDL" << std::endl;
        return false;
    }

    // Initialize the GUI where the game state will be displayed
    state = new State(sdlManager.getWindow(), sdlManager.getGLContext());

    // Initialize the first block at the entry point
    block = new Block(0.0f, -0.0f, initZ, getNextBlockType());

    // Load sounds
    parkedSound   = sdlManager.loadSound("sounds/parked.mp3");
    keyPressSound = sdlManager.loadSound("sounds/select.mp3");
    gameOverSound = sdlManager.loadSound("sounds/gameover.mp3");
    levelUpSound  = sdlManager.loadSound("sounds/select.mp3");  // Reuse select sound for now
    lineClearSound = sdlManager.loadSound("sounds/parked.mp3"); // Reuse parked sound for now

    isRunning = true;
    //data->loadGameState(score, blockCount, cubeCount, highScore, gameIsOver, gameIsPaused, occupiedPositions, parkedBlocks);
    return true;
}

// Main game loop
void Game::run() 
{
    while (isRunning) {
        // Start the frame timer
        Uint32 frameStart = sdlManager.getTicks();

        // Handle user input
        handleEvents();

        // Update the game state
        update();

        // Draw the game state
        state->draw(stackPosition,score,highScore,blockCount,cubeCount,gameIsOver,gameIsPaused,soundEnabled,currentLevel,linesCleared,targetLinesForNextLevel,currentFallSpeed,showGhostBlock,&nextBlockTypes);

        // Render the game
        render();
        
        // Check if the stack is full
        if (stackPosition >= MAX_STACK_HEIGHT && !gameIsOver) {  // Use constant instead of magic number
            gameIsOver = true;
            playSound(GAME_OVER_SOUND);
        }

        // Delay the frame to achieve the desired frame rate
        sdlManager.delayFrame(frameStart);
    }
}

// Handle user input events
void Game::handleEvents()
{
    SDL_Event event;
    while (sdlManager.getEvent(&event)) {
         if (event.type == SDL_QUIT) {
            isRunning = false;
            saveHighScore();
        } else if (event.type == SDL_KEYDOWN){
            float dx = 0.0f;
            float dy = 0.0f;

            // Detect movement keys
            switch (event.key.keysym.sym) {
                case SDLK_UP: // Up 
                    dy = 1.0f;
                    playSound(KEY_PRESS_SOUND); 
                    break;  
                case SDLK_DOWN:  // Down
                    dy = -1.0f;
                    playSound(KEY_PRESS_SOUND); 
                    break;
                case SDLK_LEFT:  // Left
                    dx = -1.0f;
                    playSound(KEY_PRESS_SOUND); 
                    break;
                case SDLK_RIGHT:   // Right
                    dx = 1.0f;
                    playSound(KEY_PRESS_SOUND); 
                    break;
                case SDLK_p:
                     gameIsPaused = !gameIsPaused;
                     break;
                case SDLK_r:
                    resetGame(true);
                    break;     
                case SDLK_m:
                    soundEnabled = !soundEnabled;
                    break;        
                case SDLK_h: // Toggle ghost block
                    showGhostBlock = !showGhostBlock;
                    playSound(KEY_PRESS_SOUND);
                    break;
                case SDLK_n: // Toggle next block preview
                    showNextBlocks = !showNextBlocks;
                    playSound(KEY_PRESS_SOUND);
                    break;
                case SDLK_w: // Rotate around X-axis
                    if(event.key.keysym.mod & KMOD_CTRL){
                        cameraAngleX += 5.0f;
                    }else if(block){  // Added null check
                        // Try to rotate, only rotates if it stays within bounds
                        block->tryRotateX(SCENE_LIMIT);
                    }
                    playSound(KEY_PRESS_SOUND); 
                    break; 
                case SDLK_s: // Rotate in opposite direction around X-axis
                    if(event.key.keysym.mod & KMOD_CTRL){
                        cameraAngleX -= 5.0f;
                    }else if(block){  // Added null check
                        // Rotate in opposite direction (3 times = -90 degrees)
                        for(int i = 0; i < 3; i++) {
                            if(!block->tryRotateX(SCENE_LIMIT)) break; // Stop if rotation fails
                        }
                    }
                    playSound(KEY_PRESS_SOUND); 
                    break; 
                case SDLK_a: // Rotate around Y-axis
                    if(event.key.keysym.mod & KMOD_CTRL){
                        cameraAngleY += 5.0f;
                    }else if(block){  // Added null check
                        // Try to rotate, only rotates if it stays within bounds
                        block->tryRotateY(SCENE_LIMIT);
                    }
                    playSound(KEY_PRESS_SOUND); 
                    break; 
                case SDLK_d: // Rotate in opposite direction around Y-axis
                    if(event.key.keysym.mod & KMOD_CTRL){
                        cameraAngleY -= 5.0f;
                    }else if(block){  // Added null check
                        // Rotate in opposite direction (3 times = -90 degrees)
                        for(int i = 0; i < 3; i++) {
                            if(!block->tryRotateY(SCENE_LIMIT)) break; // Stop if rotation fails
                        }
                    }
                    playSound(KEY_PRESS_SOUND); 
                    break; 
                case SDLK_q: // Rotate around Z-axis
                    if(block){  // Added null check
                        // Try to rotate, only rotates if it stays within bounds
                        block->tryRotateZ(SCENE_LIMIT);
                    }
                    playSound(KEY_PRESS_SOUND); 
                    break; 
                case SDLK_e: // Rotate in opposite direction around Z-axis
                    if(block){  // Added null check
                        // Rotate in opposite direction (3 times = -90 degrees)
                        for(int i = 0; i < 3; i++) {
                            if(!block->tryRotateZ(SCENE_LIMIT)) break; // Stop if rotation fails
                        }
                    }
                    playSound(KEY_PRESS_SOUND); 
                    break; 
                case SDLK_SPACE: //Space bar
                    if(block) {  // Added null check
                        dropBlock(); 
                    }
                    break;
                case SDLK_RETURN:
                    if(gameIsOver)
                        resetGame(); 
                    break;
                case SDLK_ESCAPE: 
                    isRunning = false; 
                    saveHighScore();
                    break;
                default: break;
            }
            // Move the block with collision checks
            if(!gameIsOver && !gameIsPaused && block)
                moveBlockWithCollision(dx, dy);
        } 
    }
}

void Game::render() 
{
    
     // Prepare the frame
     sdlManager.prepareFrame(cameraX, cameraY, cameraZ, cameraAngleX, cameraAngleY);
     

    // Render parked blocks with null pointer checks
    for (const auto& parkedBlock : parkedBlocks) {
        if (parkedBlock) {  // Check for null pointer before rendering
            parkedBlock->render(false);
        }
    }

    // Render the active block
    if (block) {
        block->render(true);
        
        // Render ghost block (semi-transparent preview) if enabled
        if (showGhostBlock) {
            Block* ghostBlock = calculateGhostBlock();
            if (ghostBlock) {
                // Render ghost block with wireframe to show where it will land
                ghostBlock->render(false); // Use wireframe rendering for ghost
                delete ghostBlock; // Clean up temporary ghost block
            }
        }
    }

    /*
        Render the scene
        
        SCENE_LIMIT Radius to make the total scene 9x9x9
        CELL_SIZE Size of each cell in the grid
        GRID_SIZE Number of cells in each direction (9x9)
        color Green color for the wireframe
    */
    
    sdlManager.renderScene(SCENE_LIMIT, CELL_SIZE, GRID_SIZE, Color(0.0f, 1.0f, 1.0f));
    // Render the GUI
    state->render();
    
    // Swap the window
    sdlManager.swapWindow();
}


void Game::update() 
{
    if (block && !gameIsOver && !gameIsPaused) {
        // Use dynamic fall speed instead of constant
        float dz = currentFallSpeed;

        
        // Check if the block can move or if there's a collision
        if (!checkCollisionWithWalls(block, dz) && !checkCollisionWithParkedBlocks(block, dz)) {
            block->move(0.0f, 0.0f, dz);  // Move the block towards the back
        } else {
            // Collision detected: Park the block and generate a new one
            dropBlock();
        }
    }
}

bool Game::checkCollisionWithParkedBlocks(Block* block, float dz) 
{
    for (const auto& cube : block->cubes) {
        int nextZ = static_cast<int>(Utils::round(cube.z + dz));
        if (occupiedPositions.count(std::make_tuple(static_cast<int>(Utils::round(cube.x)), static_cast<int>(Utils::round(cube.y)), nextZ)) > 0) {
            return true;  // Collision detected
        }
    }
    return false;
}

bool Game::checkCollisionWithWalls(Block* block, float dz) 
{
    for (const auto& cube : block->cubes) {
        if (cube.z + dz < SCENE_BACK_LIMIT) {  // Use constant instead of magic number
            return true;
        }
    }
    return false;
}



// Clean up resources
void Game::clean() {
    data->saveGameState(score, blockCount, cubeCount, highScore, gameIsOver, gameIsPaused, parkedBlocks);
    if (block) {
        delete block;
        block = nullptr;
    }

    sdlManager.clean();
}

void Game::moveBlockWithCollision(float dx, float dy) 
{
    bool canMove = true;

    // Check if each cube in the block will remain within the limits
    for (const auto& cube : block->cubes) {
        float newX = cube.x + dx;
        float newY = cube.y + dy;

        if (newX < -SCENE_LIMIT || newX > SCENE_LIMIT || newY < -SCENE_LIMIT || newY > SCENE_LIMIT) {
            canMove = false;
            break;  // If any cube goes out of bounds, movement is not allowed
        }
    }

    // Move the block if all cubes are within limits
    if (canMove) {
        block->move(dx, dy, 0.0f);
    }
}

void Game::resetGame(bool clearAll) 
{
    // Clear parked blocks and occupied positions safely
    for (auto& parkedBlock : parkedBlocks) {
        if (parkedBlock) {  // Check for null pointer before delete
            delete parkedBlock;
            parkedBlock = nullptr;  // Set to null after delete to prevent double delete
        }
    }
    parkedBlocks.clear();
    occupiedPositions.clear();

    // Delete current block if it exists to prevent memory leak
    if (block) {
        delete block;
        block = nullptr;
    }

    // Reset the closest depth and create a new block
    stackPosition = -1;
    gameIsOver = false;
    gameIsPaused = false;
    block = new Block(0.0f, 0.0f, initZ, getNextBlockType());

    if(clearAll){
        score = 0;
        blockCount = 0;
        cubeCount = 0;
        
        // Reset level progression only on complete reset
        currentLevel = 1;
        currentFallSpeed = AUTOMATIC_FALL_SPEED;
        linesCleared = 0;
        targetLinesForNextLevel = LINES_PER_LEVEL;
    }
}

void Game::dropBlock() 
{
    if (block && !gameIsOver){
        while (true) {
            if (!checkCollisionWithWalls(block, MANUAL_DROP_SPEED) && !checkCollisionWithParkedBlocks(block, MANUAL_DROP_SPEED)) {
                block->move(0.0f, 0.0f, MANUAL_DROP_SPEED);  // Use constant instead of magic number
            } else {
                // Collision detected: Align and park the block
                for (auto& cube : block->cubes) {
                    cube.x = Utils::round(cube.x);  // Align to a multiple of 1 on the X-axis
                    cube.y = Utils::round(cube.y);  // Align to a multiple of 1 on the Y-axis
                    cube.z = Utils::round(cube.z);  // Align to a multiple of 1 on the Z-axis
                }

                // Record the occupied positions of each cube in the block
                for (const auto& cube : block->cubes) {
                    occupiedPositions.insert(std::make_tuple(static_cast<int>(Utils::round(cube.x)), static_cast<int>(Utils::round(cube.y)), static_cast<int>(Utils::round(cube.z))));
                }

                // Add the block to the list of parked blocks
                parkedBlocks.push_back(block);

                // Calculate the maximum z value in the block
                int maxZ = MIN_Z_COORDINATE;  // Use constant instead of magic number
                for (const auto& cube : block->cubes) {
                    maxZ = std::max(maxZ, static_cast<int>(Utils::round(cube.z)));
                }

                stackPosition = calculateStackPosition();

                blockCount++;
                cubeCount  += block->getCubeCount();
                score += block->getCubeCount()+blockCount;

                if (!sdlManager.isPlayingSound()) {
                    playSound(PARKED_SOUND);
                }

                // Create a new block at the entry point
                block = new Block(0.0f, 0.0f, initZ, getNextBlockType());

                if(isAnyDepthLevelFull()){
                    linesCleared++;  // Increment cleared lines counter
                    playSound(LINE_CLEAR_SOUND); // Play line clear sound
                    updateLevel();   // Check for level progression
                    score += LEVEL_CLEAR_BONUS;  // Use constant instead of magic number
                    resetGame();
                }
                break;  // Exit the loop since the block has been parked
            }
        }
    }
}

// Update stackPosition based on all parked blocks
int Game::calculateStackPosition() 
{
    // If no blocks are parked, return -1 to indicate empty stack
    if (parkedBlocks.empty()) {
        return -1;
    }
    
    int maxZ = MIN_Z_COORDINATE; // Use constant instead of magic number
    for (const auto& parkedBlock : parkedBlocks) {
        if (parkedBlock) {  // Check for null pointer before accessing cubes
            for (const auto& cube : parkedBlock->cubes) {
                maxZ = std::max(maxZ, static_cast<int>(Utils::round(cube.z)));
            }
        }
    }

    // Map the maximum Z value to the stack position
    return Utils::mapDepth(maxZ);
}

void Game::playSound(int sound)
{
    // Stop any sound that is currently playing
    
    sdlManager.stopSound();
    if(soundEnabled){
        switch (sound)
        {
            case PARKED_SOUND:
                sdlManager.playSound(parkedSound);
                break;
            case KEY_PRESS_SOUND:
                sdlManager.playSound(keyPressSound);
                break;
            case GAME_OVER_SOUND:
                sdlManager.playSound(gameOverSound);
                break;
            case LEVEL_UP_SOUND:
                sdlManager.playSound(levelUpSound);
                break;
            case LINE_CLEAR_SOUND:
                sdlManager.playSound(lineClearSound);
                break;
            default:
                break;
        }
    }
}

bool Game::isAnyDepthLevelFull() 
{
    // Create a map to store the positions (x, y) that are occupied at each level.
    std::map<int, std::set<std::pair<int, int>>> occupiedPositionsAtDepth;

    // Iterate over all parked blocks.
    for (const auto& parkedBlock : parkedBlocks) {
        if (parkedBlock) {  // Check for null pointer before accessing cubes
            // Iterate over all cubes of the parked block.
            for (const auto& cube : parkedBlock->cubes) {
                int depthLevel = Utils::mapDepth(static_cast<int>(Utils::round(cube.z)));
                // Store the position (x, y) of that cube at the corresponding depth level.
                occupiedPositionsAtDepth[depthLevel].insert({static_cast<int>(Utils::round(cube.x)), static_cast<int>(Utils::round(cube.y))});
            }
        }
    }

    // Check if any level is full.
    for (const auto& depthPositionsPair : occupiedPositionsAtDepth) {
        if (depthPositionsPair.second.size() == GRID_SIZE * GRID_SIZE) {  // Use constant instead of magic number
            return true; // Return true if any level is full.
        }
    }

    return false; // Return false if no level is full.
}


bool Game::isDepthLevelFull(int depthLevel) 
{
    // Create a set to store the positions (x, y) that are occupied at the specified level.
    std::set<std::pair<int, int>> occupiedPositionsAtDepth;

    // Iterate over all parked blocks.
    for (const auto& parkedBlock : parkedBlocks) {
        if (parkedBlock) {  // Check for null pointer before accessing cubes
            // Iterate over all cubes of the parked block.
            for (const auto& cube : parkedBlock->cubes) {
                // If the cube is at the depth level we want to check.
                if(Utils::mapDepth(static_cast<int>(Utils::round(cube.z))) == depthLevel){
                    // Store the position (x, y) of that cube.
                    occupiedPositionsAtDepth.insert({static_cast<int>(Utils::round(cube.x)), static_cast<int>(Utils::round(cube.y))});
                }
            }
        }
    }

    // Check if all possible positions at the level are occupied.
    // If the number of occupied positions at that level equals the grid size (GRID_SIZE * GRID_SIZE), the level is full.
    return occupiedPositionsAtDepth.size() == GRID_SIZE * GRID_SIZE;  // Use constant instead of magic number
}

 void Game::saveHighScore()
 {
    highScore = std::max(highScore, score);
    data->writeData(highScore);
 }
 
 // New function to update level progression
 void Game::updateLevel() {
     if (linesCleared >= targetLinesForNextLevel) {
         currentLevel++;
         linesCleared = 0;
         targetLinesForNextLevel = LINES_PER_LEVEL * currentLevel; // Progressive target
         
         // Increase fall speed but cap it
         currentFallSpeed *= SPEED_INCREASE_FACTOR;
         if (currentFallSpeed < MAX_FALL_SPEED) {
             currentFallSpeed = MAX_FALL_SPEED;
         }
         
         // Level bonus
         score += LEVEL_SCORE_MULTIPLIER * currentLevel;
         
         // Visual/audio feedback could be added here
         playSound(LEVEL_UP_SOUND); // Use dedicated level up sound
     }
 }
 
 // Generate preview blocks queue
 void Game::generateNextBlocks() {
     // Fill the preview queue if it's empty or too small
     while (nextBlockTypes.size() < PREVIEW_COUNT) {
         nextBlockTypes.push_back(rand() % 9); // 0-8 for 9 block types
     }
 }
 
 // Get next block type and update queue
 int Game::getNextBlockType() {
     if (nextBlockTypes.empty()) {
         generateNextBlocks();
     }
     
     int nextType = nextBlockTypes.front();
     nextBlockTypes.erase(nextBlockTypes.begin());
     generateNextBlocks(); // Refill queue
     
     return nextType;
 }
 
 // Calculate ghost block position (where the current block will land)
 Block* Game::calculateGhostBlock() {
     if (!block) return nullptr;
     
     // Create a copy of the current block
     Block* ghostBlock = new Block(0.0f, 0.0f, 0.0f, 0); // Temporary block
     ghostBlock->cubes = block->cubes; // Copy current block's cubes
     
     // Move the ghost block down until it would collide
     while (true) {
         if (checkCollisionWithWalls(ghostBlock, MANUAL_DROP_SPEED) || 
             checkCollisionWithParkedBlocks(ghostBlock, MANUAL_DROP_SPEED)) {
             break; // Stop when collision would occur
         }
         ghostBlock->move(0.0f, 0.0f, MANUAL_DROP_SPEED);
     }
     
     return ghostBlock;
 }
 


