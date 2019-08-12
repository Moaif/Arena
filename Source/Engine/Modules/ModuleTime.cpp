#include "ModuleTime.h"
#include <SDL.h>


ModuleTime::ModuleTime() {
	previousTicks = 0;
	deltaTime = 0;
	unscaledDeltaTime = 0;
	scale = 1.0f;
}

ModuleTime::~ModuleTime() {

}

update_status ModuleTime::PreUpdate() {
	float currentTicks = (float)SDL_GetTicks();
	deltaTime = ((currentTicks - previousTicks)/1000.0f)*scale;
	unscaledDeltaTime = (currentTicks - previousTicks) / 1000.0f;
	previousTicks = currentTicks;

	return UPDATE_CONTINUE;
}

void ModuleTime::SetTimeScale(float value) {
	if (value > 1.0f) {
		value = 1.0f;
	}

	if (value < 0) {
		value = 0;
	}
	scale = value;
}

float ModuleTime::GetDeltaTime() const {
	return deltaTime;
}

float ModuleTime::GetUnscaledDeltaTime() const {
	return unscaledDeltaTime;
}

float ModuleTime::GetTimeSinceStart() const {
	return (float)SDL_GetTicks() / 1000.0f;
}

float ModuleTime::GetTimeScale()const {
	return scale;
}