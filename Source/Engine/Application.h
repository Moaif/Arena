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

	inline static ModuleRender* GetRenderer() {return App->m_renderer.get(); };
	inline static ModuleWindow* GetWindow() {return App->m_window.get(); };
	inline static ModuleTextures* GetTextures() {return App->m_textures.get(); };
	inline static ModuleInput* GetInput() {return App->m_input.get(); };
	inline static ModuleAudio* GetAudio() {return App->m_audio.get(); };
	inline static ModuleFadeToBlack* GetFade() {return App->m_fade.get(); };
	inline static ModuleCollision* GetCollision() {return App->m_collision.get(); };
	inline static ModuleFont* GetFonts() {return App->m_fonts.get(); };
	inline static ModuleTime* GetTime(){return App->m_time.get();};

public:
	//TODO: maybe set a function for pause that goes through all modules and so on
	bool playing;

private:
	std::vector<Module*> modules;

	std::unique_ptr<ModuleRender> m_renderer = nullptr;
	std::unique_ptr <ModuleWindow> m_window = nullptr;
	std::unique_ptr<ModuleTextures> m_textures = nullptr;
	std::unique_ptr<ModuleInput> m_input = nullptr;
	std::unique_ptr<ModuleAudio> m_audio = nullptr;
	std::unique_ptr<ModuleFadeToBlack> m_fade = nullptr;
	std::unique_ptr<ModuleCollision> m_collision = nullptr;
	std::unique_ptr<ModuleFont> m_fonts = nullptr;
	std::unique_ptr<ModuleTime> m_time = nullptr;

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