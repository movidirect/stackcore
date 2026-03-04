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

#define SDL_MAIN_HANDLED
#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Game game;

    bool startDemo = false;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--demo" || arg == "-d") {
            startDemo = true;
        }
    }

    if (game.init()) {
        if (startDemo) {
            game.setDemoMode(true);
        }
        game.run();
    }

    return 0;
}
