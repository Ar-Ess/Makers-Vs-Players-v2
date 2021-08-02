#ifndef __TRANSITION_H__
#define __TRANSITION_H__

#include "Module.h"
#include "SDL\include\SDL_rect.h"

class Transition : public Module
{
public:

    Transition();

    ~Transition();

    bool Start() override;

    bool PreUpdate();
    bool Update(float dt);
    bool PostUpdate();

    bool FadeToBlackEffect(bool fadeIn, float frames = 60); 
    //fadeIn == true, doing the fade in
    //fadeIn == false, doing the fade out

private: // Fade To Black Transition

    enum FadeToBlackStep
    {
        NO_BODY,
        TO_BLACK,
        FROM_BLACK
    }
    currentStep = FadeToBlackStep::NO_BODY;

    uint32 frameCount;
    uint32 maxFadeFrames;

    SDL_Rect screenRect;

public:
    bool isFading = false;
};

#endif //__MODULEFADETOBLACK_H__
