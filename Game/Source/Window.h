#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class Window : public Module
{
public:

	Window();

	virtual ~Window();

	bool Awake(pugi::xml_node&);

	bool FullScreenLogic();

	bool CleanUp();

	void SetTitle(const char* title);

	void GetWindowSize(uint& width, uint& height) const;

	uint GetScale() const;

	void SetWinFullScreen(bool fullScreen);

	void SetWinDFullScreen(bool fullScreen);

	int GetWidth() { return width; }

public:
	SDL_Window* window;

	SDL_Surface* screenSurface;

	bool fullScreen = false;
	bool dFullScreen = false;

private:
	SString title;
	uint width;
	uint height;
	uint scale;
};

#endif // __WINDOW_H__