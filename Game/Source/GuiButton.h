#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"
#include "Point.h"

class GuiButton : public GuiControl
{
public:

    GuiButton(SDL_Rect bounds, const char* text);
    virtual ~GuiButton();

    bool Update(float dt);
    bool Draw(float scaleX = 1.0f, float scaleY = 1.0f, bool drawTexture = true, bool staticPos = true);
    void SetTexture(const char* path, iPoint magnitude);
    void Delete();
	bool buttonFocus = false;

private:
    void UpdateDimensions(iPoint magnitudes);

private:
    //Texture sections
    iPoint locked = { 0, 0};
    iPoint normal = { 0, 0};
    iPoint focused = { 0, 0};
    iPoint pressed = { 0, 0};
    SDL_Texture* spritesheet = nullptr;
};

#endif // __GUIBUTTON_H__
