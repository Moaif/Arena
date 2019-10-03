#pragma once
#include "../Engine/GameObject.h"

class Player : public GameObject
{
	RTTI_ENABLE(Player,GameObject)
public:
	bool Start() override;
	update_status Update()override;

	virtual void OnTriggerEnter(ColliderComponent& other)override;

private:
	int m_remainingLives = 0;
	float m_shootTimer = 0;
};