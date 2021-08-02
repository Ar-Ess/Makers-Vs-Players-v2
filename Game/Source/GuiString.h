#ifndef _GUISTRING_H_
#define _GUISTRING_H_

#include "GuiControl.h"
#include "App.h"
#include "FontTTF.h"

class SString;
struct _TTF_Font;
struct SDL_Texture;
class DialogueManager;

class GuiString : public GuiControl
{
public:
	GuiString();

	virtual ~GuiString();

public:

	void Draw(float scaleX = 1.0f, float scaleY = 1.0f, bool staticPos = true);
	void SetString(const char* newText, SDL_Color color = { 255,255,255,255 }, int endline = 850);
	void SetTextFont(_TTF_Font* textFont);
	void Delete();
	void CenterAlign();

	void CenterDialogue();
	void NodePlacing();	

private:
	friend class DialogueManager;
	_TTF_Font* textFont = nullptr;
	SDL_Texture* textTexture = nullptr;
};

#endif // !_GUISTRING_H_
