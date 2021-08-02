#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"

enum SliderType {
    MUSIC,
    FX,
};

class GuiSlider : public GuiControl
{
public:

    GuiSlider(SDL_Rect bounds, const char *text);
    virtual ~GuiSlider();

    bool Update(float dt);
    bool Draw(float scaleX = 1.0f, float scaleY = 1.0f, bool drawTexture = true, bool staticPos = true);
    void SetTexture(const char* path, iPoint barMagnitude, iPoint sMagnitude);
    void Delete();

    void SetMinValue(int val);
    void SetMaxValue(int val);
    void SetValue(int val);
    void UpdateValue();
    void SetSliderValue();

    int GetMinValue() const
    {
        return minValue;
    }

    int GetMaxValue() const
    {
        return maxValue;
    }

    int GetValue() const
    {
        return value;
    }

    float GetPercentValue() const
    {
        return percentValue * 100;
    }

    void SetSlider(SDL_Rect bounds);

    bool sliderFocus = false;

private:
    void UpdateDimensions(iPoint barMagnitudes, iPoint sMagnitudes);

private:

    SDL_Rect slider;
    SDL_Texture* spritesheet = nullptr;
    int value;
    float percentValue;

    int minValue;
    int maxValue;

    bool sliderClicked = false;

    iPoint locked = { 0, 0};
    iPoint normal = { 0, 0 };
    iPoint focused = { 0, 0 };
    iPoint pressed = { 0, 0 };

    iPoint sLocked = { 0, 0 };
    iPoint sNormal = { 0, 0 };
    iPoint sFocused = { 0, 0 };
    iPoint sPressed = { 0, 0 };

    iPoint sLowLocked = { 0, 0 };
    iPoint sLowNormal = { 0, 0 };
    iPoint sLowFocused = { 0, 0 };
    iPoint sLowPressed = { 0, 0 };

    iPoint sMuteLocked = { 0, 0 };
    iPoint sMuteNormal = { 0, 0 };
    iPoint sMuteFocused = { 0, 0 };
    iPoint sMutePressed = { 0, 0 };
};

#endif // __GUISLIDER_H__
