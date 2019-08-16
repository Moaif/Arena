#include "../../Globals.h"
#include "../Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

ModuleTextures::ModuleTextures()
{
}

ModuleTextures::~ModuleTextures()
{
	IMG_Quit();
}

bool ModuleTextures::Init()
{
	LOG("Init Image library");

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		return false;
	}

	return true;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");

	for(list<SDL_Texture*>::iterator it = m_textures.begin(); it != m_textures.end(); ++it)
		SDL_DestroyTexture(*it);

	m_textures.clear();
	return true;
}

// Load new texture from file path
SDL_Texture* const ModuleTextures::Load(const char* path)
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(path);

	if(surface == nullptr)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(Renderer->GetRenderer(), surface);

		if(texture == nullptr)
		{
			LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			m_textures.push_back(texture);
		}

		SDL_FreeSurface(surface);
	}

	return texture;
}

// Free texture from memory
void ModuleTextures::Unload(SDL_Texture* texture)
{
	for(list<SDL_Texture*>::iterator it = m_textures.begin(); it != m_textures.end(); ++it)
	{
		if(*it == texture)
		{
			SDL_DestroyTexture(*it);
			m_textures.erase(it);
			break;
		}
	}
}