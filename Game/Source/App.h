#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "List.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

#define WHITE  { 255,255,255,255 }
#define BLACK  { 0,0,0,255 }
#define YELLOW  { 255,192,0,255}
#define ORANGE  { 134,60,0,255 }
#define BROWN	{153,113,99,255}

// Modules
class Window;
class Input;
class Render;
class Textures;
class AudioManager;
class EntityManager;
class GuiManager;
class FontTTF;
class Scene;
class DialogueManager;
class Transition;
class QuestManager;
class AssetsManager;
class ParticleSystem;

class App
{
public:
	App(int argc, char* args[]);

	virtual ~App();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void AddModule(Module* module);

	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	uint32 GetFrameCount() const
	{
		return frameCount;
	}
	float GetDeltaTime() const
	{
		return dt;
	}

    // L02: DONE 1: Create methods to request Load / Save
	void LoadGameRequest();
	void SaveGameRequest() const;

	// Load config file
	// NOTE: It receives config document
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

private:

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGame();
	bool SaveGame() const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	AudioManager* audio;
	EntityManager* entityManager;
	GuiManager* guiManager;
	FontTTF* fontTTF;
	Scene* scene;
	DialogueManager* dialogueManager;
	Transition* transition;
	QuestManager* questManager;
	AssetsManager* assetsManager;
	ParticleSystem* psystem;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	// L01: DONE 2: Create new variables from pugui namespace
	// NOTE: Redesigned LoadConfig() to avoid storing this variables
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	mutable bool saveGameRequested;
	bool loadGameRequested;

	// L07: DONE 4: Calculate some timing measures
	// required variables are provided:
	PerfTimer ptimer;
	uint64 frameCount = 0;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;
	uint32 lastSecFrameCount = 0;
	uint32 prevLastSecFrameCount = 0;
	float dt = 0.0f;

	const uint32 frameDelay = 1000 / 60;

	int	cappedMs = -1;
};

extern App* app;

#endif	// __APP_H__