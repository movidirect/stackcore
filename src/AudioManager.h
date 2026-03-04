#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <raylib.h>

enum Sounds {
    PARKED_SOUND,
    KEY_PRESS_SOUND,
    GAME_OVER_SOUND,
    LEVEL_UP_SOUND,
    LINE_CLEAR_SOUND
};

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool init();
    void update();
    void clean();
    void playSound(int sound);
    void toggleSound();
    bool isSoundEnabled() const;
    bool isPlaying(int sound) const;

private:
    bool soundEnabled;
    Music parkedSound;
    Music keyPressSound;
    Music gameOverSound;
    Music levelUpSound;
    Music lineClearSound;
};

#endif // AUDIOMANAGER_H
