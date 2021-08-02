#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "Animation.h"
#include "List.h"

#define MAGENTA {190,0,0,150}
#define RED {255,0,0,255}
#define SOFT_RED {240,80,0,255}
#define BLUE {0,143,255,255}
#define CYAN {0,255,247,255}	

struct SDL_Texture;

class GuiManager : public Module
{
public:

	GuiManager();

	virtual ~GuiManager();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	GuiControl* CreateGuiControl(GuiControlType type);

	void DestroyGuiControl(GuiControl* entity);

	void AddGuiControl(GuiControl* entity);

	void DrawPlayerLifeBar(int life, int maxLife, int x, int y);

	void DrawEnemyLifeBar(int life, int maxLife, int x, int y, int size = 4);

	void BlinkLifeBar(int life,SDL_Color color1 , SDL_Color color2);

	void DisableAllButtons();

	void SelectButtonsLogic();

public:

	List<GuiControl*> controls;
	List<GuiControl*> buttons;

	bool debugGui = false;
	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;

	int idSelection = -1;

private:
	int secondsCounter;
	int frameCounter;

};

#endif // __GUIMANAGER_H__

//TEXT
/*    IMPLEMENTATION
//testText = (GuiString*)app->guiManager->CreateGuiControl(GuiControlType::TEXT);
//testText->bounds = {400, 400, 0, 0};
//testText->SetString("TEST");
//testText->SetTextFont(app->fontTTF->defaultFont);

//    UPDATING
//testText->Draw();

//    DELETING
//testText->Delete();
//app->guiManager->DestroyGuiControl(testText);
//testText = nullptr;*/

//BUTTON
/*    IMPLEMENTATION
//testButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON);
//testButton->bounds = { 200, 200, 0, 0 };
//testButton->SetTexture("Assets/Textures/UI/button_set_1.png", { 163, 62 });
//testButton->SetObserver(this);
//testButton->text = "test";

//    UPDATING
//testButton->Update(0.0f);
//testButton->Draw();

//    DELETING
//testButton->Delete();
//app->guiManager->DestroyGuiControl(testButton);
//testButton = nullptr;*/

//CHECKBOX
/*    IMPLEMENTATION
//testCheckBox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX);
//testCheckBox->bounds = { 400, 400, 0, 0 };
//testCheckBox->SetTexture("Assets/Textures/UI/checkbox_set_1.png", { 60, 64 });
//testCheckBox->SetObserver(this);
//testCheckBox->text = "test";

//    UPDATING
//testCheckBox->Update(0.0f);
//testCheckBox->Draw();

//    DELETING
//testCheckBox->Delete();
//app->guiManager->DestroyGuiControl(testCheckBox);
//testCheckBox = nullptr;*/

//SLIDER
/*    IMPLEMENTATION
//testSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER);
//testSlider->bounds = { 700, 400, 0, 0 };
//testSlider->SetMaxValue(128);
//testSlider->SetMinValue(0);
//testSlider->SetTexture("Assets/Textures/UI/slider_set_1.png", { 487, 22 }, { 60, 64 });
//testSlider->SetObserver(this);
//testSlider->text = "test";

//    UPDATING
//testSlider->Update(0.0f);
//testSlider->Draw();

//    DELETING
//testString->Delete();
//app->guiManager->DestroyGuiControl(testSlider);
//testString = nullptr;*/
