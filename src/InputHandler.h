#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <raylib.h>

struct GameCommands {
    bool moveUp = false;
    bool moveDown = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool rotateX = false;
    bool rotateXRev = false;
    bool rotateY = false;
    bool rotateYRev = false;
    bool rotateZ = false;
    bool rotateZRev = false;
    bool drop = false;
    bool ctrlPressed = false;
    
    // System commands
    bool togglePause = false;
    bool resetGame = false;
    bool toggleSound = false;
    bool toggleGhost = false;
    bool toggleNext = false;
    bool toggleDemo = false;
    bool toggleHelp = false;
    bool exitGame = false;
};

class InputHandler {
public:
    InputHandler();
    ~InputHandler();

    GameCommands getPlayerCommands();

private:
    bool isGamepadButtonRepeat(int button);
    
    // Gamepad repeat logic state
    float gamepadRepeatTimers[GAMEPAD_BUTTON_LEFT_THUMB + 1]; 
    float lastGamepadRepeatTime = 0.0f;
};

#endif // INPUTHANDLER_H
