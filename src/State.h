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

#ifndef STATE_H
#define STATE_H

#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl2.h"
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <imgui.h>
#include <vector>

class State {
public:
    State(SDL_Window* window, SDL_GLContext glContext);
    void draw(int stackPosition,int score,int highScore,int cubesPlaced,int blocksPlaced, bool gameIsOver, bool paused, bool soundEnabled, int level = 1, int linesCleared = 0, int linesTarget = 10, float currentSpeed = 0.02f, bool showGhost = true, const std::vector<int>* nextBlocks = nullptr);
    void render();

private:
    void showImageInImGui(const char* name,GLuint textureID, ImVec2 position, ImVec2 size);
    GLuint loadTexture(const char* imagePath);
    
    // New functions for organized UI panels
    void drawGameStats(int level, int linesCleared, int linesTarget, float currentSpeed);
    void drawScoreInfo(int score, int highScore, int blocks, int cubes);
    void drawControlsHelp();
    void drawNextBlocksPreview(const std::vector<int>* nextBlocks);
    void drawGameStatus(bool gameIsOver, bool paused, bool soundEnabled, bool showGhost);
    void drawEnhancedStackIndicator(int stackPosition, bool gameIsOver);

    GLuint textureLogo;
    GLuint textureGameOver;
    GLuint texturePaused;
    GLuint textureSoundEnabled;
    GLuint textureSoundDisabled;
    SDL_Window* window;
    SDL_GLContext glContext;
    ImFont* myFont;
    ImFont* smallFont;


};

#endif // STATE_H


