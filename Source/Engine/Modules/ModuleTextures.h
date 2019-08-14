#pragma once
#include<list>
#include "Module.h"
#include "../../Globals.h"

struct SDL_Texture;

class ModuleTextures : public Module
{
public:
	ModuleTextures();
	~ModuleTextures();

	bool Init()override;
	bool CleanUp()override;

	SDL_Texture* const Load(const char* path);
	void Unload(SDL_Texture* texture);

private:
	std::list<SDL_Texture*> textures;
};