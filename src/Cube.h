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

#ifndef CUBE_H
#define CUBE_H

class Cube {
    public:
        float x, y, z;  // Position cube in 3d space
        float size;     // cube size
        Cube(float xPos, float yPos, float zPos, float cubeSize);
        void render(bool wireframe) const;
        void move(float dx, float dy, float dz);
};
#endif // CUBE_H


