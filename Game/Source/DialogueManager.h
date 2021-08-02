#ifndef __DIALOGUEMANAGER_H__
#define __DIALOGUEMANAGER_H__

#include "App.h"
#include "Module.h"
#include "Input.h"
#include "DialogueOption.h"
#include "FontTTF.h"

#include "SDL/include/SDL_rect.h"
//#include "SDL/include/SDL.h"

#include "Log.h"
#include "List.h"

#define DIALOGUE_CONFIG_FILENAME "dialog_config.xml"

struct SDL_Texture;
struct SDL_Color;
struct SDL_Rect;

class DialogueManager : public Module
{
public: //functions
	// Constructor and destructor //
	DialogueManager();
	virtual ~DialogueManager() {}
	////////////////////////////////
	// Flow Functions
	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool CleanUp();
	/////////////////////////////////	
	void StartDialogue(int dialogueID);

	int GetDialogId() const
	{
		if (currentDialogue == nullptr) return -1;

		return currentDialogue->dialogueID;
	}

private: //functions

	//////CREATORS//////
	DialogueOption* CreateOptions(pugi::xml_node& setter);
	DialogueNode* CreateNode(pugi::xml_node& setter);
	void CreateDialogue(pugi::xml_node& setter);

	void Draw();
	void Input();

	bool OnGuiMouseClickEvent(GuiControl* option);

	pugi::xml_node LoadDialogueConfig(pugi::xml_document&) const;

	void WrittingMachineLook();
	void AccelerateAnim();
	void SkipDialogue();

	void EndDialogue();
	void DeleteOptions(DialogueNode* node);
	void DeleteOptionButtons();


public: //variables
	bool				onDialog = false;
	int					endLine = 850;
	SString bTexPath;
private: // variables
	int					offset = 15;
	int					counter = 0;
	float				dscale = 1.2f;
	std::string			tempText;

	SDL_Rect			textureChart{ 0,0,910,113 };
	GuiString*			tempString;

	Dialogue*			currentDialogue;
	List<Dialogue*>		dialoguesList;

	SDL_Color           black = { 0  , 0  , 0  , 255 };
	SDL_Color           white = { 255, 255, 255, 125 };
	SDL_Color           red =	{ 255, 0  , 0  , 125 };
	SDL_Color           green = { 0  , 255, 0  , 125 };
	SDL_Color           blue =	{ 0  , 0  , 255, 125 };
	SDL_Rect			buttonPrefab{ 0,0,0,0 };

	SDL_Rect			aButton{ 0,0,0,0 };
	SDL_Rect			bButton{ 0,0,0,0 };
	SDL_Texture*		optTexture = nullptr;
	SDL_Texture*		dialogueTexture = nullptr;
};

#endif // __DIALOGUEMANAGER_H__
