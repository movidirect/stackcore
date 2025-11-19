# Contributing to Stackcore 3D

Thank you for your interest in contributing to Stackcore 3D! This document provides guidelines and information for contributors.

## 🚀 How to Contribute

### Reporting Bugs
1. **Check existing issues** first to avoid duplicates
2. **Use the bug report template** when creating new issues
3. **Include system information**: OS, compiler version, SDL2 version
4. **Provide steps to reproduce** the issue
5. **Include error messages** or console output if applicable

### Suggesting Features
1. **Check existing feature requests** to avoid duplicates
2. **Explain the use case** and why the feature would be valuable
3. **Consider implementation complexity** and compatibility
4. **Be open to discussion** and alternative solutions

### Code Contributions

#### Prerequisites
- Knowledge of C++17 or higher
- Familiarity with SDL2 and OpenGL
- Understanding of 3D graphics concepts
- Experience with Dear ImGui (helpful but not required)

#### Development Setup
1. **Fork the repository**
   ```bash
   git clone https://github.com/your-username/stackcore.git
   cd stackcore
   ```

2. **Install dependencies** (Linux example)
   ```bash
   sudo apt-get install build-essential pkg-config libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libgl1-mesa-dev libglu1-mesa-dev
   ```

3. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

4. **Build and test**
   ```bash
   make clean
   make debug
   ./Output/stackcore
   ```

#### Code Style Guidelines

**General Principles:**
- Use **camelCase** for variables and functions
- Use **PascalCase** for classes
- Use **UPPER_CASE** for constants
- Include **meaningful comments** for complex logic
- **Avoid magic numbers** - use named constants

**C++ Specific:**
- Follow **C++17 standards** minimum
- Use **smart pointers** when appropriate
- **Prefer `const`** when possible
- **Initialize variables** at declaration
- Use **RAII principles**

**Example:**
```cpp
// Good
const float SCENE_LIMIT = 4.5f;
class GameManager {
private:
    int currentScore;
    bool isGameActive;
    
public:
    void updateScore(int points);
    bool checkGameState() const;
};

// Avoid
#define LIMIT 4.5
class gamemanager {
    int score;
    bool active;
    void update(int p);
};
```

#### Commit Guidelines
- Use **clear, descriptive commit messages**
- **One logical change per commit**
- **Reference issue numbers** when applicable

```bash
# Good commit messages
git commit -m "Fix rotation bounds checking for blocks near boundaries"
git commit -m "Add T-block shape implementation (#42)"
git commit -m "Refactor Game.cpp to use consistent float types"

# Avoid
git commit -m "fix bug"
git commit -m "changes"
```

## 🏗️ Architecture Overview

### Core Components
- **`Game`**: Main game logic, block management, collision detection
- **`Block`**: Individual block handling, rotation, positioning
- **`Cube`**: Basic cube primitive with position and color
- **`SDLManager`**: SDL2/OpenGL initialization and window management
- **`State`**: UI rendering with Dear ImGui
- **`Data`**: Save/load game state functionality

### Key Systems
- **Coordinate System**: Float-based 3D coordinates (±4.5f boundaries)
- **Rotation System**: 90-degree rotations around X, Y, Z axes
- **Collision Detection**: Boundary and block-to-block collision
- **Rendering Pipeline**: OpenGL with SDL2 context

## 🧪 Testing

### Manual Testing Checklist
- [ ] **Basic Gameplay**: Blocks fall, rotate, and place correctly
- [ ] **Rotation System**: All rotation keys work (WASD, QE)
- [ ] **Boundary Checking**: Blocks stay within game limits
- [ ] **Collision Detection**: Blocks don't overlap
- [ ] **Level Clearing**: Complete rows are removed
- [ ] **UI Elements**: All interface elements display correctly
- [ ] **Sound System**: Audio plays when enabled
- [ ] **Save/Load**: Game state persists between sessions

### Cross-Platform Testing
Please test on both Linux and Windows if possible:
- **Linux**: Primary development platform
- **Windows**: MinGW compilation target

## 🐛 Known Issues

### Current Limitations
- No automated test suite (contributions welcome!)
- Save/load functionality partially commented out
- Limited sound effect variety
- No network/multiplayer support

### Areas for Improvement
- **Performance optimization** for large block counts
- **Enhanced graphics** (shaders, better textures)
- **Additional block types** or game modes
- **Improved UI/UX** design
- **Automated testing** framework

## 📝 Pull Request Process

1. **Update documentation** if you're changing functionality
2. **Test thoroughly** on your system
3. **Follow the code style** guidelines above
4. **Write descriptive PR description** explaining:
   - What changes were made
   - Why they were necessary
   - How to test the changes
5. **Be responsive** to code review feedback
6. **Squash commits** if requested before merge

### PR Template
```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] Tested on Linux
- [ ] Tested on Windows
- [ ] Manual testing performed
- [ ] No new compilation warnings

## Screenshots (if applicable)
[Include screenshots for UI changes]
```

## 🤝 Community Guidelines

- **Be respectful** and constructive in all interactions
- **Help newcomers** get started with the project
- **Share knowledge** and explain complex concepts
- **Give credit** where credit is due
- **Focus on the code**, not the person

## 📞 Getting Help

- **GitHub Issues**: For bugs and feature requests
- **GitHub Discussions**: For general questions and ideas
- **Code Comments**: For specific implementation questions

Thank you for contributing to Stackcore 3D! 🎮 
 