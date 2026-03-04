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

#include "GameColor.h"
#include <set>
#include <tuple>

GameColor::GameColor(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
};

GameColor GameColor::getRandomGameColor() {
    return GameColor(static_cast<float>(rand()) / RAND_MAX,
                 static_cast<float>(rand()) / RAND_MAX,
                 static_cast<float>(rand()) / RAND_MAX);
}

std::array<GameColor, 9> GameColor::getRandomGameColors() {
    std::array<GameColor, 9> colors;
    std::set<std::tuple<float, float, float>> uniqueGameColors;

    for (auto& color : colors) {
        GameColor newGameColor;
        do {
            newGameColor = getRandomGameColor();
        } while (uniqueGameColors.find(std::make_tuple(newGameColor.getR(), newGameColor.getG(), newGameColor.getB())) != uniqueGameColors.end());

        uniqueGameColors.insert(std::make_tuple(newGameColor.getR(), newGameColor.getG(), newGameColor.getB()));
        color = newGameColor;
    }

    return colors;
}

GameColor GameColor::getDeepthGameColor(int deepth)
{
    switch (deepth)
    {
    case 0:
        return GameColor(0.0f, 0.0f, 1.0f);
    case 1:
        return GameColor(0.3f, 1.0f, 0.3f);
    case 2:
        return GameColor(1.0f, 0.0f, 0.0f);
    case 3:
        return GameColor(0.3f, 0.3f, 1.0f);
    case 4:
        return GameColor(1.0f, 0.0f, 1.0f);
    case 5:
        return GameColor(0.0f, 0.5f, 0.5f);
    case 6:
        return GameColor(0.5f, 0.5f, 0.5f);
    case 7:
        return GameColor(0.5f, 0.0f, 0.0f);
    case 8:
        return GameColor(0.0f, 0.5f, 0.0f);
    case 9:
        return GameColor(0.0f, 0.0f, 0.5f);    
    default:
        return GameColor(0.0f, 0.0f, 0.0f);
    }
}

float GameColor::getR() { return r; }
float GameColor::getG() { return g; }
float GameColor::getB() { return b; }
