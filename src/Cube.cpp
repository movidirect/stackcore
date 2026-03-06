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

#include "Cube.h"
#include <raylib.h>
#include "GameColor.h"
#include "Utils.h"

Cube::Cube(float xPos, float yPos, float zPos, float cubeSize)
{
    x = xPos;
    y = yPos;
    z = zPos;
    size = cubeSize;// = 1.0f
}

void Cube::render(bool wireframe) const{

    int mapDepth = 0;
    if(!wireframe){
        mapDepth = Utils::mapDepth(static_cast<int>(Utils::round(z)));
    }
    GameColor color = GameColor::getDeepthGameColor(mapDepth);
    
    // Raylib conversion: Raylib Colors are structs (r, g, b, a) 0-255
    ::Color rayColor = { 
        static_cast<unsigned char>(color.getR() * 255), 
        static_cast<unsigned char>(color.getG() * 255), 
        static_cast<unsigned char>(color.getB() * 255), 
        255 
    };

    Vector3 pos = { x, y, z };

    if (wireframe) {
        DrawCubeWires(pos, size, size, size, WHITE);
    } else {
        DrawCube(pos, size, size, size, rayColor);
        DrawCubeWires(pos, size, size, size, WHITE); // Cambiado de DARKGRAY a WHITE
    }
}

void Cube::move(float dx, float dy, float dz) {
    x += dx;
    y += dy;
    z += dz;
}
