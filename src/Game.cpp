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

#include "Utils.h"
#include "Game.h"
#include "BotAI.h"
#include "rlImGui.h"
#include <rlgl.h>
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
    audioManager = new AudioManager();
    renderer = new Renderer();
    inputHandler = new InputHandler();
    board = new Board();
    block = nullptr;
    cameraX = -0.9f;
    cameraY = 0.0f;
    cameraZ = -16.0f;
    cameraAngleX = 0.0f;
    cameraAngleY = 0.0f;
    targetCameraAngleX = 0.0f;
    targetCameraAngleY = 0.0f;
    initZ = INITIAL_Z_POSITION;

    stackPosition = -1;  // -1 means no blocks parked, 0+ means blocks up to that level
    score = 0;
    blockCount = 0;
    cubeCount = 0;
    gameIsOver = false;
    gameIsPaused = false;
    demoMode = false;
    showHelpWindow = false;
    isFastDropping = false;
    
    // Initialize the AI bot
    bot = new BotAI(this);
    
    // Initialize configuration variables
    showGhostBlock = false;
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
    delete bot;
    delete audioManager;
    delete renderer;
    delete inputHandler;
    delete board;
    clean();
}

bool Game::init() 
{
    // Initialize Raylib window and audio
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(800, 600, "StackCore Game");
    SetExitKey(0); // Disable ESC key closing the window
    ChangeDirectory(GetApplicationDirectory());
    SetTargetFPS(60);

    if (!audioManager->init()) {
        return false;
    }

    renderer->init();

    // Initialize the GUI where the game state will be displayed
    state = new State();

    // Initialize the first block at the entry point
    block = new Block(0.0f, -0.0f, initZ, getNextBlockType());
    if (bot) bot->reset();
    currentBlockX = 0.0f;
    currentBlockY = 0.0f;

    isRunning = true;
    return true;
}

// Main game loop
void Game::run() 
{
    while (!WindowShouldClose() && isRunning) {
        // Update Music Streams (required by Raylib)
        audioManager->update();

        // Handle user input
        handleEvents();

        // Update the game state
        update();

        // Render the game
        render();
        
        // Check if the stack is full
        if (stackPosition >= MAX_STACK_HEIGHT && !gameIsOver) {
            gameIsOver = true;
            audioManager->playSound(GAME_OVER_SOUND);
        }
    }
}

// Handle user input events
void Game::handleEvents()
{
    GameCommands cmds = inputHandler->getPlayerCommands();

    // In demo mode, override with bot commands
    if (demoMode) {
        bot->update();
        auto botCmds = bot->getCommands();
        cmds.moveUp     = botCmds.up;
        cmds.moveDown   = botCmds.down;
        cmds.moveLeft   = botCmds.left;
        cmds.moveRight  = botCmds.right;
        cmds.rotateX    = botCmds.rotX;
        cmds.rotateXRev = botCmds.rotXRev;
        cmds.rotateY    = botCmds.rotY;
        cmds.rotateYRev = botCmds.rotYRev;
        cmds.rotateZ    = botCmds.rotZ;
        cmds.rotateZRev = botCmds.rotZRev;
        cmds.drop       = botCmds.drop;
    }

    // Global keys
    if (cmds.toggleDemo) {
        demoMode = !demoMode;
        if (demoMode) {
            resetGame(true);
        }
    }

    if (cmds.toggleHelp) showHelpWindow = !showHelpWindow;
    if (cmds.togglePause) gameIsPaused = !gameIsPaused;
    if (cmds.resetGame) {
        if (gameIsOver) resetGame();
        else resetGame(true);
    }
    if (cmds.toggleSound) audioManager->toggleSound();
    if (cmds.toggleGhost) { showGhostBlock = !showGhostBlock; audioManager->playSound(KEY_PRESS_SOUND); }
    if (cmds.toggleNext)  { showNextBlocks = !showNextBlocks; audioManager->playSound(KEY_PRESS_SOUND); }
    
    if (cmds.exitGame) {
        if (showHelpWindow) {
            showHelpWindow = false;
        } else {
            isRunning = false;
            saveHighScore();
        }
    }

    if (gameIsOver || gameIsPaused) return;
    
    // Block manual input during visual fast drop
    if (isFastDropping) return;

    float dx = 0.0f;
    float dy = 0.0f;

    if (cmds.moveUp)    { dy =  1.0f; audioManager->playSound(KEY_PRESS_SOUND); }
    if (cmds.moveDown)  { dy = -1.0f; audioManager->playSound(KEY_PRESS_SOUND); }
    if (cmds.moveLeft)  { dx = -1.0f; audioManager->playSound(KEY_PRESS_SOUND); }
    if (cmds.moveRight) { dx =  1.0f; audioManager->playSound(KEY_PRESS_SOUND); }

    if (cmds.rotateX) {
        if (cmds.ctrlPressed) targetCameraAngleX += 15.0f;
        else if (block) block->tryRotateX(SCENE_LIMIT, board);
        audioManager->playSound(KEY_PRESS_SOUND);
    }
    if (cmds.rotateXRev) {
        if (cmds.ctrlPressed) targetCameraAngleX -= 15.0f;
        else if (block) { for(int i=0; i<3; i++) if(!block->tryRotateX(SCENE_LIMIT, board)) break; }
        audioManager->playSound(KEY_PRESS_SOUND);
    }
    
    if (cmds.rotateY) {
        if (cmds.ctrlPressed) targetCameraAngleY += 15.0f;
        else if (block) block->tryRotateY(SCENE_LIMIT, board);
        audioManager->playSound(KEY_PRESS_SOUND);
    }
    if (cmds.rotateYRev) {
        if (cmds.ctrlPressed) targetCameraAngleY -= 15.0f;
        else if (block) { for(int i=0; i<3; i++) if(!block->tryRotateY(SCENE_LIMIT, board)) break; }
        audioManager->playSound(KEY_PRESS_SOUND);
    }
    
    if (cmds.rotateZ) {
        if (block) block->tryRotateZ(SCENE_LIMIT, board);
        audioManager->playSound(KEY_PRESS_SOUND);
    }
    if (cmds.rotateZRev) {
        if (block) { for(int i=0; i<3; i++) if(!block->tryRotateZ(SCENE_LIMIT, board)) break; }
        audioManager->playSound(KEY_PRESS_SOUND);
    }

    if (cmds.drop && block) {
        if (demoMode) {
            dropBlock();
        } else {
            isFastDropping = true;
        }
    }

    if (block && (dx != 0.0f || dy != 0.0f)) {
        moveBlockWithCollision(dx, dy);
    }
}

void Game::render() 
{
    Block* ghostBlock = nullptr;
    if (showGhostBlock) {
        ghostBlock = calculateGhostBlock();
    }

    renderer->render(
        cameraAngleX, cameraAngleY,
        cameraX, cameraY, cameraZ,
        board->getParkedBlocks(),
        block,
        ghostBlock,
        state,
        stackPosition, score, highScore, blockCount, cubeCount,
        gameIsOver, gameIsPaused, audioManager->isSoundEnabled(),
        currentLevel, linesCleared, targetLinesForNextLevel,
        currentFallSpeed, showGhostBlock, &nextBlockTypes,
        demoMode, showHelpWindow
    );

    if (ghostBlock) delete ghostBlock;
}

void Game::update() 
{
    // Interpolate camera angles for smooth rotation
    cameraAngleX += (targetCameraAngleX - cameraAngleX) * 0.1f;
    cameraAngleY += (targetCameraAngleY - cameraAngleY) * 0.1f;

    if (block && !gameIsOver && !gameIsPaused) {
        // During fast drop, piece moves 1 full unit per frame (very fast, visible)
        float dz = isFastDropping ? MANUAL_DROP_SPEED : currentFallSpeed;

        // Check if the block can move or if there's a collision
        if (!checkCollisionWithWalls(block, dz) && !board->checkCollision(block, dz)) {
            block->move(0.0f, 0.0f, dz);  // Move the block towards the back
            if (isFastDropping) score += 2; // Hard drop bonus (frame by frame)
        } else {
            // Collision detected:
            if (isFastDropping) isFastDropping = false; // Stop manual animation
            
            // Call dropBlock to immediately park the block (collision detected)
            dropBlock();
        }
    }
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
    data->saveGameState(score, blockCount, cubeCount, highScore, gameIsOver, gameIsPaused, board->getParkedBlocks());
    if (block) {
        delete block;
        block = nullptr;
    }

    // AudioManager handles audio cleanup
    audioManager->clean();

    CloseWindow();
}

void Game::moveBlockWithCollision(float dx, float dy) 
{
    bool canMove = true;

    // First check bounds
    for (const auto& cube : block->cubes) {
        float newX = cube.x + dx;
        float newY = cube.y + dy;

        if (newX < -SCENE_LIMIT || newX > SCENE_LIMIT || newY < -SCENE_LIMIT || newY > SCENE_LIMIT) {
            canMove = false;
            break;  // If any cube goes out of bounds, movement is not allowed
        }
    }

    if (canMove) {
        // Create a temporary block to check horizontal collisions
        Block tempBlock = *block;
        tempBlock.move(dx, dy, 0.0f);
        
        // Use dz=0 to check collision at the current z depth but new x,y position
        if (board->checkCollision(&tempBlock, 0.0f)) {
            canMove = false;
        }
    }

    // Move the block if all cubes are within limits and no collision
    if (canMove) {
        block->move(dx, dy, 0.0f);
        currentBlockX += dx;
        currentBlockY += dy;
    }
}

void Game::resetGame(bool clearAll) 
{
    // Clear parked blocks and occupied positions safely using Board
    board->reset();

    // Delete current block if it exists to prevent memory leak
    if (block) {
        delete block;
        block = nullptr;
    }

    // Reset the closest depth and create a new block
    stackPosition = -1;
    gameIsOver = false;
    gameIsPaused = false;
    isFastDropping = false;
    block = new Block(0.0f, 0.0f, initZ, getNextBlockType());
    if (bot) bot->reset();
    currentBlockX = 0.0f;
    currentBlockY = 0.0f;

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
        int dropDistance = 0;
        while (true) {
            if (!checkCollisionWithWalls(block, MANUAL_DROP_SPEED) && !board->checkCollision(block, MANUAL_DROP_SPEED)) {
                block->move(0.0f, 0.0f, MANUAL_DROP_SPEED);  // Use constant instead of magic number
                dropDistance++;
            } else {
                // Collision detected: Align and park the block using Board
                board->parkBlock(block);

                blockCount++;
                cubeCount  += block->getCubeCount();
                score += block->getCubeCount() * currentLevel;
                score += dropDistance * 2; // Hard drop bonus

                if (!audioManager->isPlaying(PARKED_SOUND)) {
                    audioManager->playSound(PARKED_SOUND);
                }

                int lines = board->clearFullLevels();
                if(lines > 0){
                    linesCleared += lines;
                    audioManager->playSound(LINE_CLEAR_SOUND);
                    updateLevel();
                    
                    int lineScore = 0;
                    switch (lines) {
                        case 1: lineScore = 100; break;
                        case 2: lineScore = 300; break;
                        case 3: lineScore = 500; break;
                        case 4: lineScore = 800; break; // Blockout!
                        default: lineScore = 1000; break; 
                    }
                    score += lineScore * currentLevel;
                    
                    // Perfect Clear Bonus
                    if (board->getParkedBlocks().empty()) {
                        score += 10000 * currentLevel;
                    }
                }
                
                // Calculate stack position after blocks have potentially dropped
                stackPosition = board->calculateStackPosition();

                // Create a new block at the entry point
                block = new Block(0.0f, 0.0f, initZ, getNextBlockType());
                if (bot) bot->reset();
                currentBlockX = 0.0f;
                currentBlockY = 0.0f;

                break;  // Exit the loop since the block has been parked
            }
        }
    }
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
         audioManager->playSound(LEVEL_UP_SOUND); // Use dedicated level up sound
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
             board->checkCollision(ghostBlock, MANUAL_DROP_SPEED)) {
             break; // Stop when collision would occur
         }
         ghostBlock->move(0.0f, 0.0f, MANUAL_DROP_SPEED);
     }
     
     return ghostBlock;
 }
 
void Game::setDemoMode(bool enable) {
    demoMode = enable;
}
 


