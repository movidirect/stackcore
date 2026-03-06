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
    UnloadShader(lightingShader);
}

void Renderer::init() {
    // Initialize stars
    initStars();

    // Load lighting shader
    lightingShader = LoadShader("shaders/lighting.vs", "shaders/lighting.fs");

    // Get uniform locations
    lightingShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lightingShader, "viewPos");
    lightDirLoc = GetShaderLocation(lightingShader, "lightDir");
    viewPosLoc = GetShaderLocation(lightingShader, "viewPos");

    // Set light direction (pointing slightly down and left)
    Vector3 lightDir = { -1.0f, -1.0f, -1.0f };
    SetShaderValue(lightingShader, lightDirLoc, &lightDir, SHADER_UNIFORM_VEC3);
}

void Renderer::initStars() {
    stars.clear();
    // Create 300 stars scattered in a wide area behind the grid
    for (int i = 0; i < 300; i++) {
        Star s;
        // Distribute stars widely in X and Y, and deeply in Z (behind the grid)
        s.position.x = (float)GetRandomValue(-400, 400) / 10.0f;
        s.position.y = (float)GetRandomValue(-400, 400) / 10.0f;
        s.position.z = (float)GetRandomValue(-600, -100) / 10.0f; 
        
        // Varying speeds for parallax effect
        s.speed = (float)GetRandomValue(5, 20) / 100.0f;
        
        // Randomize brightness (alpha) for twinkling effect
        unsigned char brightness = GetRandomValue(100, 255);
        s.color = { brightness, brightness, brightness, 255 }; // White to grey stars
        
        stars.push_back(s);
    }
}

void Renderer::drawStars(float deltaTime) {
    for (auto& star : stars) {
        // Move stars forward on the Z axis
        star.position.z += star.speed * deltaTime * 60.0f; // Scale by roughly 60fps to match typical speed
        
        // If a star passes the camera, reset it far back
        if (star.position.z > camera.position.z + 5.0f) {
            star.position.z = (float)GetRandomValue(-600, -500) / 10.0f;
            star.position.x = (float)GetRandomValue(-400, 400) / 10.0f;
            star.position.y = (float)GetRandomValue(-400, 400) / 10.0f;
        }
        
        // Draw the star as a tiny cube or point
        // Using a tiny point/box makes it look like a star
        DrawCubeV(star.position, (Vector3){0.05f, 0.05f, 0.05f}, star.color);
    }
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
    bool demoMode, bool showHelpWindow
    ) {    // Update camera position based on current logic
    camera.position = (Vector3){ -cameraX, -cameraY, -cameraZ };
    camera.target = (Vector3){ -cameraX, -cameraY, 0.0f };
    
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera);
    
    // Draw the animated starfield
    drawStars(GetFrameTime());

    rlPushMatrix();
    rlRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    rlRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);

    // Update camera position for shader specular reflection
    float cameraPos[3] = { camera.position.x, camera.position.y, camera.position.z };
    SetShaderValue(lightingShader, viewPosLoc, cameraPos, SHADER_UNIFORM_VEC3);

    // Enable Lighting Shader
    BeginShaderMode(lightingShader);

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

    EndShaderMode();

    // Render the grid scene (without lighting)
    drawGrid();
    
    rlPopMatrix();

    EndMode3D();

    // Render the GUI (rlImGui)
    rlImGuiBegin();
    state->draw(stackPosition, score, highScore, blockCount, cubeCount,
                gameIsOver, gameIsPaused, soundEnabled,
                currentLevel, linesCleared, targetLinesForNextLevel,
                currentFallSpeed, showGhostBlock, nextBlockTypes,
                demoMode, showHelpWindow);
    rlImGuiEnd();

    EndDrawing();
}

void Renderer::drawGrid() {
    float size = SCENE_LIMIT;
    float cellSize = CELL_SIZE;
    int gridCount = GRID_SIZE;
    ::Color gridColor = {0, 255, 255, 160}; // More solid cyan
    ::Color sideGridColor = {0, 255, 255, 100}; // Much more visible than before (was 40)

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
        DrawLine3D((Vector3){-size, position, -size}, (Vector3){-size, position, size}, sideGridColor);
        DrawLine3D((Vector3){-size, -size, position}, (Vector3){-size, size, position}, sideGridColor);
    }

    // Render right wall (YZ plane at x = size)
    for (int i = 0; i <= gridCount; ++i) {
        float position = -size + i * cellSize;
        DrawLine3D((Vector3){size, position, -size}, (Vector3){size, position, size}, sideGridColor);
        DrawLine3D((Vector3){size, -size, position}, (Vector3){size, size, position}, sideGridColor);
    }

    // Render back wall (XY plane at z = -size)
    for (int i = 0; i <= gridCount; ++i) {
        float position = -size + i * cellSize;
        DrawLine3D((Vector3){position, -size, -size}, (Vector3){position, size, -size}, gridColor);
        DrawLine3D((Vector3){-size, position, -size}, (Vector3){size, position, -size}, gridColor);
    }
}
