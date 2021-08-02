#include "App.h"
#include "GuiCheckBox.h"
#include "GuiManager.h"
#include "Audio.h"
#include "Textures.h"

GuiCheckBox::GuiCheckBox(SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX)
{
    this->bounds = bounds;
    this->text = text;
}

GuiCheckBox::~GuiCheckBox()
{
	observer = nullptr;
	text.Clear();
    delete &text;
}

bool GuiCheckBox::Update(float dt)
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

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || app->input->GetControl(A) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
            }

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP || app->input->GetControl(A) == KeyState::KEY_UP)
            {
                checked = !checked;
                app->audio->SetFx(Effect::BUTTON_RELEASED);
                NotifyObserver();
            }
        }
        else if (checkBoxFocus)
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
                checked = !checked;
                app->audio->SetFx(Effect::BUTTON_RELEASED);
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiCheckBox::Draw(float scaleX, float scaleY, bool drawTexture, bool staticPos)
{
    if (drawTexture)
    {
        SDL_Rect section = { 0, 0, bounds.w, bounds.h };

        switch (state)
        {
        case GuiControlState::LOCKED:
            section.x = locked.x;
            section.y = locked.y;
            app->render->DrawTexture(spritesheet, bounds.x, bounds.y, scaleX, scaleY, &section, false, staticPos);
            break;

        case GuiControlState::NORMAL:
            if (checked)
            {
                section.x = checkNormal.x;
                section.y = checkNormal.y;
            }
            else
            {
                section.x = uncheckNormal.x;
                section.y = uncheckNormal.y;
            }
            app->render->DrawTexture(spritesheet, bounds.x, bounds.y, scaleX, scaleY, &section, false, staticPos);
            break;

        case GuiControlState::FOCUSED:
            if (checked)
            {
                section.x = checkFocused.x;
                section.y = checkFocused.y;
            }
            else
            {
                section.x = uncheckFocused.x;
                section.y = uncheckFocused.y;
            }
            app->render->DrawTexture(spritesheet, bounds.x, bounds.y, scaleX, scaleY, &section, false, staticPos);
            break;

        case GuiControlState::PRESSED:
            if (checked)
            {
                section.x = checkPressed.x;
                section.y = checkPressed.y;
            }
            else
            {
                section.x = uncheckPressed.x;
                section.y = uncheckPressed.y;
            }
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

void GuiCheckBox::SetTexture(const char* path, iPoint magnitude)
{
    app->tex->UnLoad(spritesheet);
    spritesheet = nullptr;
    spritesheet = app->tex->Load(path);

    UpdateDimensions(magnitude);
}

void GuiCheckBox::Delete()
{
    observer = nullptr;
    app->tex->UnLoad(spritesheet);
    spritesheet = nullptr;
    text.Clear();
}

void GuiCheckBox::UpdateDimensions(iPoint magnitude)
{
    bounds.w = magnitude.x;
    bounds.h = magnitude.y;

    locked = { 0, 0 * bounds.h };
    checkNormal = { 0, 1 * bounds.h };
    checkFocused = { 0, 2 * bounds.h };
    checkPressed = { 0, 3 * bounds.h };
    uncheckNormal = { 0, 4 * bounds.h };
    uncheckFocused = { 0, 5 * bounds.h };
    uncheckPressed = { 0, 6 * bounds.h };
}
