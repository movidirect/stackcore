#include "Renderer.h"
#include "rlImGui.h"
#include <rlgl.h>

Renderer::Renderer() {
    // Initialize Camera3D defaults
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

Renderer::~Renderer() {
}

void Renderer::init() {
    // Initialization if needed
}

void Renderer::render(
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
) {
    // Update camera position based on current logic
    camera.position = (Vector3){ -cameraX, -cameraY, -cameraZ };
    camera.target = (Vector3){ -cameraX, -cameraY, 0.0f };
    
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera);
    
    rlPushMatrix();
    rlRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    rlRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);

    // Render parked blocks
    for (const auto& parkedBlock : parkedBlocks) {
        if (parkedBlock) {
            parkedBlock->render(false);
        }
    }

    // Render the active block
    if (activeBlock) {
        activeBlock->render(true);
        
        if (showGhostBlock && ghostBlock) {
            ghostBlock->render(false);
        }
    }

    // Render the grid scene
    drawGrid();
    
    rlPopMatrix();

    EndMode3D();

    // Render the GUI (rlImGui)
    rlImGuiBegin();
    state->draw(stackPosition, score, highScore, blockCount, cubeCount, 
                gameIsOver, gameIsPaused, soundEnabled, 
                currentLevel, linesCleared, targetLinesForNextLevel, 
                currentFallSpeed, showGhostBlock, nextBlockTypes, demoMode);
    rlImGuiEnd();

    EndDrawing();
}

void Renderer::drawGrid() {
    float size = SCENE_LIMIT;
    float cellSize = CELL_SIZE;
    int gridCount = GRID_SIZE;
    ::Color gridColor = {0, 255, 255, 255}; // Cyan

    // Render floor (XZ plane at y = -size)
    for (int i = 0; i <= gridCount; ++i) {
        float position = -size + i * cellSize;
        DrawLine3D((Vector3){position, -size, -size}, (Vector3){position, -size, size}, gridColor);
        DrawLine3D((Vector3){-size, -size, position}, (Vector3){size, -size, position}, gridColor);
    }

    // Render ceiling (XZ plane at y = size)
    for (int i = 0; i <= gridCount; ++i) {
        float position = -size + i * cellSize;
        DrawLine3D((Vector3){position, size, -size}, (Vector3){position, size, size}, gridColor);
        DrawLine3D((Vector3){-size, size, position}, (Vector3){size, size, position}, gridColor);
    }

    // Render left wall (YZ plane at x = -size)
    for (int i = 0; i <= gridCount; ++i) {
        float position = -size + i * cellSize;
        DrawLine3D((Vector3){-size, position, -size}, (Vector3){-size, position, size}, gridColor);
        DrawLine3D((Vector3){-size, -size, position}, (Vector3){-size, size, position}, gridColor);
    }

    // Render right wall (YZ plane at x = size)
    for (int i = 0; i <= gridCount; ++i) {
        float position = -size + i * cellSize;
        DrawLine3D((Vector3){size, position, -size}, (Vector3){size, position, size}, gridColor);
        DrawLine3D((Vector3){size, -size, position}, (Vector3){size, size, position}, gridColor);
    }

    // Render back wall (XY plane at z = -size)
    for (int i = 0; i <= gridCount; ++i) {
        float position = -size + i * cellSize;
        DrawLine3D((Vector3){position, -size, -size}, (Vector3){position, size, -size}, gridColor);
        DrawLine3D((Vector3){-size, position, -size}, (Vector3){size, position, -size}, gridColor);
    }
}
