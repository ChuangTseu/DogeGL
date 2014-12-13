#include "input.h"

/**
 * Constructor
 */
Input::Input() : m_x(0), m_y(0), m_xRel(0), m_yRel(0)
{
    for (int i = 0; i < NUM_LOGICAL_BUTTONS; ++i)
    {
        m_keys[i] = false;
    }

    for (int i = 0; i < 8; ++i)
    {
        m_mouseButtons[i] = false;
    }
}

int Input::updateEvents() {
    SDL_Event event;

    bool mouseMoved = false;

    static bool isFirstSdlAbsurdMouseMove = true;

    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
            {
                if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    return QUIT_EVENT;

                m_keys[event.key.keysym.scancode] = true;
            }
            break;

            case SDL_KEYUP:
            {
                m_keys[event.key.keysym.scancode] = false;
            }
            break;

            case SDL_MOUSEBUTTONDOWN:
            {
                m_mouseButtons[event.button.button] = true;
            }
            break;

            case SDL_MOUSEBUTTONUP:
            {
                m_mouseButtons[event.button.button] = false;
            }
            break;

            case SDL_MOUSEMOTION:
            {
                if (!isFirstSdlAbsurdMouseMove) {
                    m_x = event.motion.x;
                    m_y = event.motion.y;

                    m_xRel = event.motion.xrel;
                    m_yRel = event.motion.yrel;

                    mouseMoved = true;
                }
                else {
                    isFirstSdlAbsurdMouseMove = false;
                }
            }
            break;

            case SDL_WINDOWEVENT:
            {
                if(event.window.event == SDL_WINDOWEVENT_CLOSE)
                    return QUIT_EVENT;
            }
            break;

            default:
                // nothing in here
                break;
        }

    }

    if (!mouseMoved) {
        m_xRel = 0;
        m_yRel = 0;
    }

    return 1;
}

void Input::dispatchEvent(SDL_Event * event)
{
    switch (event->type)
    {
        case SDL_KEYDOWN:
        {
            m_keys[event->key.keysym.scancode] = true;
        }
        break;

        case SDL_KEYUP:
        {
            m_keys[event->key.keysym.scancode] = false;
        }
        break;

        case SDL_MOUSEBUTTONDOWN:
        {
            m_mouseButtons[event->button.button] = true;
        }
        break;

        case SDL_MOUSEBUTTONUP:
        {
            m_mouseButtons[event->button.button] = false;
        }
        break;

        case SDL_MOUSEMOTION:
        {
            m_x = event->motion.x;
            m_y = event->motion.y;

            m_xRel = event->motion.xrel;
            m_yRel = event->motion.yrel;
        }
        break;

        case SDL_WINDOWEVENT:
            //sendType(PAINPUT_WINDOWEVENT);
        break;

        default:
            // nothing in here
            break;
    }
}

void Input::showCursor(bool enabled) const
{
    if (enabled)
        SDL_ShowCursor (SDL_ENABLE);

    else
        SDL_ShowCursor (SDL_DISABLE);
}

void Input::captureCursor(bool enabled) const
{
    if (enabled)
        SDL_SetRelativeMouseMode(SDL_TRUE);

    else
        SDL_SetRelativeMouseMode(SDL_FALSE);
}

// Getters

/**
 * TODO : comment
 */
bool Input::getKey(const SDL_Scancode key) const
{
    return m_keys[key];
}

/**
 * TODO : comment
 */
bool Input::getMouseButton(const Uint8 button) const
{
    return m_mouseButtons[button];
}

/**
 * TODO : comment
 */
bool Input::mouseMouve() const
{
    return !(m_xRel == 0 && m_yRel == 0);
}

// Getters concernant la position du curseur

/**
 * TODO : comment
 */
int Input::getX() const
{
    return m_x;
}

/**
 * TODO : comment
 */
int Input::getY() const
{
    return m_y;
}

/**
 * TODO : comment
 */
int Input::getXRel() const
{
    return m_xRel;
}

/**
 * TODO : comment
 */
int Input::getYRel() const
{
    return m_yRel;
}


