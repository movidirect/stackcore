#include "InputHandler.h"

InputHandler::InputHandler() {}
InputHandler::~InputHandler() {}

GameCommands InputHandler::getPlayerCommands() {
    GameCommands cmds;

    // Movement
    cmds.moveUp    = IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_UP);
    cmds.moveDown  = IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_DOWN);
    cmds.moveLeft  = IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT);
    cmds.moveRight = IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT);

    // Rotation
    cmds.rotateX    = IsKeyPressed(KEY_W);
    cmds.rotateXRev = IsKeyPressed(KEY_S);
    cmds.rotateY    = IsKeyPressed(KEY_A);
    cmds.rotateYRev = IsKeyPressed(KEY_D);
    cmds.rotateZ    = IsKeyPressed(KEY_Q);
    cmds.rotateZRev = IsKeyPressed(KEY_E);

    // Drop
    cmds.drop = IsKeyPressed(KEY_SPACE);

    // Modifiers
    cmds.ctrlPressed = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);

    // System keys
    cmds.togglePause = IsKeyPressed(KEY_P);
    cmds.resetGame  = IsKeyPressed(KEY_R) || (IsKeyPressed(KEY_ENTER)); // Simplified for Game handle
    cmds.toggleSound = IsKeyPressed(KEY_M);
    cmds.toggleGhost = IsKeyPressed(KEY_H);
    cmds.toggleNext  = IsKeyPressed(KEY_N);
    cmds.toggleDemo  = IsKeyPressed(KEY_F1);
    cmds.exitGame    = IsKeyPressed(KEY_ESCAPE);

    return cmds;
}
