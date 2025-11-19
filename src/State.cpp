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

#include "State.h"
#include <iostream>
#include "Color.h"
#include <cmath>

State::State(SDL_Window* window, SDL_GLContext glContext)
{
    this->window = window;
    this->glContext = glContext;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL2_Init();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    myFont = io.Fonts->AddFontDefault();
    smallFont = io.Fonts->AddFontDefault();
    if (myFont == nullptr) {
        std::cerr << "Error loading font" << std::endl;
    }

    textureLogo = loadTexture("images/blockimage.png");
    textureGameOver = loadTexture("images/gameisover.png");
    texturePaused = loadTexture("images/gamepaused.png");
    textureSoundEnabled = loadTexture("images/sound_enabled.png");
    textureSoundDisabled = loadTexture("images/sound_disabled.png");
}

void State::draw(int stackPosition, int score, int highScore, int blocksPlaced, int cubesPlaced, bool gameIsOver, bool gameIspaused, bool soundEnabled, int level, int linesCleared, int linesTarget, float currentSpeed, bool showGhost, const std::vector<int>* nextBlocks)
{
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                 = ImVec4(0.00f, 1.00f, 1.00f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.20f, 0.20f, 0.20f, 0.54f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.00f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.00f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.00f, 0.50f, 0.50f, 0.40f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.00f, 0.80f, 0.80f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.00f, 0.60f, 0.60f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.50f, 0.50f, 0.31f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.80f, 0.80f, 0.80f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.00f, 0.60f, 0.60f, 1.00f);

    // Flags for windows without headers
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    // LEFT PANEL (Stack Indicator)
    drawEnhancedStackIndicator(stackPosition, gameIsOver);

    // RIGHT PANEL (Game Info)
    float rightPanelX = 635.0f;
    float panelY = 16.0f;
    
    ImGui::SetNextWindowPos(ImVec2(rightPanelX, panelY), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(150, 568), ImGuiCond_Always);
    ImGui::Begin("main panel", NULL, window_flags);

    // Logo - positioned relative to panel
    showImageInImGui("textureLogo", textureLogo, ImVec2(rightPanelX + 12, panelY-7), ImVec2(120.0f, 120.0f));
    
    // Game status overlays
    if (gameIsOver) {
        showImageInImGui("textureGameOver", textureGameOver, ImVec2(rightPanelX + 3, panelY + 104), ImVec2(135.0f, 30.0f));
    } else if (gameIspaused) {
        showImageInImGui("texturePaused", texturePaused, ImVec2(rightPanelX + 3, panelY + 104), ImVec2(135.0f, 30.0f));
    }

    // Draw organized sections
    drawGameStats(level, linesCleared, linesTarget, currentSpeed);
    drawScoreInfo(score, highScore, blocksPlaced, cubesPlaced);
    drawNextBlocksPreview(nextBlocks);
    drawGameStatus(gameIsOver, gameIspaused, soundEnabled, showGhost);
    drawControlsHelp();

    ImGui::End();
    ImGui::Render();
}

void State::showImageInImGui(const char* name, GLuint textureID, ImVec2 position, ImVec2 size)
{
    ImGui::SetNextWindowPos(position, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    ImGui::Begin(name, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

    // Use ImGui::Image to display the texture
    ImGui::Image(reinterpret_cast<void*>(textureID), size);

    ImGui::End();
}

GLuint State::loadTexture(const char* imagePath) 
{
    SDL_Surface* imageSurface = IMG_Load(imagePath);
    if (!imageSurface) {
        std::cerr << "Error loading image " << IMG_GetError() << std::endl;
        return 0;
    }

    SDL_Surface* optimizedSurface = SDL_ConvertSurfaceFormat(imageSurface, SDL_PIXELFORMAT_RGBA32, 0);
    if (!optimizedSurface) {
        std::cerr << "Error optimizing image: " << IMG_GetError() << std::endl;
        SDL_FreeSurface(imageSurface);
        return 0;
    }
    SDL_FreeSurface(imageSurface);
    imageSurface = optimizedSurface;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSurface->w, imageSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageSurface->pixels);

    SDL_FreeSurface(imageSurface);  // Free the surface
    return textureID;
}

void State::render()
{
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

// ═══════════════════════════════════════════════════════════════
// MODULAR UI SECTIONS
// ═══════════════════════════════════════════════════════════════

void State::drawGameStats(int level, int linesCleared, int linesTarget, float currentSpeed)
{
    ImGui::SetCursorPos(ImVec2(10, 140));
    ImGui::Text("--- GAME STATUS ---");
    
    ImGui::SetCursorPos(ImVec2(10, 160));
    ImGui::Text("Level:        %2d", level);
    
    ImGui::SetCursorPos(ImVec2(10, 175));
    ImGui::Text("Progress:   %d/%d", linesCleared, linesTarget);
    
    
    ImGui::SetCursorPos(ImVec2(10, 190));
    ImGui::Text("Speed:     %.3f", -currentSpeed); // Show positive value
}

void State::drawScoreInfo(int score, int highScore, int blocks, int cubes)
{
    ImGui::SetCursorPos(ImVec2(10, 210));
    ImGui::Text("--- STATISTICS ----");
    
    ImGui::SetCursorPos(ImVec2(10, 230));
    if (score <= 99999) {
        ImGui::Text("Score:    %6d", score);
    } else {
        ImGui::Text("Score:  %8d", score);
    }
    
    ImGui::SetCursorPos(ImVec2(10, 245));
    if (highScore <= 99999) {
        ImGui::Text("Best:     %6d", highScore);
    } else {
        ImGui::Text("Best:   %8d", highScore);
    }
    
    ImGui::SetCursorPos(ImVec2(10, 260));
    ImGui::Text("Blocks:     %4d", blocks);
    
    ImGui::SetCursorPos(ImVec2(10, 275));
    ImGui::Text("Cubes:      %4d", cubes);
    
    // Calculate some interesting stats
    float cubesPerBlock = blocks > 0 ? (float)cubes / (float)blocks : 0.0f;
    ImGui::SetCursorPos(ImVec2(10, 290));
    ImGui::Text("Avg C/B:     %.1f", cubesPerBlock);
}

void State::drawNextBlocksPreview(const std::vector<int>* nextBlocks)
{
    ImGui::SetCursorPos(ImVec2(10, 315));
    ImGui::Text("--- NEXT BLOCKS ---");
    
    if (nextBlocks && nextBlocks->size() >= 3) {
        const char* blockNames[] = {
            "Single", "I-2", "I-3", "L-3", "L-4", "Square", "T-4", "I-4", "Cross"
        };
        
        ImGui::SetCursorPos(ImVec2(10, 340));
        ImGui::Text("Next:   %s", blockNames[(*nextBlocks)[0] % 9]);
        
        ImGui::SetCursorPos(ImVec2(10, 355));
        ImGui::Text("Then:   %s", blockNames[(*nextBlocks)[1] % 9]);
        
        ImGui::SetCursorPos(ImVec2(10, 370));
        ImGui::Text("After:  %s", blockNames[(*nextBlocks)[2] % 9]);
    } else {
        ImGui::SetCursorPos(ImVec2(10, 340));
        ImGui::Text("Preview: N/A");
    }
}

void State::drawGameStatus(bool gameIsOver, bool paused, bool soundEnabled, bool showGhost)
{
    ImGui::SetCursorPos(ImVec2(10, 395));
    ImGui::Text("---- SETTINGS  ----");
    
    ImGui::SetCursorPos(ImVec2(10, 415));
    ImGui::Text("Sound:    %s", soundEnabled ? "ON " : "OFF");
    
    ImGui::SetCursorPos(ImVec2(10, 430));
    ImGui::Text("Ghost:    %s", showGhost ? "ON " : "OFF");
    
    ImGui::SetCursorPos(ImVec2(10, 445));
    if (gameIsOver) {
        ImGui::Text("Status:   GAME OVER");
    } else if (paused) {
        ImGui::Text("Status:   PAUSED");
    } else {
        ImGui::Text("Status:   PLAYING");
    }
}

void State::drawControlsHelp()
{
    if (smallFont) {
        ImGui::PushFont(smallFont);
    }
    
    ImGui::SetCursorPos(ImVec2(10, 465));
    ImGui::Text("---- CONTROLS -----");
    
    ImGui::SetCursorPos(ImVec2(5, 480));
    ImGui::Text("Arrows  Move");
    
    ImGui::SetCursorPos(ImVec2(5, 490));
    ImGui::Text("SPACE   Drop");
    
    ImGui::SetCursorPos(ImVec2(5, 500));
    ImGui::Text("WASD    Rotate XY");
    
    ImGui::SetCursorPos(ImVec2(5, 510));
    ImGui::Text("QE      Rotate Z");
    
    ImGui::SetCursorPos(ImVec2(5, 520));
    ImGui::Text("H       Ghost");
    
    ImGui::SetCursorPos(ImVec2(5, 530));
    ImGui::Text("M       Sound");
    
    ImGui::SetCursorPos(ImVec2(5, 540));
    ImGui::Text("P       Pause");
    
    ImGui::SetCursorPos(ImVec2(5, 550));
    ImGui::Text("R       Reset");
    
    if (smallFont) {
        ImGui::PopFont();
    }
}

// ═══════════════════════════════════════════════════════════════
// ENHANCED STACK INDICATOR - Much better than the basic version!
// ═══════════════════════════════════════════════════════════════
void State::drawEnhancedStackIndicator(int stackPosition, bool gameIsOver)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | 
                                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | 
                                   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    
    // ═══════════════════════════════════════════════════════════════
    // PERFECTLY BALANCED LEFT PANEL WITH PROPER SPACING
    // Position: 12px from edge | Size: 38x568px | More space from grid
    // ═══════════════════════════════════════════════════════════════
    
    ImGui::SetNextWindowPos(ImVec2(12, 16), ImGuiCond_Always);        // Closer to edge
    ImGui::SetNextWindowSize(ImVec2(38, 568), ImGuiCond_Always);      // Much narrower for spacing
    ImGui::Begin("enhanced stack", NULL, window_flags);

    // Pure visual stack - NO TEXT CLUTTER!
    // Title removed for cleaner look
    float borderRadius = 6.0f;
    float spacing = 2.0f;
    float posY = 20.0f;         // Start a bit lower since no title
    float barWidth = 28.0f;     // Narrower bars for 38px panel
    float barHeight = 22.0f;
    int maxLevel = stackPosition; // Maximum level with blocks
    
    // Get current time for pulsing effects
    float time = ImGui::GetTime();
    
    // Draw 10 levels: from level 9 (top) to level 0 (bottom)
    for (int level = 9; level >= 0; level--) {
        //ImVec2 p = ImGui::GetCursorScreenPos();
        float currentY = posY + (9 - level) * (barHeight + spacing);  // Level 9 at top, level 0 at bottom
        
        // Determine colors and effects based on level
        ImVec4 baseColor;
        bool isDangerous = (level >= 8); // Levels 8-9 are dangerous
        bool isCritical = (level >= 9);  // Level 9 is critical
        bool isFilled = (maxLevel >= 0 && level <= maxLevel); // Fill if stackPosition >= 0 (blocks exist) AND level is occupied
        
        if (isFilled) {
            // Get depth color
            Color color = Color::getDeepthColor(level);  // Use level directly, not level-1
            baseColor = ImVec4(color.getR(), color.getG(), color.getB(), 1.0f);
            
            // Add pulsing effect for dangerous levels
            if (isDangerous && !gameIsOver) {
                float pulse = 0.7f + 0.3f * sin(time * 4.0f + level);
                baseColor.x *= pulse;
                baseColor.y *= pulse;
                baseColor.z *= pulse;
            }
            
            // Critical level gets red tint and faster pulse
            if (isCritical && !gameIsOver) {
                float fastPulse = 0.5f + 0.5f * sin(time * 8.0f);
                baseColor = ImVec4(1.0f * fastPulse, 0.2f, 0.2f, 1.0f);
            }
            
        } else {
            // Empty levels - subtle cyan outline
            baseColor = ImVec4(0.0f, 0.4f, 0.4f, 1.0f);
        }
        
        // Draw the bar segment - USING IMGUI NATIVE CONTENT AREA
        ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        ImVec2 contentMax = ImGui::GetWindowContentRegionMax();
        ImVec2 windowPos = ImGui::GetWindowPos();
        
        float contentWidth = contentMax.x - contentMin.x;
        float leftMargin = (contentWidth - barWidth) / 2.0f;
        float barStartX = windowPos.x + contentMin.x + leftMargin;
        
        if (isFilled) {
            // Filled rectangle with gradient effect
            ImGui::GetWindowDrawList()->AddRectFilled(
                ImVec2(barStartX, currentY),
                ImVec2(barStartX + barWidth, currentY + barHeight),
                ImGui::GetColorU32(baseColor),
                borderRadius);
                
            // Add a lighter border for depth
            ImVec4 borderColor = ImVec4(baseColor.x * 1.3f, baseColor.y * 1.3f, baseColor.z * 1.3f, 1.0f);
            ImGui::GetWindowDrawList()->AddRect(
                ImVec2(barStartX, currentY),
                ImVec2(barStartX + barWidth, currentY + barHeight),
                ImGui::GetColorU32(borderColor),
                borderRadius, 0, 2.0f);
        } else {
            // Empty rectangle
            ImGui::GetWindowDrawList()->AddRect(
                ImVec2(barStartX, currentY),
                ImVec2(barStartX + barWidth, currentY + barHeight),
                ImGui::GetColorU32(baseColor),
                borderRadius, 0, 1.5f);
        }
    }
    
    // All text indicators removed - pure visual stack indicator now!
    // The visual effects (colors, pulsing) provide all needed feedback
    
    ImGui::End();
}
