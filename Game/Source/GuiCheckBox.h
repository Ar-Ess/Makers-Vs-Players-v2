#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "App.h"
#include "GuiControl.h"

#include "Point.h"

class GuiCheckBox : public GuiControl
{
public:

    GuiCheckBox(SDL_Rect bounds, const char *text);
    virtual ~GuiCheckBox();

    bool Update(float dt);
    bool Draw(float scaleX = 1.0f, float scaleY = 1.0f, bool drawTexture = true, bool staticPos = true);
    void SetTexture(const char* path, iPoint magnitude);
    void Delete();
    
    bool checkBoxFocus = false;

    bool GetCheckedState() const
    {
        return checked;
    }

private:
    void UpdateDimensions(iPoint magnitudes);
    bool checked = false;
    SDL_Texture* spritesheet = nullptr;
    //options menu

    iPoint checkNormal = {0, 0};
    iPoint checkFocused = { 0, 0 };
    iPoint checkPressed = { 0, 0 };
    iPoint uncheckNormal = { 0, 0 };
    iPoint uncheckFocused = { 0, 0 };
    iPoint uncheckPressed = { 0, 0 };
    iPoint locked = {0, 0};
};

#endif // __GUICHECKBOX_H__
