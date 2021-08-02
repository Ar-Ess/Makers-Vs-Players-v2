#include "App.h"

#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "EntityManager.h"
#include "GuiManager.h"
#include "FontTTF.h"
#include "DialogueManager.h"
#include "AssetsManager.h"
#include "Scene.h"
#include "Transition.h"
#include "Player.h"
#include "ParticleSystem.h"
#include <time.h>

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

App::App(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	win = new Window();
	input = new Input();
	assetsManager = new AssetsManager();
	render = new Render();
	tex = new Textures();
	audio = new AudioManager();
	entityManager = new EntityManager();
	guiManager = new GuiManager();
	fontTTF = new FontTTF();
	scene = new Scene();
	dialogueManager = new DialogueManager();
	transition = new Transition();
	psystem = new ParticleSystem();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(assetsManager);
	AddModule(tex);
	AddModule(audio);
	AddModule(entityManager);
	AddModule(guiManager);
	AddModule(fontTTF);
	AddModule(scene);
	AddModule(dialogueManager);
	AddModule(transition);
	AddModule(psystem);

	// Render last to swap buffer
	AddModule(render);
	
	PERF_PEEK(ptimer);
}

App::~App()
{
	// Release modules
	/*ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}*/
	modules.RClear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

bool App::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());

		int cap = configApp.attribute("framerate_cap").as_int(-1);
		if (cap > 0) cappedMs = 1000 / cap;
	}

	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}
	
	PERF_PEEK(ptimer);

	configFile.reset();

	return ret;
}

bool App::Start()
{
	PERF_START(ptimer);
	
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}
	
	PERF_PEEK(ptimer);

	return ret;
}

bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();

	srand(time(NULL));

	return ret;
}

pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

void App::PrepareUpdate()
{
    frameCount++;
    lastSecFrameCount++;

	dt = frameTime.ReadSec();
	frameTime.Start();
}

void App::FinishUpdate()
{
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();
    
	if (lastSecFrameTime.Read() > 1000)
	{
		lastSecFrameTime.Start();
		prevLastSecFrameCount = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	float averageFps = float(frameCount) / startupTime.ReadSec();
	float secondsSinceStartup = startupTime.ReadSec();
	uint32 lastFrameMs = frameTime.Read();
	uint32 framesOnLastUpdate = prevLastSecFrameCount;

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last Frame Ms: %02u Last sec frames: %i Last dt: %.3f Time since startup: %.3f Frame Count: %I64u ",
			  averageFps, lastFrameMs, framesOnLastUpdate, dt, secondsSinceStartup, frameCount);

	app->win->SetTitle(title);

	if (frameDelay > lastFrameMs)
	{
		SDL_Delay(frameDelay - lastFrameMs);
	}
}

bool App::PreUpdate()
{
	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

bool App::PostUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

int App::GetArgc() const
{
	return argc;
}

const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

const char* App::GetTitle() const
{
	return title.GetString();
}

const char* App::GetOrganization() const
{
	return organization.GetString();
}

void App::LoadGameRequest()
{
	loadGameRequested = true;
}

void App::SaveGameRequest() const
{
	saveGameRequested = true;
}

bool App::LoadGame()
{
	bool ret = true;

	pugi::xml_document data;
	pugi::xml_parse_result doc = data.load_file("save_game.xml");
	pugi::xml_node playerInfo;

	if (doc == NULL)
	{
		LOG("There was an arror trying to load the game, take care of save_game.xml file. %s", doc.description());
		ret = false;
	}
	else
	{
		playerInfo = data.child("saveState").child("Player_Information");
		pugi::xml_node pos = playerInfo.child("Position");

		LOG("Loading finished...");
	}

	loadGameRequested = false;

	data.reset();

	return ret;
}

bool App::SaveGame() const
{
	bool ret = true;

	pugi::xml_document saveDoc;
	pugi::xml_node root = saveDoc.append_child("saveState");
	pugi::xml_node saveNode;
	pugi::xml_node playerInfo;
	

	if (root != NULL)
	{
		//PLAYER INFO
		saveNode = root.append_child("Player_Information");
		playerInfo = saveNode.append_child("Position");

		saveDoc.save_file("save_game.xml");
		LOG("Game saved correctly");
	}
	else
	{
		LOG("Error on append child of the save_game.xml file");
		ret = false;
	}

	saveDoc.reset();

	saveGameRequested = false;

	return ret;
}
