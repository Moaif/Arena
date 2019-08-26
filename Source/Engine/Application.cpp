#include "Application.h"
#include "Modules/ModuleAudio.h"
#include "Modules/ModuleCollision.h"
#include "Modules/ModuleFont.h"
#include "Modules/ModuleInput.h"
#include "Modules/ModuleRender.h"
#include "Modules/ModuleTextures.h"
#include "Modules/ModuleTime.h"
#include "Modules/ModuleWindow.h"
#include "Modules/ModuleGameScene.h"

using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back((m_input = std::make_unique<ModuleInput>()).get());
	modules.push_back((m_window = std::make_unique <ModuleWindow>()).get());

	modules.push_back((m_textures = std::make_unique<ModuleTextures>()).get());
	modules.push_back((m_audio = std::make_unique<ModuleAudio>()).get());
	modules.push_back((m_time = std::make_unique<ModuleTime>()).get());
	modules.push_back((m_fonts = std::make_unique<ModuleFont>()).get());
	modules.push_back((m_collision = std::make_unique<ModuleCollision>()).get());
	modules.push_back((m_scene = std::make_unique < ModuleGameScene>()).get());

	//Renderer must be here to draw from buffer after all other modules had request to blit
	modules.push_back((m_renderer = std::make_unique <ModuleRender>()).get());

	playing = false;
}

Application::~Application()
{
}

bool Application::Init()
{
	bool ret = true;

	for(vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	for(vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate();

	for(vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update();

	for(vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();
	//TODO move this to a Module which handles the current state
	//Game pause
	if (m_input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && playing) {
		App->m_renderer->PostUpdate();
		while (true)
		{
			m_time->PreUpdate();//In order to not stack aditional time on deltaTime
			//Print pause over the render on interval
			if (m_input->PreUpdate() == UPDATE_STOP) {
				return UPDATE_STOP;
			 }
			if (m_input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
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

	for(vector<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	return ret;
}

bool Application::Restart()
{
	bool ret = true;
	for (vector<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
			ret = (*it)->Restart();

	return ret;
}

