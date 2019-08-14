#include <math.h>
#include "../../Globals.h"
#include "../Application.h"
#include "ModuleFadeToBlack.h"
#include "ModuleRender.h"
#include "ModuleAudio.h"
#include "ModuleTime.h"
#include <SDL.h>

//This should be managed by app, not an additional module

ModuleFadeToBlack::ModuleFadeToBlack(bool start_enabled) :  Module(start_enabled)
{}

ModuleFadeToBlack::~ModuleFadeToBlack()
{}

// Load assets
bool ModuleFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(Renderer->GetRenderer(), SDL_BLENDMODE_BLEND);
	return true;
}

// Update: draw background
update_status ModuleFadeToBlack::Update()
{
	if(start_time > 0)
	{
		Uint32 now = SDL_GetTicks() - start_time;
		float normalized = (float) now / (float) total_time;
		
		if(normalized > 1.0f)
			normalized = 1.0f;
		
		if(fading_in == false)
			normalized = 1.0f - normalized;

		SDL_SetRenderDrawColor(Renderer->GetRenderer(), 0, 0, 0, (Uint8) (normalized * 255.0f));
		SDL_RenderFillRect(Renderer->GetRenderer(), NULL);

		if(module_off == nullptr && module_on != nullptr)
		{
			module_on->Enable();
			module_on = nullptr;
		}

		if(now >= total_time)
		{
			if(fading_in == true)
			{	
				Audio->PauseMusic();
				if(module_off != nullptr) 
					module_off->Disable();
				module_on->Enable();
				Time->SetTimeScale(1);

				total_time += total_time;
				start_time = SDL_GetTicks();
				fading_in = false;
			}
			else
			{
				start_time = 0;
			}
		}
	}

	return UPDATE_CONTINUE;
}

// Fade to black. At mid point deactivate one module, then activate the other
void ModuleFadeToBlack::FadeToBlack(Module* module_on, Module* module_off, float time)
{
	fading_in = (module_off != nullptr) ? true : false;
	start_time = SDL_GetTicks();
	total_time = (Uint32) (time  * 0.5f * 1000.0f);

	this->module_on = module_on;
	this->module_off = module_off;
	//Can't pause in transitions
	App->playing = false;
}

bool ModuleFadeToBlack::isFading() const
{
	return start_time > 0;
}