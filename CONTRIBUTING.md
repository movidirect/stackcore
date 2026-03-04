# Contributing to StackCore V2

Thank you for your interest in contributing to StackCore V2! This document provides guidelines and information for contributors after our migration to Raylib and the transition to a decoupled architecture.

## 🚀 How to Contribute

### Reporting Bugs
1. **Check existing issues** first to avoid duplicates.
2. **Use the bug report template** when creating new issues.
3. **Include system information**: OS, compiler version, Raylib version.
4. **Provide steps to reproduce** the issue.

### Suggesting Features
1. **Check existing feature requests**.
2. **Explain the use case** and why the feature would be valuable.

### Code Contributions

#### Prerequisites
- Knowledge of C++17 or higher.
- Familiarity with **Raylib**.
- Understanding of 3D graphics concepts.
- Experience with **Dear ImGui**.

#### Development Setup
1. **Fork the repository**
   ```bash
   git clone https://github.com/your-username/stackcore.git
   cd stackcore
   ```

2. **Install dependencies** (Linux example)
   ```bash
   sudo apt-get install build-essential pkg-config libraylib-dev
   ```

3. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```

4. **Build and test**
   ```bash
   make clean
   make linux
   ./Output/stackcore
   ```

#### Code Style Guidelines

**General Principles:**
- Use **camelCase** for variables and functions.
- Use **PascalCase** for classes.
- Use **UPPER_CASE** for constants.
- **Avoid magic numbers** - use named constants (centralized in `Game.h` or relevant headers).
- **Decoupling**: Keep systems separate. Don't add rendering logic to the `Board` or audio logic to `InputHandler`.

**C++ Specific:**
- Follow **C++17 standards**.
- Use **RAII principles**.
- **Header Guards**: All headers MUST have `#ifndef FILENAME_H` guards.
- **Pointers**: Use raw pointers for system components managed by the `Game` controller, or smart pointers where appropriate.

#### Commit Guidelines
- Use **clear, descriptive commit messages**.
- **One logical change per commit**.

## 🏗️ Architecture Overview

StackCore V2 uses a decoupled architecture to ensure maintainability:

### Core Components
- **`Game`**: The Orchestrator. It manages the lifecycle of all other systems and handles the high-level game state.
- **`Board`**: The Physics & Logic engine. Manages the 3D grid, occupied positions, and the classic layer-clearing logic.
- **`Renderer`**: The View. Encapsulates all Raylib drawing code, camera management, and grid visualization.
- **`AudioManager`**: The Audio system. Manages Raylib music streams, sound loading, and playback.
- **`InputHandler`**: The Input abstraction. Translates hardware keystrokes into `GameCommands` for both players and the Bot AI.
- **`BotAI`**: The "Cerebro". Uses a predictive algorithm to evaluate board states and generate moves.

## 🧪 Testing

### Manual Testing Checklist
- [ ] **Basic Gameplay**: Blocks fall, rotate, and place correctly.
- [ ] **Layer Clearing**: Full 9x9 layers are removed, and blocks above drop correctly.
- [ ] **Bot AI**: Demo mode works and plays intelligently.
- [ ] **UI Elements**: ImGui panels display correct scores, levels, and previews.
- [ ] **Sound System**: Audio plays when enabled and mutes correctly.

## 📝 Pull Request Process

1. **Update documentation** if you're changing functionality.
2. **Test thoroughly** on your system.
3. **Follow the decoupled architecture** - don't introduce cross-system dependencies.
4. **Write descriptive PR description**.

Thank you for contributing to StackCore V2! 🎮
