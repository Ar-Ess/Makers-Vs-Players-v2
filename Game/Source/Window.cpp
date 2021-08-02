#include "Input.h"
#include "Window.h"
#include "App.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL.h"


Window::Window() : Module()
{
	window = NULL;
	screenSurface = NULL;
	name.Create("window");
}

Window::~Window()
{
}

bool Window::Awake(pugi::xml_node& config)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		// Create window
		// L01: DONE 6: Load all required configurations from config.xml
		uint32 flags = SDL_WINDOW_SHOWN;
		fullScreen = config.child("fullscreen").attribute("value").as_bool(false);
		bool borderless = config.child("borderless").attribute("value").as_bool(false);
		bool resizable = config.child("resizable").attribute("value").as_bool(false);
		bool fullscreenWindow = config.child("fullscreenWindow").attribute("value").as_bool(false);

		width = config.child("resolution").attribute("width").as_int(640);
		height = config.child("resolution").attribute("height").as_int(480);
		scale = config.child("resolution").attribute("scale").as_int(1);

		if (fullScreen == true) flags |= SDL_WINDOW_FULLSCREEN;
		if (borderless == true) flags |= SDL_WINDOW_BORDERLESS;
		if (resizable == true) flags |= SDL_WINDOW_RESIZABLE;
		if (fullscreenWindow == true) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

		window = SDL_CreateWindow(app->GetTitle(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			// Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

bool Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	// Destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	// Quit SDL subsystems
	SDL_Quit();
	return true;
}

void Window::SetTitle(const char* newTitle)
{
	SDL_SetWindowTitle(window, newTitle);
}

void Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}

void Window::SetWinFullScreen(bool full)
{
	if (full) SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
	else if (!full) SDL_SetWindowFullscreen(app->win->window, 0);
}

void Window::SetWinDFullScreen(bool full)
{
	if (full) SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else if (!full) SDL_SetWindowFullscreen(app->win->window, 0);
}

uint Window::GetScale() const
{
	return scale;
}

bool Window::FullScreenLogic()
{
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (fullScreen) SetWinFullScreen(false);
		else if (!fullScreen) SetWinFullScreen(true);

		fullScreen = !fullScreen;
	}

	return true;
}
