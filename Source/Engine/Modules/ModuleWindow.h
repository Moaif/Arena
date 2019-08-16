#pragma once
#include "Module.h"

struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow();
	virtual ~ModuleWindow();

	bool Init()override;
	bool CleanUp()override;

	SDL_Window* GetWindow() const{return m_window;};

private:
	SDL_Window* m_window = nullptr;
};