# Stackcore 3D

![Stackcore Game](poster.png){width=700}

A modern C++ recreation of the classic 1989 Blockout game - the 3D version of Tetris! Navigate falling 3D blocks in a cylindrical well, rotating them in all dimensions to create complete horizontal layers.

## 🎮 Game Features

- **3D Block Puzzle**: Experience Tetris in three dimensions
- **9 Block Types**: From simple cubes to complex shapes (I-blocks, L-blocks, T-blocks, cross, 2x2 cube)
- **Full 3D Rotation**: Rotate blocks around X, Y, and Z axes
- **Dynamic Camera**: Control the camera with CTRL+WASD
- **Score System**: Track blocks placed, cubes placed, and high scores
- **Visual Level Indicator**: See your progress through the depth levels
- **Sound Effects**: Immersive audio feedback
- **Retro UI**: Classic LCD-style interface with ImGui

## 🛠️ Technology Stack

- **Language**: C++17
- **Graphics**: SDL2 + OpenGL
- **GUI**: Dear ImGui
- **Audio**: SDL2_mixer
- **Build System**: Makefile (cross-platform)

## 🎯 Game Mechanics

- **Grid**: 9x9x9 playing field (±4.5 unit boundaries)
- **Goal**: Fill complete horizontal layers to clear them
- **Controls**: 
  - `WASD` - Rotate block around X and Y axes
  - `QE` - Rotate block around Z axis
  - `CTRL+WASD` - Move camera
  - `P` - Pause game
  - `M` - Toggle sound
  - `ESC` - Exit game

## 📋 Requirements

### Linux (Recommended)
```bash
sudo apt-get update
sudo apt-get install build-essential pkg-config
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev
```

### Windows
- MinGW with C++17 support
- SDL2, SDL2_image, SDL2_mixer development libraries
- OpenGL and GLU libraries

## 🚀 Installation & Building

### Quick Start (Linux)
```bash
# Clone the repository
git clone https://github.com/movidirect/stackcore.git
cd stackcore

# Install dependencies (Ubuntu/Debian)
sudo apt-get install build-essential pkg-config libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libgl1-mesa-dev libglu1-mesa-dev

# Build the game for release (optimized)
make release

# Or build for debugging
make debug

# Run the game
./Output/stackcore
```

### Windows Build
```bash
# Ensure you have MinGW and required libraries

# Build for release (optimized)
make release

# Or build for debugging
make debug

# Run the game
./Output/stackcore.exe
```

### Clean Build
To remove all build artifacts:
```bash
make clean
```

## 📁 Project Structure

```
stackcore/
├── src/                 # Source code
│   ├── main.cpp        # Entry point
│   ├── Game.cpp/.h     # Main game logic
│   ├── Block.cpp/.h    # Block management
│   ├── Cube.cpp/.h     # Individual cube handling
│   ├── SDLManager.*    # SDL2/OpenGL setup
│   ├── State.*         # UI state management
│   ├── Data.*          # Save/load functionality
│   ├── Color.*         # Color management
│   └── Utils.*         # Utility functions
├── Output/             # Game assets and executable
│   ├── fonts/         # LCD font
│   ├── images/        # UI graphics
│   └── sounds/        # Audio files
├── imgui/             # Dear ImGui library
├── lib/               # Windows libraries
├── include/           # Header files
├── Makefile          # Build configuration
└── README.md         # This file
```

## 🎨 Block Types

1. **Single Cube** - 1x1x1
2. **I-Block 2** - 2 cubes in line
3. **I-Block 3** - 3 cubes in line  
4. **I-Block 4** - 4 cubes in line
5. **L-Block** - L-shaped configuration
6. **2x2 Cube** - 2x2x1 square
7. **T-Block** - T-shaped configuration
8. **Cross** - Plus-shaped configuration

## 🐛 Bug Fixes

This version includes several critical bug fixes:
- ✅ Fixed rotation boundaries (blocks stay within game limits)
- ✅ Fixed type inconsistencies (float/int precision issues)
- ✅ Fixed memory leaks in game reset
- ✅ Fixed random seed initialization
- ✅ Fixed double delete protection
- ✅ Differentiated W/S rotation controls

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request. For major changes, please open an issue first to discuss what you would like to change.

### Development Setup
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the GPL-3.0 - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Original Blockout game by Aleksander Ustaszewski and Mirosław Zabłocki (1989)
- SDL2 Development Libraries
- Dear ImGui for the user interface
- The retro gaming community for inspiration

## 📞 Support

If you encounter any issues or have questions:
- Open an issue on GitHub
- Check the troubleshooting section below

### Troubleshooting

**Font Loading Error**: Ensure `fonts/LCD.ttf` exists in the Output directory
**Sound Issues**: Verify SDL2_mixer is properly installed
**Graphics Problems**: Update your OpenGL drivers

---

**Enjoy playing Stakcore 3D!** 🎮
