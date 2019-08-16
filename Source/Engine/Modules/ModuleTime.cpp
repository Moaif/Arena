#include "ModuleTime.h"
#include <SDL.h>


ModuleTime::ModuleTime() {
	m_previousTicks = 0;
	m_deltaTime = 0;
	m_unscaledDeltaTime = 0;
	m_scale = 1.0f;
}

ModuleTime::~ModuleTime() {

}

update_status ModuleTime::PreUpdate() {
	float currentTicks = (float)SDL_GetTicks();
	m_deltaTime = ((currentTicks - m_previousTicks)/1000.0f)*m_scale;
	m_unscaledDeltaTime = (currentTicks - m_previousTicks) / 1000.0f;
	m_previousTicks = currentTicks;

	return UPDATE_CONTINUE;
}

void ModuleTime::SetTimeScale(float value) {
	//Limit time scale in range 0-1
	value = MIN(1.0f, value);
	value = MAX(0.0f, value);
	m_scale = value;
}

float ModuleTime::GetDeltaTime() const {
	return m_deltaTime;
}

float ModuleTime::GetUnscaledDeltaTime() const {
	return m_unscaledDeltaTime;
}

float ModuleTime::GetTimeSinceStart() const {
	return (float)SDL_GetTicks() / 1000.0f;
}

float ModuleTime::GetTimeScale()const {
	return m_scale;
}