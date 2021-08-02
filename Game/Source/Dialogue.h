#ifndef _DIALOGUE_H__
#define _DIALOGUE_H__


#include "List.h"
#include "GuiString.h"
#include "SString.h"
#include "Point.h"

#include "App.h"
#include "Window.h"

//#include "SDL/include/SDL.h"

class DialogueNode;
class DialogueOption;

struct SDL_Texture;

class Dialogue
{
public:
	//default constructor
	Dialogue() {};
	Dialogue(int id) : dialogueID(id), currentNode(nullptr) {};
	virtual ~Dialogue() {}

	void AssignOptions();

public: 
	int						dialogueID = NULL;

	DialogueNode*			currentNode;
	List<DialogueNode*>		nodeList;
};

#endif // _DIALOGUE_H__