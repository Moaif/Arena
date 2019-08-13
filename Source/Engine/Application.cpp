#include "Application.h"
#include "./Modules/ModuleAudio.h"
#include "./Modules/ModuleCollision.h"
#include "./Modules/ModuleFadeToBlack.h"
#include "./Modules/ModuleFont.h"
#include "./Modules/ModuleInput.h"
#include "./Modules/ModuleRender.h"
#include "./Modules/ModuleTextures.h"
#include "./Modules/ModuleTime.h"
#include "./Modules/ModuleWindow.h"
#include "../Gameplay/ModuleSceneSega.h"

using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());
	modules.push_back(time = new ModuleTime());
	modules.push_back(fonts = new ModuleFont());
	modules.push_back(collision = new ModuleCollision());

	modules.push_back(sega = new ModuleSceneSega());

	//Renderer must be here to draw from buffer after all other modules had request to blit
	modules.push_back(renderer = new ModuleRender());

	//Fade to black is the last one, in order to work properly
	modules.push_back(fade = new ModuleFadeToBlack());

	playing = false;
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	fade->FadeToBlack(sega, nullptr, 3.0f);

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();

	//Game pause
	if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && playing) {
		App->renderer->PostUpdate();
		while (true)
		{
			time->PreUpdate();//In order to not stack aditional time on deltaTime
			//Print pause over the render on interval
			if (input->PreUpdate() == UPDATE_STOP) {
				return UPDATE_STOP;
			 }
			if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
				break;
			}
		}
		//on exit, restablish original render, to continue
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	return ret;
}

bool Application::Restart()
{
	bool ret = true;
	for (list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
			ret = (*it)->Restart();

	return ret;
}

