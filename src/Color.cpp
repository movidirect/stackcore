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

#include "Color.h"
#include <set>
#include <tuple>

Color::Color(float r, float g, float b)
{
    this->r = r;
    this->g = g;
    this->b = b;
};

Color Color::getRandomColor() {
    return Color(static_cast<float>(rand()) / RAND_MAX,
                 static_cast<float>(rand()) / RAND_MAX,
                 static_cast<float>(rand()) / RAND_MAX);
}

std::array<Color, 9> Color::getRandomColors() {
    std::array<Color, 9> colors;
    std::set<std::tuple<float, float, float>> uniqueColors;

    for (auto& color : colors) {
        Color newColor;
        do {
            newColor = getRandomColor();
        } while (uniqueColors.find(std::make_tuple(newColor.getR(), newColor.getG(), newColor.getB())) != uniqueColors.end());

        uniqueColors.insert(std::make_tuple(newColor.getR(), newColor.getG(), newColor.getB()));
        color = newColor;
    }

    return colors;
}

Color Color::getDeepthColor(int deepth)
{
    switch (deepth)
    {
    case 0:
        return Color(0.0f, 0.0f, 1.0f);
    case 1:
        return Color(0.3f, 1.0f, 0.3f);
    case 2:
        return Color(1.0f, 0.0f, 0.0f);
    case 3:
        return Color(0.3f, 0.3f, 1.0f);
    case 4:
        return Color(1.0f, 0.0f, 1.0f);
    case 5:
        return Color(0.0f, 0.5f, 0.5f);
    case 6:
        return Color(0.5f, 0.5f, 0.5f);
    case 7:
        return Color(0.5f, 0.0f, 0.0f);
    case 8:
        return Color(0.0f, 0.5f, 0.0f);
    case 9:
        return Color(0.0f, 0.0f, 0.5f);    
    default:
        return Color(0.0f, 0.0f, 0.0f);
    }
}

float Color::getR() { return r; }
float Color::getG() { return g; }
float Color::getB() { return b; }
