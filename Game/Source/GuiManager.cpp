#include "App.h"
#include "Textures.h"

#include "GuiManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"

#include "Log.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "GuiString.h"
#include "FontTTF.h"

GuiControl* GuiManager::CreateGuiControl(GuiControlType type)
{
	GuiControl* control = nullptr;

	switch (type)
	{
	case GuiControlType::BUTTON: control = new GuiButton({ 0, 0, 0, 0 }, "0"); break;
	case GuiControlType::CHECKBOX: control = new GuiCheckBox({ 0, 0, 0, 0 }, "0"); break;
	case GuiControlType::SLIDER: control = new GuiSlider({ 0, 0, 0, 0 }, "0"); break;
	case GuiControlType::TEXT: control = new GuiString(); break;
	default: break;
	}

	if (control != nullptr) controls.Add(control);

	return control;
}

void GuiManager::DestroyGuiControl(GuiControl* entity)
{
	int id = controls.Find(entity);
	controls.Del(controls.At(id));
}

GuiManager::GuiManager()
{
}

GuiManager::~GuiManager()
{
}

bool GuiManager::Awake(pugi::xml_node&)
{
	return true;
}

bool GuiManager::Start()
{
	secondsCounter = 0;
	frameCounter = 0;

	return true;
}

bool GuiManager::Update(float dt)
{
	frameCounter++;
	if (frameCounter % 25 == 0)
		secondsCounter++;

	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	//UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	Scene* s = app->scene;
	if (s->GetCurrScene() != Scenes::LOGO_SCENE) SelectButtonsLogic();

	return true;
}

void GuiManager::BlinkLifeBar(int life, SDL_Color color1, SDL_Color color2)
{
	/*if(secondsCounter % 2 ==0)
		app->render->DrawRectangle(lifeBar, color1);
	else
		app->render->DrawRectangle(lifeBar, color2);*/
}

void GuiManager::DisableAllButtons()
{
	Scene* s = app->scene;

	//switch (s->GetCurrScene())
	//{
	//case MAIN_MENU:
	//	s->newGameButton->buttonFocus = false;
	//	s->continueButton->buttonFocus = false;
	//	s->optionsButton->buttonFocus = false;
	//	s->exitButton->buttonFocus = false;
	//	break;

	//case PAUSE_MENU:
	//	s->backToGameButton->buttonFocus = false;
	//	s->saveGameButton->buttonFocus = false;
	//	s->optionsPauseButton->buttonFocus = false;
	//	s->backToMenuButton->buttonFocus = false;
	//	break;

	//case OPTIONS_MENU:
	//	s->optionsMenu->fullScreenCheckBox->checkBoxFocus = false;
	//	s->optionsMenu->dFullScreenCheckBox->checkBoxFocus = false;
	//	s->optionsMenu->vSyncCheckBox->checkBoxFocus = false;
	//	s->optionsMenu->musicVolumeSlider->sliderFocus = false;
	//	s->optionsMenu->fxVolumeSlider->sliderFocus = false;
	//	s->optionsMenu->returnMenuButton->buttonFocus = false;
	//	break;
	//}

	s = nullptr;
}

void GuiManager::SelectButtonsLogic()
{
	Scene* s = app->scene;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || app->input->GetControl(L3) == KEY_DOWN)
	{
		idSelection++;
		DisableAllButtons();
	}

	/*switch (s->GetCurrScene())
	{
	case MAIN_MENU:
		if (idSelection == 0) s->newGameButton->buttonFocus = true;
		else if (idSelection == 1) s->continueButton->buttonFocus = true;
		else if (idSelection == 2) s->optionsButton->buttonFocus = true;
		else if (idSelection == 3) s->exitButton->buttonFocus = true;
		else if (idSelection == 4) idSelection = -1;
		break;

	case PAUSE_MENU:
		if (idSelection == 0) s->backToGameButton->buttonFocus = true;
		else if (idSelection == 1) s->saveGameButton->buttonFocus = true;
		else if (idSelection == 2) s->optionsPauseButton->buttonFocus = true;
		else if (idSelection == 3) s->backToMenuButton->buttonFocus = true;
		else if (idSelection == 4) idSelection = -1;
		break;

	case OPTIONS_MENU:
		if (idSelection == 0) s->optionsMenu->dFullScreenCheckBox->checkBoxFocus = true;
		else if (idSelection == 1) s->optionsMenu->fullScreenCheckBox->checkBoxFocus = true;
		else if (idSelection == 2) s->optionsMenu->vSyncCheckBox->checkBoxFocus = true;
		else if (idSelection == 3) s->optionsMenu->musicVolumeSlider->sliderFocus = true;
		else if (idSelection == 4) s->optionsMenu->fxVolumeSlider->sliderFocus = true;
		else if (idSelection == 5) s->optionsMenu->returnMenuButton->buttonFocus = true;
		else if (idSelection == 6) idSelection = -1;
		break;
	}*/

	s = nullptr;
}

bool GuiManager::CleanUp()
{
	return true;
}

