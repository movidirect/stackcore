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

#ifndef COLOR_H
#define COLOR_H

#include <cstdlib>
#include <array>
class GameColor {
    public:
        GameColor() : r(0.0f), g(0.0f), b(1.0f) {} 
        GameColor(float r, float g, float b);
        GameColor static getRandomGameColor();
        GameColor static getDeepthGameColor(int depth);
        static std::array<GameColor, 9> getRandomGameColors(); 
        float getR();
        float getG();
        float getB();

    private:
        float r, g, b;

        

};
#endif // COLOR_H
