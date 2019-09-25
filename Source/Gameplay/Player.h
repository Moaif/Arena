#pragma once
#include "../Engine/GameObject.h"

class Player : public GameObject
{
	RTTI_ENABLE(Player,GameObject)
public:
	bool Start() override;
	update_status Update()override;
	virtual void OnCollision(ColliderComponent& other)override;

	virtual void OnTriggerEnter(ColliderComponent& other)override;
	virtual void OnTriggerExit(ColliderComponent& other)override;
	virtual void OnTriggerStay(ColliderComponent& other)override;
};