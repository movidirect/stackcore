#include "AudioManager.h"
#include <iostream>

AudioManager::AudioManager() : soundEnabled(true) {
}

AudioManager::~AudioManager() {
    clean();
}

bool AudioManager::init() {
    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
    }
    if (!IsAudioDeviceReady()) {
        std::cerr << "Error initializing audio device" << std::endl;
        return false;
    }
    
    // Load sounds using Raylib
    parkedSound   = LoadMusicStream("sounds/parked.mp3");
    keyPressSound = LoadMusicStream("sounds/select.mp3");
    gameOverSound = LoadMusicStream("sounds/gameover.mp3");
    levelUpSound  = LoadMusicStream("sounds/select.mp3");
    lineClearSound = LoadMusicStream("sounds/parked.mp3");

    // Start playing streams
    parkedSound.looping = false;
    keyPressSound.looping = false;
    gameOverSound.looping = false;
    levelUpSound.looping = false;
    lineClearSound.looping = false;

    return true;
}

void AudioManager::update() {
    if (IsAudioDeviceReady()) {
        UpdateMusicStream(parkedSound);
        UpdateMusicStream(keyPressSound);
        UpdateMusicStream(gameOverSound);
        UpdateMusicStream(levelUpSound);
        UpdateMusicStream(lineClearSound);
    }
}

void AudioManager::clean() {
    if (IsAudioDeviceReady()) {
        // Unload sounds
        UnloadMusicStream(parkedSound);
        UnloadMusicStream(keyPressSound);
        UnloadMusicStream(gameOverSound);
        UnloadMusicStream(levelUpSound);
        UnloadMusicStream(lineClearSound);

        CloseAudioDevice();
    }
}

void AudioManager::playSound(int sound) {
    if (!IsAudioDeviceReady()) return;

    // Stop any sound that is currently playing
    StopMusicStream(parkedSound);
    StopMusicStream(keyPressSound);
    StopMusicStream(gameOverSound);
    StopMusicStream(levelUpSound);
    StopMusicStream(lineClearSound);

    if (soundEnabled) {
        switch (sound) {
            case PARKED_SOUND:
                PlayMusicStream(parkedSound);
                break;
            case KEY_PRESS_SOUND:
                PlayMusicStream(keyPressSound);
                break;
            case GAME_OVER_SOUND:
                PlayMusicStream(gameOverSound);
                break;
            case LEVEL_UP_SOUND:
                PlayMusicStream(levelUpSound);
                break;
            case LINE_CLEAR_SOUND:
                PlayMusicStream(lineClearSound);
                break;
            default:
                break;
        }
    }
}

void AudioManager::toggleSound() {
    soundEnabled = !soundEnabled;
}

bool AudioManager::isSoundEnabled() const {
    return soundEnabled;
}

bool AudioManager::isPlaying(int sound) const {
    if (!IsAudioDeviceReady()) return false;
    
    switch (sound) {
        case PARKED_SOUND:
            return IsMusicStreamPlaying(parkedSound);
        case KEY_PRESS_SOUND:
            return IsMusicStreamPlaying(keyPressSound);
        case GAME_OVER_SOUND:
            return IsMusicStreamPlaying(gameOverSound);
        case LEVEL_UP_SOUND:
            return IsMusicStreamPlaying(levelUpSound);
        case LINE_CLEAR_SOUND:
            return IsMusicStreamPlaying(lineClearSound);
        default:
            return false;
    }
}
