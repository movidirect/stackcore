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
    bool exitGame = false;
};

class InputHandler {
public:
    InputHandler();
    ~InputHandler();

    GameCommands getPlayerCommands();
};

#endif // INPUTHANDLER_H
