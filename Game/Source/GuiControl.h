#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Input.h"
#include "Render.h"
#include "Scene.h"

#include "Point.h"
#include "SString.h"

#include "SDL/include/SDL.h"

enum class GuiControlType
{
    BUTTON,
    TOGGLE,
    CHECKBOX,
    SLIDER,
    SLIDERBAR,
    COMBOBOX,
    DROPDOWNBOX,
    INPUTBOX,
    VALUEBOX,
    SPINNER, 
    TEXT
};

enum class GuiControlState
{
    LOCKED,
    NORMAL,
    FOCUSED,
    PRESSED,
    SELECTED
};

class GuiControl
{
public:

    GuiControl(GuiControlType type) : type(type), state(GuiControlState::NORMAL) {}

    GuiControl(GuiControlType type, SDL_Rect bounds, const char* text) : type(type), state(GuiControlState::NORMAL), bounds(bounds), text(text) {}

    virtual bool Update(float dt)
    {
        return true;
    }

    virtual bool Draw() const
    {
        return true;
    }

    void SetObserver(Module* module)
    {
        observer = module;
    }

    void NotifyObserver()
    {
        observer->OnGuiMouseClickEvent(this);
    }

public:
    GuiControlType type;
    GuiControlState state;
    SString text;
    SDL_Rect bounds;
    Module* observer = nullptr;
};

#endif // __GUICONTROL_H__