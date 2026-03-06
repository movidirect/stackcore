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

#include "State.h"
#include <iostream>
#include "GameColor.h"
#include <cmath>
#include <cstdint>

State::State()
{
    rlImGuiSetup(true);

    ImGuiIO& io = ImGui::GetIO();
    // Use Roboto for a clean, readable, professional interface
    myFont = io.Fonts->AddFontFromFileTTF("fonts/Roboto-Regular.ttf", 16.0f);
    smallFont = io.Fonts->AddFontFromFileTTF("fonts/Roboto-Regular.ttf", 14.0f);
    
    if (myFont == nullptr) {
        myFont = io.Fonts->AddFontFromFileTTF("Output/fonts/Roboto-Regular.ttf", 16.0f);
        smallFont = io.Fonts->AddFontFromFileTTF("Output/fonts/Roboto-Regular.ttf", 14.0f);
    }

    rlImGuiReloadFonts();

    textureLogo = loadTexture("images/blockimage.png");
    textureGameOver = loadTexture("images/gameisover.png");
    texturePaused = loadTexture("images/gamepaused.png");
    textureSoundEnabled = loadTexture("images/sound_enabled.png");
    textureSoundDisabled = loadTexture("images/sound_disabled.png");
}

State::~State()
{
    UnloadTexture(textureLogo);
    UnloadTexture(textureGameOver);
    UnloadTexture(texturePaused);
    UnloadTexture(textureSoundEnabled);
    UnloadTexture(textureSoundDisabled);
    rlImGuiShutdown();
}

void State::draw(int stackPosition, int score, int highScore, int blocksPlaced, int cubesPlaced, bool gameIsOver, bool gameIspaused, bool soundEnabled, int level, int linesCleared, int linesTarget, float currentSpeed, bool showGhost, const std::vector<int>* nextBlocks, bool demoMode, bool showHelpWindow)
{
    if (myFont) ImGui::PushFont(myFont);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.PopupRounding = 8.0f;
    style.ScrollbarRounding = 6.0f;
    style.ChildRounding = 6.0f;

    // Single main bluish color for panels (same fill and border)
    const ImVec4 mainPanelColor = ImVec4(0.05f, 0.08f, 0.14f, 0.94f);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.00f, 1.00f, 0.90f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.40f, 0.40f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = mainPanelColor;
    colors[ImGuiCol_PopupBg]                = mainPanelColor;
    colors[ImGuiCol_Border]                 = mainPanelColor;
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = mainPanelColor;
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.08f, 0.14f, 0.22f, 0.54f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.10f, 0.18f, 0.28f, 0.67f);
    colors[ImGuiCol_TitleBg]                = mainPanelColor;
    colors[ImGuiCol_TitleBgActive]          = mainPanelColor;
    colors[ImGuiCol_TitleBgCollapsed]       = mainPanelColor;
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.00f, 1.00f, 0.80f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.00f, 0.70f, 0.70f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_Button]                 = mainPanelColor;
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.08f, 0.14f, 0.22f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.10f, 0.18f, 0.28f, 1.00f);
    colors[ImGuiCol_Header]                 = mainPanelColor;
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

    // Draw Logo horizontally centered inside the main panel
    ImGui::SetCursorPos(ImVec2((150.0f - 120.0f) * 0.5f, 5.0f));
    showImageInImGui("textureLogo", &textureLogo, ImVec2(0,0), ImVec2(120, 120));

    // Game status overlays
    if (gameIsOver) {
        ImGui::SetCursorPos(ImVec2((150.0f - 135.0f) * 0.5f, 130.0f));
        showImageInImGui("textureGameOver", &textureGameOver, ImVec2(0,0), ImVec2(135, 30));
    } else if (gameIspaused) {
        ImGui::SetCursorPos(ImVec2((150.0f - 135.0f) * 0.5f, 130.0f));
        showImageInImGui("texturePaused", &texturePaused, ImVec2(0,0), ImVec2(135, 30));
    }

    // Draw organized sections
    drawGameStats(level, linesCleared, linesTarget, currentSpeed);
    drawScoreInfo(score, highScore, blocksPlaced, cubesPlaced);
    drawNextBlocksPreview(nextBlocks);
    drawGameStatus(gameIsOver, gameIspaused, soundEnabled, showGhost, demoMode);
    drawControlsHelp();

    ImGui::End();

    if (showHelpWindow) {
        ImVec2 center = ImVec2(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f);
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(450, 420), ImGuiCond_Appearing);
        ImGui::Begin("Ayuda - Controles", NULL, ImGuiWindowFlags_NoCollapse);
        ImGui::Text("StackCoreV2 - Ayuda\n\n");
        ImGui::Text("Controles principales:");
        ImGui::Text("  Flechas   : Mover Bloque");
        ImGui::Text("  W/S       : Rotar en X");
        ImGui::Text("  A/D       : Rotar en Y");
        ImGui::Text("  Q/E       : Rotar en Z");
        ImGui::Text("  ESPACIO   : Caida rapida");
        ImGui::Text("  TAB       : Activar/Desactivar Bot (Modo Demo)");
        ImGui::Text("  F1        : Mostrar esta ayuda");
        ImGui::Text("  P         : Pausar/Reanudar");
        ImGui::Text("  R/ENTER   : Reiniciar Juego");
        ImGui::Text("  H         : Activar/Desactivar Ghost Block");
        ImGui::Text("  N         : Mostrar/Ocultar siguiente bloque");
        ImGui::Text("  M         : Activar/Desactivar Sonido");
        ImGui::Text("  ESC       : Salir del Juego");
        ImGui::End();
    }

    if (myFont) ImGui::PopFont();
}

void State::drawOverlayImages(bool /*gameIsOver*/, bool /*gameIspaused*/)
{
    // Keeping this function signature to satisfy Game.cpp, but moving all drawing back inside ImGui window context.
}

void State::showImageInImGui(const char* /*name*/, Texture2D* texture, ImVec2 /*position*/, ImVec2 size)
{
    // Use rlImGuiImageSize macro to draw texture inside current ImGui panel
    rlImGuiImageSize(texture, (int)size.x, (int)size.y);
}

Texture2D State::loadTexture(const char* imagePath) 
{
    return LoadTexture(imagePath);
}

void State::render()
{
    // Render logic is now handled by Game::render via rlImGuiEnd()
}

// ═══════════════════════════════════════════════════════════════
// MODULAR UI SECTIONS
// ═══════════════════════════════════════════════════════════════

void State::drawGameStats(int level, int linesCleared, int linesTarget, float currentSpeed)
{
    ImGui::SetCursorPos(ImVec2(10, 160)); // Position raised from 175
    ImGui::Text("--- GAME STATUS ---");

    ImGui::SetCursorPos(ImVec2(10, 178));
    ImGui::Text("Level:        %2d", level);

    ImGui::SetCursorPos(ImVec2(10, 191));
    ImGui::Text("Progress:   %d/%d", linesCleared, linesTarget);

    ImGui::SetCursorPos(ImVec2(10, 204));
    ImGui::Text("Speed:     %.3f", -currentSpeed);
}

void State::drawScoreInfo(int score, int highScore, int blocks, int cubes)
{
    ImGui::SetCursorPos(ImVec2(10, 225)); // Position raised from 250
    ImGui::Text("--- STATISTICS ----");

    ImGui::SetCursorPos(ImVec2(10, 243));
    ImGui::Text("Score:    %6d", score);

    ImGui::SetCursorPos(ImVec2(10, 256));
    ImGui::Text("Best:     %6d", highScore);

    ImGui::SetCursorPos(ImVec2(10, 269));
    ImGui::Text("Blocks:     %4d", blocks);

    ImGui::SetCursorPos(ImVec2(10, 282));
    ImGui::Text("Cubes:      %4d", cubes);
}

void State::drawNextBlocksPreview(const std::vector<int>* nextBlocks)
{
    ImGui::SetCursorPos(ImVec2(10, 305)); // Position raised from 340
    ImGui::Text("--- NEXT BLOCKS ---");

    if (nextBlocks && nextBlocks->size() >= 3) {
        const char* blockNames[] = {
            "Single", "I-2", "I-3", "L-3", "L-4", "Square", "T-4", "I-4", "Cross"
        };
        ImGui::SetCursorPos(ImVec2(10, 323));
        ImGui::Text("1: %s", blockNames[(*nextBlocks)[0] % 9]);
        ImGui::SetCursorPos(ImVec2(10, 336));
        ImGui::Text("2: %s", blockNames[(*nextBlocks)[1] % 9]);
        ImGui::SetCursorPos(ImVec2(10, 349));
        ImGui::Text("3: %s", blockNames[(*nextBlocks)[2] % 9]);
    }
}

void State::drawGameStatus(bool gameIsOver, bool paused, bool soundEnabled, bool showGhost, bool demoMode)
{
    ImGui::SetCursorPos(ImVec2(10, 375)); // Position raised from 415
    ImGui::Text("---- SETTINGS  ----");

    ImGui::SetCursorPos(ImVec2(10, 393));
    ImGui::Text("Sound:    %s", soundEnabled ? "ON " : "OFF");

    ImGui::SetCursorPos(ImVec2(10, 406));
    ImGui::Text("Ghost:    %s", showGhost ? "ON " : "OFF");

    ImGui::SetCursorPos(ImVec2(10, 419));
    if (gameIsOver) ImGui::Text("Status: GAME OVER");
    else if (paused) ImGui::Text("Status:   PAUSED");
    else if (demoMode) {
        float blink = 0.5f + 0.5f * sin((float)GetTime() * 6.0f);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, blink), "Status: DEMO MODE");
    }
    else ImGui::Text("Status:  PLAYING");
}

void State::drawControlsHelp()
{
    ImGui::SetCursorPos(ImVec2(10, 445)); // Position raised from 490
    ImGui::Text("---- CONTROLS -----");

    if (smallFont) ImGui::PushFont(smallFont);
    
    float y = 463.0f; 
    const float step = 12.0f; 
    
    ImGui::SetCursorPos(ImVec2(5, y)); ImGui::Text("Arrows: Move"); y += step;
    ImGui::SetCursorPos(ImVec2(5, y)); ImGui::Text("SPACE:  Drop"); y += step;
    ImGui::SetCursorPos(ImVec2(5, y)); ImGui::Text("WASD:   Rotate XY"); y += step;
    ImGui::SetCursorPos(ImVec2(5, y)); ImGui::Text("QE:     Rotate Z"); y += step;
    ImGui::SetCursorPos(ImVec2(5, y)); ImGui::Text("H: Ghost | M: Sound"); y += step;
    ImGui::SetCursorPos(ImVec2(5, y)); ImGui::Text("P: Pause | R: Reset"); y += step;
    ImGui::SetCursorPos(ImVec2(5, y)); ImGui::Text("TAB: Demo | F1: Help"); 

    if (smallFont) ImGui::PopFont();
}

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
    float time = (float)GetTime(); // Use Raylib's GetTime()
    
    // Draw 10 levels: from level 9 (top) to level 0 (bottom)
    for (int level = 9; level >= 0; level--) {
        float currentY = posY + (9 - level) * (barHeight + spacing);  // Level 9 at top, level 0 at bottom
        
        // Determine colors and effects based on level
        ImVec4 baseColor;
        bool isDangerous = (level >= 8); // Levels 8-9 are dangerous
        bool isCritical = (level >= 9);  // Level 9 is critical
        bool isFilled = (maxLevel >= 0 && level <= maxLevel); // Fill if stackPosition >= 0 (blocks exist) AND level is occupied
        
        if (isFilled) {
            // Get depth color
            GameColor color = GameColor::getDeepthGameColor(level);  // Use level directly, not level-1
            baseColor = ImVec4(color.getR(), color.getG(), color.getB(), 1.0f);
            
            // Add pulsing effect for dangerous levels
            if (isDangerous && !gameIsOver) {
                float pulse = 0.7f + 0.3f * sin(time * 4.0f + level);
                baseColor.x *= pulse;
                baseColor.y *= pulse;
                baseColor.z *= pulse;
            }
            
            // Critical level gets cyan tint and faster pulse (matches main theme)
            if (isCritical && !gameIsOver) {
                float fastPulse = 0.5f + 0.5f * sin(time * 8.0f);
                baseColor = ImVec4(0.2f, 0.7f * fastPulse, 1.0f, 1.0f);
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
