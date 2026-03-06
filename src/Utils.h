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

#ifndef UTILS_H
#define UTILS_H

#include <cmath>

class Utils {
    public:
        // Static inline function for rounding a value
        static float round(float value);
        static int mapDepth(int tmp);
};

#endif // UTILS_H
