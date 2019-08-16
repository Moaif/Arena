#include "../Application.h"
#include "ModuleInput.h"
#include <SDL.h>

ModuleInput::ModuleInput() : Module(), m_mouse({0, 0}), m_mouse_motion({0,0})
{
	m_keyboard = new KeyState[MAX_KEYS];
	memset(m_keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
	memset(m_mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	RELEASE_ARRAY(m_keyboard);
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate()
{
	static SDL_Event event;

	m_mouse_motion = {0, 0};
	memset(m_windowEvents, false, WE_COUNT * sizeof(bool));
	
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(m_keyboard[i] == KEY_IDLE)
				m_keyboard[i] = KEY_DOWN;
			else
				m_keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(m_keyboard[i] == KEY_REPEAT || m_keyboard[i] == KEY_DOWN)
				m_keyboard[i] = KEY_UP;
			else
				m_keyboard[i] = KEY_IDLE;
		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(m_mouse_buttons[i] == KEY_DOWN)
			m_mouse_buttons[i] = KEY_REPEAT;

		if(m_mouse_buttons[i] == KEY_UP)
			m_mouse_buttons[i] = KEY_IDLE;
	}

	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				m_windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					m_windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					m_windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				m_mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

			case SDL_MOUSEBUTTONUP:
				m_mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

			case SDL_MOUSEMOTION:
				m_mouse_motion.x = event.motion.xrel;
				m_mouse_motion.y = event.motion.yrel;
				m_mouse.x = event.motion.x;
				m_mouse.y = event.motion.y;
			break;
		}
	}

	if(GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

// ---------
bool ModuleInput::GetWindowEvent(EventWindow ev) const
{
	return m_windowEvents[ev];
}

const iVector& ModuleInput::GetMousePosition() const
{
	return m_mouse;
}

const iVector& ModuleInput::GetMouseMotion() const
{
	return m_mouse_motion;
}