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

#include "Cube.h"
#include "SDLManager.h"
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
    Color color = Color::getDeepthColor(mapDepth);
    SDLManager::drawCube(wireframe, x, y, z, size, color.getR(), color.getG(), color.getB());


}

void Cube::move(float dx, float dy, float dz) {
    x += dx;
    y += dy;
    z += dz;
}
