#pragma once
#include "../Engine/GameObject.h"

class Invader : public GameObject
{
	RTTI_ENABLE(Invader, GameObject)
public:
	virtual bool Start() override;
	virtual update_status Update() override;
	void Right();
	void Left();
	void Down();
	void TryDropBomb();

	virtual void OnTriggerEnter(ColliderComponent& other)override;

private:
	float m_tryToShootTimer = 0;
};