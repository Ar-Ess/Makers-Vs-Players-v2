#include "App.h"
#include "Scene.h"
#include "GuiButton.h"
#include "GuiManager.h"
#include "Audio.h"
#include "Textures.h"
#include "DialogueManager.h"

GuiButton::GuiButton(SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON)
{
    this->bounds = bounds;
    this->text = text;   
}

GuiButton::~GuiButton()
{
	observer = nullptr;
    text.Clear();
    text.~SString();
}

bool GuiButton::Update(float dt)
{
    if (state != GuiControlState::LOCKED)
    {
        int mouseX, mouseY;
        app->input->GetMousePosition(mouseX, mouseY);

        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            if (state == GuiControlState::NORMAL)
            {
                app->audio->SetFx(Effect::BUTTON_FOCUSSED);
            }
            state = GuiControlState::FOCUSED;
            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP || app->input->GetControl(A) == KeyState::KEY_UP)
            {
                app->audio->SetFx(Effect::BUTTON_RELEASED);
                NotifyObserver();
            }
        }
        else if (buttonFocus)
        {
            if (state == GuiControlState::NORMAL)
            {
                app->audio->SetFx(Effect::BUTTON_FOCUSSED);
            }
            state = GuiControlState::FOCUSED;
            if ((app->input->GetControl(A) == KeyState::KEY_REPEAT) || (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_REPEAT))
            {
                state = GuiControlState::PRESSED;
            }

            if ((app->input->GetControl(A) == KeyState::KEY_UP) || (app->input->GetKey(SDL_SCANCODE_RETURN) == KeyState::KEY_UP))
            {
                app->audio->SetFx(Effect::BUTTON_RELEASED);
                NotifyObserver();
                app->guiManager->idSelection = -1;
            }
        }
        else state = GuiControlState::NORMAL;
    }
    return true;
}

bool GuiButton::Draw(float scaleX, float scaleY, bool drawTexture, bool staticPos)
{
    if (drawTexture)
    {
        SDL_Rect section = {0, 0, bounds.w, bounds.h};
        switch (state)
        {
        case GuiControlState::LOCKED:
            section.x = locked.x;
            section.y = locked.y;
            app->render->DrawTexture(spritesheet, bounds.x, bounds.y, scaleX, scaleY, &section, false, staticPos);
            break;
        case GuiControlState::NORMAL: 
            section.x = normal.x;
            section.y = normal.y;
            app->render->DrawTexture(spritesheet, bounds.x, bounds.y, scaleX, scaleY, &section, false, staticPos);
            break;
        case GuiControlState::FOCUSED:
            section.x = focused.x;
            section.y = focused.y;
            app->render->DrawTexture(spritesheet, bounds.x, bounds.y, scaleX, scaleY, &section, false, staticPos);
            break;
        case GuiControlState::PRESSED:
            section.x = pressed.x;
            section.y = pressed.y;
            app->render->DrawTexture(spritesheet, bounds.x, bounds.y, scaleX, scaleY, &section, false, staticPos);
            break;
        default:
            break;
        }
    }

    if (app->guiManager->debugGui)
    {
        SDL_Rect buttonRect = bounds;

        if (staticPos)
        {
            buttonRect.x += (int)app->render->camera.x;
            buttonRect.y += (int)app->render->camera.y;
        }
        buttonRect.w *= scaleX;
        buttonRect.h *= scaleY;

        switch (state)
        {
        case GuiControlState::LOCKED: app->render->DrawRectangle(buttonRect, { 100, 100, 100, 80 });
            break;
        case GuiControlState::NORMAL: app->render->DrawRectangle(buttonRect, { 0, 255, 0, 80 });
            break;
        case GuiControlState::FOCUSED: app->render->DrawRectangle(buttonRect, { 255, 255, 0, 80 });
            break;
        case GuiControlState::PRESSED: app->render->DrawRectangle(buttonRect, { 0, 255, 255, 80 });
            break;
        default:
            break;
        }
    }

    return false;
}

void GuiButton::SetTexture(const char* path, iPoint magnitude)
{
    app->tex->UnLoad(spritesheet);
    spritesheet = app->tex->Load(path);

    UpdateDimensions(magnitude);
}

void GuiButton::Delete()
{
    observer = nullptr;
    app->tex->UnLoad(spritesheet);
    spritesheet = nullptr;
    text.Clear();
}

void GuiButton::UpdateDimensions(iPoint magnitude)
{
    bounds.w = magnitude.x;
    bounds.h = magnitude.y;

    locked = {0, 0 * bounds.h};
    normal = { 0, 1 * bounds.h };
    focused = { 0, 2 * bounds.h };
    pressed = { 0, 3 * bounds.h };
}
