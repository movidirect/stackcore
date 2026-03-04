#ifndef RENDERER_H
#define RENDERER_H

#include <raylib.h>
#include <vector>
#include "Block.h"
#include "State.h"
#include "AudioManager.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void init();
    void render(
        float cameraAngleX, float cameraAngleY,
        float cameraX, float cameraY, float cameraZ,
        const std::vector<Block*>& parkedBlocks,
        Block* activeBlock,
        Block* ghostBlock,
        State* state,
        int stackPosition, int score, int highScore, int blockCount, int cubeCount,
        bool gameIsOver, bool gameIsPaused, bool soundEnabled,
        int currentLevel, int linesCleared, int targetLinesForNextLevel,
        float currentFallSpeed, bool showGhostBlock, std::vector<int>* nextBlockTypes,
        bool demoMode
    );

private:
    void drawGrid();
    
    Camera3D camera;
    
    // Constants for rendering (matching Game's limits)
    const float SCENE_LIMIT = 4.5f;
    const float CELL_SIZE = 1.0f;
    const int GRID_SIZE = 9;
};

#endif // RENDERER_H
