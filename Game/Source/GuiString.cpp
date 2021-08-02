#include "GuiString.h"
#include "Textures.h"
#include "DialogueManager.h"
#include "App.h"

GuiString::GuiString() : GuiControl(GuiControlType::TEXT), textFont(app->fontTTF->defaultFont)
{
	textTexture = app->fontTTF->Print(this->text.GetString(), YELLOW, textFont);	
}

GuiString::~GuiString()
{
	app->tex->UnLoad(textTexture);
}

void GuiString::Draw(float scaleX, float scaleY, bool staticPos)
{
	app->render->DrawTexture(textTexture, bounds.x, bounds.y, scaleX, scaleY, (SDL_Rect*)0, false, staticPos);
}

void GuiString::SetString(const char* newText, SDL_Color color, int endline)
{
	text = SString(newText);
	textTexture = app->fontTTF->Print(newText, color, textFont, textTexture, endline);
}

void GuiString::SetTextFont(_TTF_Font* textFont)
{
	this->textFont = textFont;
}

void GuiString::Delete()
{
	observer = nullptr;
	text.Clear();
	textFont = nullptr;
	app->tex->UnLoad(textTexture);
	textTexture = nullptr;
}

void GuiString::CenterAlign()
{
	int width = 0, height = 0;
	app->fontTTF->CalcSize(text.GetString(), width, height, textFont);

	bounds.x += (bounds.w / 2) - (width / 2);
	bounds.y += (bounds.h / 2) - (height / 2);
}

void GuiString::CenterDialogue()
{
	int width = 0, height = 0, endLine = app->dialogueManager->endLine;
	app->fontTTF->CalcSize(text.GetString(), width, height, textFont);

	int lineScale = static_cast<int>(ceil(width / endLine));
	
	width = endLine;
	height *= lineScale;

	bounds.x += (bounds.w / 2) - (width / 2);
	bounds.y += (bounds.h / 2) - (height / 2);
}

void GuiString::NodePlacing()
{
	uint w, h;
	app->win->GetWindowSize(w, h);
	
	bounds.x = (w / 2) - (bounds.w / 2);
	bounds.y = 5 * (h / 8);	
}
