#pragma once
#include "../../Globals.h"
#include "Module.h"
#include "../Vector.h"
#include <SDL.h>

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{

public:

	ModuleInput();

	virtual ~ModuleInput();

	bool Init()override;
	update_status PreUpdate()override;
	bool CleanUp()override;

	// Check key states (includes mouse and joy buttons)
	KeyState GetKey(int id) const
	{
		return m_keyboard[id];
	}

	KeyState GetMouseButtonDown(int id) const
	{
		return m_mouse_buttons[id - 1];
	}

	// Check for window events last frame
	bool GetWindowEvent(EventWindow code) const;

	// Get mouse / axis position
	const iVector& GetMouseMotion() const;
	const iVector& GetMousePosition() const;

private:
	bool		m_windowEvents[WE_COUNT];
	KeyState*	m_keyboard;
	KeyState	m_mouse_buttons[NUM_MOUSE_BUTTONS];
	iVector m_mouse_motion;
	iVector m_mouse;
};