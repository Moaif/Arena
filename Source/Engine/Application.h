#pragma once
#include <vector>
#include "../Globals.h"
#include "./Modules/Module.h"
#include <memory>

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleAudio;
class ModuleFadeToBlack;
class ModuleCollision;
class ModuleFont;
class ModuleTime;
class ModuleSceneSega;

extern Application* App;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	bool Restart();

	inline static ModuleRender* GetRenderer() {return App->renderer.get(); };
	inline static ModuleWindow* GetWindow() {return App->window.get(); };
	inline static ModuleTextures* GetTextures() {return App->textures.get(); };
	inline static ModuleInput* GetInput() {return App->input.get(); };
	inline static ModuleAudio* GetAudio() {return App->audio.get(); };
	inline static ModuleFadeToBlack* GetFade() {return App->fade.get(); };
	inline static ModuleCollision* GetCollision() {return App->collision.get(); };
	inline static ModuleFont* GetFonts() {return App->fonts.get(); };
	inline static ModuleTime* GetTime(){return App->time.get();};

public:
	//TODO: maybe set a function for pause that goes through all modules and so on
	bool playing;

private:
	std::vector<Module*> modules;

	std::unique_ptr<ModuleRender> renderer = nullptr;
	std::unique_ptr <ModuleWindow> window = nullptr;
	std::unique_ptr<ModuleTextures> textures = nullptr;
	std::unique_ptr<ModuleInput> input = nullptr;
	std::unique_ptr<ModuleAudio> audio = nullptr;
	std::unique_ptr<ModuleFadeToBlack> fade = nullptr;
	std::unique_ptr<ModuleCollision> collision = nullptr;
	std::unique_ptr<ModuleFont> fonts = nullptr;
	std::unique_ptr<ModuleTime> time = nullptr;

	//TODO: remove, now its here for testing
	ModuleSceneSega* sega = nullptr;
};

#define Renderer Application::GetRenderer()
#define Window Application::GetWindow()
#define Textures Application::GetTextures()
#define Input Application::GetInput()
#define Audio Application::GetAudio()
#define Fade Application::GetFade()
#define Collision Application::GetCollision()
#define Fonts Application::GetFonts()
#define Time Application::GetTime()