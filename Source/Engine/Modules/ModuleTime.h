#pragma once
#include "../../Globals.h"
#include "Module.h"

class ModuleTime :public Module {
public:
	ModuleTime();
	~ModuleTime();

	update_status PreUpdate()override;

	void SetTimeScale(float value);

	float GetDeltaTime()const;
	float GetUnscaledDeltaTime() const;
	float GetTimeSinceStart()const;
	float GetTimeScale()const;
private:
	float m_previousTicks;
	float m_deltaTime;
	float m_unscaledDeltaTime;
	float m_scale;
};
