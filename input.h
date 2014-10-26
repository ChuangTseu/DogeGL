#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>

#define NUM_KEYS SDL_NUM_SCANCODES
#define NUM_MOUSEBUTTONS SDL_BUTTON_X2 - SDL_BUTTON_LEFT
#define NUM_LOGICAL_BUTTONS NUM_KEYS + NUM_MOUSEBUTTONS
#define MAX_ACTIONS_PER_KEY 5

struct ActionKeys
{
    int numKeys;
    int keys[3];
};

class Input
{
public:
    explicit Input();

    void dispatchEvent(SDL_Event* event);

    bool loadKeysFile(const char* filePath);

    void showCursor(bool enabled) const;
    void captureCursor(bool enabled) const;

    bool getKey(const SDL_Scancode key) const;
    bool getMouseButton(const Uint8 button) const;
    bool mouseMouve() const;

    int getX() const;
    int getY() const;

    int getXRel() const;
    int getYRel() const;

    enum {
        QUIT_EVENT
    };

    int updateEvents();

private:
    void doCleanUp();

    SDL_Event m_sdlEvent;

    bool m_keys[NUM_LOGICAL_BUTTONS];
    bool m_mouseButtons[8];

    int m_x;
    int m_y;
    int m_xRel;
    int m_yRel;
};

#endif // INPUT_H
