#ifndef _DIALOGUENODE_H__
#define _DIALOGUENODE_H__

#include "Dialogue.h"
#include "SDL/include/SDL_rect.h"

class DialogueNode : public Dialogue
{
public:
	//default constructor
	DialogueNode() {};

	DialogueNode(std::string text, int id) : text(text), nodeID(id),
				 optionsActive(false) {};

	virtual ~DialogueNode() {};

public:
	int						nodeID;
	int						letterCounter = 0;
	bool					optionsActive = false;
	bool					nodeEnd = false;

	std::string				text;

	List<DialogueOption*>	optionsList;
};
#endif // __DIALOGUENODE_H__
