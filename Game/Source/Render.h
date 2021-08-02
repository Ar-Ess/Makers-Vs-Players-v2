#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "Point.h"
#include "Font.h"

#include "SDL/include/SDL.h"
#include "Collider.h"

class Render : public Module
{
public:

	Render();

	virtual ~Render();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

public:
	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Utils
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	iPoint ScreenToWorld(int x, int y) const;

	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, float sX, float sY, SDL_Rect* section = NULL, bool scaleModCoords = true, bool staticPos = true, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE) const;

	bool DrawRectangle(const SDL_Rect& rect, SDL_Color color, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, SDL_Color color, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int radius, SDL_Color color, bool filled = false, bool useCamera = true) const;
	bool DrawCircle(CircleCollider circle, SDL_Color color, bool filled = false, bool useCamera = true) const;

	//bool DrawText(Font font, const char* text, int x, int y, int size, int spacing, SDL_Color tint);
	bool BlitParticle(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, const SDL_Rect* rectSize = NULL, SDL_Color color = { 0, 0, 0, 0 }, SDL_BlendMode blendMode = SDL_BLENDMODE_NONE, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//VSYNC Funcrion
	void SetVSync(bool vSync);

public:

	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;

	bool vSync = true;

	uint scale;
};

#endif // __RENDER_H__