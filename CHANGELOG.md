# Changelog

All notable changes to Stackcore 3D will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [2.0.0] - 2026-03-04

### Changed
- **Major Migration**: Switched from SDL2/OpenGL to **Raylib 5.0**.
- **Refactored Architecture**: Decoupled the monolithic `Game` class into specialized systems:
  - `AudioManager`: Handles all Raylib audio streams and sound logic.
  - `Renderer`: Manages 3D camera, grid rendering, and scene drawing.
  - `InputHandler`: Abstrae keyboard events and provides a unified command system.
  - `Board`: Encapsulates 3D grid logic, collision detection, and layer clearing.
- **Improved Game Mechanics**: Implemented **Classic 3D Tetris** layer clearing. Completing a layer now only removes those 81 cubes and drops everything above one level, instead of wiping the entire board.

### Added
- **Bot AI (Demo Mode)**: Advanced AI using predictive evaluation (holes, height, bumpiness) to play autonomously.
- **Ghost Block**: Real-time visualization of block landing position.
- **Next Block Preview**: Queue system showing the next 3 incoming blocks.
- **Progressive Difficulty**: Dynamic speed scaling and level progression system.
- **rlImGui Integration**: Modernized UI backend for smoother ImGui integration with Raylib.

### Technical Improvements
- Added **Include Guards** to all header files to prevent multiple inclusion errors.
- Improved memory safety by using modern C++ patterns and explicit cleanup in decoupled destructors.
- Centralized game constants in `Game.h` to eliminate magic numbers.

## [1.0.0] - 2024-12-XX

### Added
- 3D block puzzle gameplay inspired by original Blockout (1989)
- 9 different block types (single cube, I-blocks, L-blocks, T-block, cross, 2x2 cube)
- Full 3D rotation system (X, Y, Z axes)
- Dynamic camera control with CTRL+WASD
- Score tracking system (blocks placed, cubes placed, high score)
- Visual level indicator showing depth progress
- Sound effects with toggle capability
- Retro LCD-style UI using Dear ImGui
- Save/load game state functionality
- Cross-platform support (Linux/Windows)

### Technical Features
- SDL2 + OpenGL graphics pipeline
- Dear ImGui user interface
- SDL2_mixer audio system
- Float-based coordinate system (±4.5f boundaries)
- 9x9x9 playing field grid
- Real-time collision detection
- Memory-safe resource management

### Fixed
- **Critical Bug Fixes** (Pre-1.0):
  - ✅ Fixed rotation bounds checking - blocks now stay within game boundaries
  - ✅ Fixed type inconsistencies (int/float precision issues) throughout codebase
  - ✅ Fixed memory leaks in game reset functionality
  - ✅ Fixed random seed initialization causing repetitive block sequences
  - ✅ Fixed double delete protection for safer memory management
  - ✅ Fixed W/S rotation controls to perform opposite rotations

### Known Issues
- Font loading may show error messages (non-critical)
- Save/load functionality partially commented out
- Limited sound effect variety

## [0.9.0] - 2024-XX-XX (Pre-release)

### Added
- Initial game implementation
- Basic 3D rendering system
- Block generation and rotation
- Collision detection system
- UI framework setup

### Issues Resolved in 1.0.0
- Blocks escaping boundaries during rotation
- Type conversion precision loss
- Memory management problems
- Inconsistent random generation
- Rotation control confusion

---

## Version History Summary

- **v1.0.0**: Full open source release with critical bug fixes
- **v0.9.x**: Initial development and testing phases

## Upgrade Notes

### From Pre-1.0 to 1.0.0
- All critical gameplay bugs have been resolved
- Improved memory management - games should be more stable
- Enhanced rotation system - better control responsiveness
- Type system consistency - better precision in block positioning

## Future Roadmap

### Planned Features
- Enhanced graphics with shader support
- Additional block types and game modes
- Multiplayer/network support
- Automated testing framework
- Performance optimizations
- Mobile platform support

### Community Contributions Welcome
- Bug reports and fixes
- New block types or game modes
- Graphics and UI improvements
- Performance optimizations
- Platform ports
- Documentation improvements

---

**Note**: This project follows semantic versioning. Given the nature of a game project:
- **MAJOR** version changes indicate significant gameplay or architectural changes
- **MINOR** version changes add new features or improvements
- **PATCH** version changes are for bug fixes and small improvements 