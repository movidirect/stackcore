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

#include "SDLManager.h"

SDLManager::SDLManager() : window(nullptr), glContext(nullptr) {}

SDLManager::~SDLManager() {
    clean();
}

bool SDLManager::init() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("StackCore Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error creating the window: " << SDL_GetError() << std::endl;
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "Error creating the OpenGL context: " << SDL_GetError() << std::endl;
        return false;
    }

    // OpenGL configuration
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = 800.0f / 600.0f;
    gluPerspective(45.0, aspectRatio, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

    

    if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640) < 0) {
        std::cerr << "Error initializing SDL_mixer: " << Mix_GetError() << std::endl;
        return false;
    }

    //colorArray = Color::getRandomColors();
    return true;
}

Mix_Music* SDLManager::loadSound(const char* path) {
    return Mix_LoadMUS(path);
}

void SDLManager::clean() {
    if (glContext) {
        SDL_GL_DeleteContext(glContext);
        glContext = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    Mix_CloseAudio();
    SDL_Quit();
}

void SDLManager::swapWindow() {
    SDL_GL_SwapWindow(window);
}


void SDLManager::prepareFrame(float cameraX, float cameraY, float cameraZ, float cameraAngleX, float cameraAngleY) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(cameraX, cameraY, cameraZ);
    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);
}

void SDLManager::delayFrame(Uint32 frameStart) {
    Uint32 frameTime = SDL_GetTicks() - frameStart;
    if (frameTime < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frameTime);
    }
}

bool SDLManager::isPlayingSound() {
    return Mix_PlayingMusic();

}

void SDLManager::stopSound() {
    Mix_HaltMusic();
}

void SDLManager::playSound(Mix_Music *sound) {
            
    Mix_PlayMusic(sound, 0);
}

void SDLManager::drawCube(bool wireframe,float x, float y, float z, float size, float r, float g, float b) {

    glPushMatrix();
    glTranslatef(x, y, z);

    if (!wireframe) {
        glColor3f(r,g,b);  // Solid color of the cube
        glBegin(GL_QUADS);

        // Render all faces of the cube in solid mode
        // Front face
        glVertex3f(-size / 2, -size / 2, size / 2);
        glVertex3f(size / 2, -size / 2, size / 2);
        glVertex3f(size / 2, size / 2, size / 2);
        glVertex3f(-size / 2, size / 2, size / 2);

        // Back face
        glVertex3f(-size / 2, -size / 2, -size / 2);
        glVertex3f(size / 2, -size / 2, -size / 2);
        glVertex3f(size / 2, size / 2, -size / 2);
        glVertex3f(-size / 2, size / 2, -size / 2);

        // Left face
        glVertex3f(-size / 2, -size / 2, -size / 2);
        glVertex3f(-size / 2, -size / 2, size / 2);
        glVertex3f(-size / 2, size / 2, size / 2);
        glVertex3f(-size / 2, size / 2, -size / 2);

        // Right face
        glVertex3f(size / 2, -size / 2, -size / 2);
        glVertex3f(size / 2, -size / 2, size / 2);
        glVertex3f(size / 2, size / 2, size / 2);
        glVertex3f(size / 2, size / 2, -size / 2);

        // Top face
        glVertex3f(-size / 2, size / 2, -size / 2);
        glVertex3f(size / 2, size / 2, -size / 2);
        glVertex3f(size / 2, size / 2, size / 2);
        glVertex3f(-size / 2, size / 2, size / 2);

        // Bottom face
        glVertex3f(-size / 2, -size / 2, -size / 2);
        glVertex3f(size / 2, -size / 2, -size / 2);
        glVertex3f(size / 2, -size / 2, size / 2);
        glVertex3f(-size / 2, -size / 2, size / 2);

        glEnd();

        // Enable polygon offset to avoid z-fighting
        glEnable(GL_POLYGON_OFFSET_LINE);
        glPolygonOffset(-1.0, -1.0);

        // Set line width for solid mode
        glLineWidth(2.0f);  // Adjust the value as needed
    } else {
        // Set line width for wireframe mode
        glLineWidth(1.0f);  // Default line width
    }

    // Draw the edges on all faces
    glColor3f(1.0f, 1.0f, 1.0f);  // Edge color (white)

    // Edges of the front face
    glBegin(GL_LINE_LOOP);
    glVertex3f(-size / 2, -size / 2, size / 2);
    glVertex3f(size / 2, -size / 2, size / 2);
    glVertex3f(size / 2, size / 2, size / 2);
    glVertex3f(-size / 2, size / 2, size / 2);
    glEnd();

    // Edges of the back face
    glBegin(GL_LINE_LOOP);
    glVertex3f(-size / 2, -size / 2, -size / 2);
    glVertex3f(size / 2, -size / 2, -size / 2);
    glVertex3f(size / 2, size / 2, -size / 2);
    glVertex3f(-size / 2, size / 2, -size / 2);
    glEnd();

    // Edges of the left face
    glBegin(GL_LINE_LOOP);
    glVertex3f(-size / 2, -size / 2, -size / 2);
    glVertex3f(-size / 2, -size / 2, size / 2);
    glVertex3f(-size / 2, size / 2, size / 2);
    glVertex3f(-size / 2, size / 2, -size / 2);
    glEnd();

    // Edges of the right face
    glBegin(GL_LINE_LOOP);
    glVertex3f(size / 2, -size / 2, -size / 2);
    glVertex3f(size / 2, -size / 2, size / 2);
    glVertex3f(size / 2, size / 2, size / 2);
    glVertex3f(size / 2, size / 2, -size / 2);
    glEnd();

    // Edges of the top face
    glBegin(GL_LINE_LOOP);
    glVertex3f(-size / 2, size / 2, -size / 2);
    glVertex3f(size / 2, size / 2, -size / 2);
    glVertex3f(size / 2, size / 2, size / 2);
    glVertex3f(-size / 2, size / 2, size / 2);
    glEnd();

    // Edges of the bottom face
    glBegin(GL_LINE_LOOP);
    glVertex3f(-size / 2, -size / 2, -size / 2);
    glVertex3f(size / 2, -size / 2, -size / 2);
    glVertex3f(size / 2, -size / 2, size / 2);
    glVertex3f(-size / 2, -size / 2, size / 2);
    glEnd();

    if (!wireframe) {
        // Disable polygon offset
        glDisable(GL_POLYGON_OFFSET_LINE);
    }

    glPopMatrix();
}


int SDLManager::getEvent(SDL_Event* event) {
    return SDL_PollEvent(event);
}

void SDLManager::renderScene(float size,float cellSize,float gridCount, Color color)
{
    glLineWidth(1.0f);
    glPushMatrix();
    glColor3f(color.getR(),color.getG(),color.getB());

    // Render the floor (XZ plane at y = -size)
    glBegin(GL_LINES);
    for (int i = 0; i <= gridCount; ++i) {
        float position = -size + i * cellSize;
        // Lines parallel to the Z-axis
        glVertex3f(position, -size, -size);
        glVertex3f(position, -size, size);
        // Lines parallel to the X-axis
        glVertex3f(-size, -size, position);
        glVertex3f(size, -size, position);
    }
    glEnd();

    // Render the ceiling (XZ plane at y = size)
    glBegin(GL_LINES);
    for (int i = 0; i <= gridCount; ++i) {
        float position = -size + i * cellSize;
        // Lines parallel to the Z-axis
        glVertex3f(position, size, -size);
        glVertex3f(position, size, size);
        // Lines parallel to the X-axis
        glVertex3f(-size, size, position);
        glVertex3f(size, size, position);
    }
    glEnd();

    // Render the left wall (YZ plane at x = -size)
    glBegin(GL_LINES);
    for (int i = 0; i <= gridCount; ++i) {
        float position = -size + i * cellSize;
        // Lines parallel to the Z-axis
        glVertex3f(-size, position, -size);
        glVertex3f(-size, position, size);
        // Lines parallel to the Y-axis
        glVertex3f(-size, -size, position);
        glVertex3f(-size, size, position);
    }
    glEnd();

    // Render the right wall (YZ plane at x = size)
    glBegin(GL_LINES);
    for (int i = 0; i <= gridCount; ++i) {
        float position = -size + i * cellSize;
        // Lines parallel to the Z-axis
        glVertex3f(size, position, -size);
        glVertex3f(size, position, size);
        // Lines parallel to the Y-axis
        glVertex3f(size, -size, position);
        glVertex3f(size, size, position);
    }
    glEnd();

    // Render the back wall (XY plane at z = -size)
    glBegin(GL_LINES);
    for (int i = 0; i <= gridCount; ++i) {
        float position = -size + i * cellSize;
        // Lines parallel to the Y-axis
        glVertex3f(position, -size, -size);
        glVertex3f(position, size, -size);
        // Lines parallel to the X-axis
        glVertex3f(-size, position, -size);
        glVertex3f(size, position, -size);
    }
    glEnd();

    glPopMatrix();

}