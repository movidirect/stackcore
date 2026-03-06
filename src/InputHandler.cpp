#include "InputHandler.h"
#include <cstring>

InputHandler::InputHandler() {
    memset(gamepadRepeatTimers, 0, sizeof(gamepadRepeatTimers));
    lastGamepadRepeatTime = GetTime();
}

InputHandler::~InputHandler() {}

bool InputHandler::isGamepadButtonRepeat(int button) {
    if (!IsGamepadAvailable(0)) return false;
    
    // Manual repeat logic (DAS)
    if (IsGamepadButtonPressed(0, button)) {
        gamepadRepeatTimers[button] = 0.3f; // Initial delay
        return true;
    }
    
    if (IsGamepadButtonDown(0, button)) {
        float dt = GetFrameTime();
        gamepadRepeatTimers[button] -= dt;
        if (gamepadRepeatTimers[button] <= 0) {
            gamepadRepeatTimers[button] = 0.05f; // Repeat interval
            return true;
        }
    }
    return false;
}

GameCommands InputHandler::getPlayerCommands() {
    GameCommands cmds;
    int g = 0; // Primary gamepad
    bool padConnected = IsGamepadAvailable(g);

    // Movement (Arrows / D-Pad / Stick)
    bool gUp    = isGamepadButtonRepeat(GAMEPAD_BUTTON_LEFT_FACE_UP) || (padConnected && GetGamepadAxisMovement(g, GAMEPAD_AXIS_LEFT_Y) < -0.5f);
    bool gDown  = isGamepadButtonRepeat(GAMEPAD_BUTTON_LEFT_FACE_DOWN) || (padConnected && GetGamepadAxisMovement(g, GAMEPAD_AXIS_LEFT_Y) > 0.5f);
    bool gLeft  = isGamepadButtonRepeat(GAMEPAD_BUTTON_LEFT_FACE_LEFT) || (padConnected && GetGamepadAxisMovement(g, GAMEPAD_AXIS_LEFT_X) < -0.5f);
    bool gRight = isGamepadButtonRepeat(GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || (padConnected && GetGamepadAxisMovement(g, GAMEPAD_AXIS_LEFT_X) > 0.5f);

    cmds.moveUp    = IsKeyPressed(KEY_UP)    || IsKeyPressedRepeat(KEY_UP)    || gUp;
    cmds.moveDown  = IsKeyPressed(KEY_DOWN)  || IsKeyPressedRepeat(KEY_DOWN)  || gDown;
    cmds.moveLeft  = IsKeyPressed(KEY_LEFT)  || IsKeyPressedRepeat(KEY_LEFT)  || gLeft;
    cmds.moveRight = IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT) || gRight;

    // Rotation
    // X (W/S) -> LT/RT (Wait, RT is usually axis 5, but mapped as button 11 in many profiles)
    bool gRotX    = padConnected && (IsGamepadButtonPressed(g, GAMEPAD_BUTTON_LEFT_TRIGGER_2) || IsGamepadButtonPressed(g, GAMEPAD_BUTTON_RIGHT_FACE_UP)); // Y
    bool gRotXRev = padConnected && (IsGamepadButtonPressed(g, GAMEPAD_BUTTON_RIGHT_TRIGGER_2) || IsGamepadButtonPressed(g, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)); // A
    cmds.rotateX    = IsKeyPressed(KEY_W) || gRotX;
    cmds.rotateXRev = IsKeyPressed(KEY_S) || gRotXRev;

    // Y (A/D) -> X/B
    bool gRotY    = padConnected && IsGamepadButtonPressed(g, GAMEPAD_BUTTON_RIGHT_FACE_LEFT); // X
    bool gRotYRev = padConnected && IsGamepadButtonPressed(g, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT); // B
    cmds.rotateY    = IsKeyPressed(KEY_A) || gRotY;
    cmds.rotateYRev = IsKeyPressed(KEY_D) || gRotYRev;

    // Z (Q/E) -> LB/RB
    bool gRotZ    = padConnected && IsGamepadButtonPressed(g, GAMEPAD_BUTTON_LEFT_TRIGGER_1); // LB
    bool gRotZRev = padConnected && IsGamepadButtonPressed(g, GAMEPAD_BUTTON_RIGHT_TRIGGER_1); // RB
    cmds.rotateZ    = IsKeyPressed(KEY_Q) || gRotZ;
    cmds.rotateZRev = IsKeyPressed(KEY_E) || gRotZRev;

    // Drop (Space / Button A) - Using Button A as secondary drop if not used for RotateXRev
    // Alternatively use RT for hard drop. Let's stick to A for drop if A is not RotateXRev.
    // Let's use A for RotateXRev and RT for drop to be distinct.
    cmds.drop = IsKeyPressed(KEY_SPACE) || (padConnected && IsGamepadButtonPressed(g, GAMEPAD_BUTTON_RIGHT_TRIGGER_2));

    // Modifiers
    cmds.ctrlPressed = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL) || (padConnected && IsGamepadButtonDown(g, GAMEPAD_BUTTON_LEFT_THUMB));

    // System keys
    cmds.togglePause = IsKeyPressed(KEY_P) || (padConnected && IsGamepadButtonPressed(g, GAMEPAD_BUTTON_MIDDLE_RIGHT)); // Start
    cmds.resetGame   = IsKeyPressed(KEY_R) || IsKeyPressed(KEY_ENTER) || (padConnected && IsGamepadButtonPressed(g, GAMEPAD_BUTTON_MIDDLE_LEFT)); // Back/View
    cmds.toggleSound = IsKeyPressed(KEY_M) || (padConnected && IsGamepadButtonPressed(g, GAMEPAD_BUTTON_RIGHT_THUMB));
    cmds.toggleGhost = IsKeyPressed(KEY_H);
    cmds.toggleNext  = IsKeyPressed(KEY_N);
    cmds.toggleDemo  = IsKeyPressed(KEY_TAB);
    cmds.toggleHelp  = IsKeyPressed(KEY_F1);
    cmds.exitGame    = IsKeyPressed(KEY_ESCAPE);

    return cmds;
}
