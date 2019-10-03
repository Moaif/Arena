#pragma once
#include "../Engine/GameObject.h"

class Shoot : public GameObject
{
	RTTI_ENABLE(Shoot, GameObject)
public:
	virtual bool Start() override;
	virtual update_status Update() override;

	virtual void OnTriggerEnter(ColliderComponent& other)override;

protected:
	float speed = 1.0f;
};