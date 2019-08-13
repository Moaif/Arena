#ifndef __APPLICATION_CPP__
#define __APPLICATION_CPP__

#include<list>
#include "../Globals.h"
#include "./Modules/Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;
class ModuleParticles;
class ModuleFont;
class ModuleTime;
class ModuleSceneSega;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	bool Restart();

public:
	ModuleRender* renderer=nullptr;
	ModuleWindow* window=nullptr;
	ModuleTextures* textures=nullptr;
	ModuleInput* input=nullptr;
	ModuleAudio* audio=nullptr;
	ModuleFadeToBlack* fade=nullptr;
	ModuleCollision* collision=nullptr;
	ModuleParticles* particles=nullptr;
	ModuleFont* fonts=nullptr;
	ModuleTime* time=nullptr;

	ModuleSceneSega* sega = nullptr;

	bool playing;

private:

	std::list<Module*> modules;
};

extern Application* App;

#endif // __APPLICATION_CPP__