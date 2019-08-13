#ifndef _MODULESCENESEGA_H_
#define _MODULESCENESEGA_H_

#include "../Engine/Modules/Module.h"
#include "../Engine/Animation.h"

struct SDL_Texture;

struct floatRect {
	float x;
	float y;
	float w;
	float h;
};

class ModuleSceneSega :public Module {
public:
	ModuleSceneSega(bool active=true);
	~ModuleSceneSega();

	bool Start()override;
	update_status Update()override;
	bool CleanUp()override;

private:
	SDL_Texture* background = nullptr;
	uint fx = 0;
	SDL_Rect totalRect;
	floatRect image;
	float timer;
	static const float OFFSET;
	static const float INTERVAL;
};

#endif // !_MODULESCENESEGA_H_

