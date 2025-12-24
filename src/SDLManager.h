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

#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include "Color.h"

#define TARGET_FPS 60
#define FRAME_DELAY (1000 / TARGET_FPS)

class SDLManager {
public:
    SDLManager();
    ~SDLManager();
    Uint32 getTicks() { return SDL_GetTicks(); }

    bool init();
    void clean();
    void swapWindow();
    void prepareFrame(float cameraX, float cameraY, float cameraZ, float cameraAngleX, float cameraAngleY);
    void delayFrame(Uint32 frameStart);
    void playSound(Mix_Music *sound);
    bool isPlayingSound();
    void stopSound();
    void static drawCube(bool wireframe,float x, float y, float z, float size, float r, float g, float b);
    int getEvent(SDL_Event* event);
    void renderScene(float size,float cellSize,float gridCount, Color color);

    SDL_Window* getWindow() const { return window; }
    SDL_GLContext getGLContext() const { return glContext; }
    Mix_Music* loadSound(const char* path);
    //std::array<Color, 9> colorArray;



private:
    SDL_Window* window;
    SDL_GLContext glContext;
};

#endif // SDLMANAGER_H
