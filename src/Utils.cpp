// Stackcore
// Copyright (C) 2025 Jose R Arenas

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

#include "Utils.h"

float Utils::round(float value){
    return (value < 0.0f) ? std::ceil(value - 0.5f) : std::floor(value + 0.5f);
}

int Utils::mapDepth(int tmp) 
{
    auto mapValue = [](int value, int old_min, int old_max, int new_min, int new_max) {
        return new_min + (value - old_min) * (new_max - new_min) / (old_max - old_min);
    };

    int result = mapValue(tmp, -4, 5, 0, 9);
    return result;

}

