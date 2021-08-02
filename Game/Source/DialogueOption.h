#ifndef __DIALOGUEOPTION_H__
#define __DIALOGUEOPTION_H__

#include "DialogueNode.h"
#include "GuiButton.h"

class DialogueManager;

enum OptionsPos
{
	NO_POS = -1,
	POS1,
	POS2,
	POS3,
	POS4
};

class DialogueOption : public DialogueNode
{
public:
	//default constructor
	DialogueOption() {};

	DialogueOption(int nextNode, int returnCode, SString text) : nextNodeID(nextNode),
				   text(text), returnCode(returnCode), optionButton(nullptr) {};

	virtual ~DialogueOption() {};

	//Utils
	void OptionPlacingX(SDL_Rect nChart, float scale)
	{
		uint w, h;
		int offset = 35;
		app->win->GetWindowSize(w, h);

		if (this->optPlacing == POS1 || this->optPlacing == POS3)
			this->optionButton->bounds.x = nChart.x;
		else
			this->optionButton->bounds.x = nChart.x + nChart.w - (this->optionButton->bounds.w * scale) - offset + 7;

		this->optionText->bounds.x = this->optionButton->bounds.x + offset;
	}

	void OptionPlacingY()
	{
		uint w, h;
		app->win->GetWindowSize(w, h);

		if (this->optPlacing == POS1 || this->optPlacing == POS2)
			this->optionButton->bounds.y = 5 * (h / 6) - this->optionButton->bounds.h;
		else
			this->optionButton->bounds.y = 5 * (h / 6) + (this->optionButton->bounds.h / 2);
		
		this->optionText->bounds.y = this->optionButton->bounds.y;
	}

public: 

	int				nextNodeID;
	int				returnCode;

	SString	text;
	GuiString* optionText = nullptr;
	GuiButton* optionButton;

	OptionsPos optPlacing;
	DialogueNode* nextNode = nullptr;
};

#endif // __DIALOGUEOPTION_H__
