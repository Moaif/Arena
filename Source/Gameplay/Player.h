#pragma once
#include "../Engine/GameObject.h"

class Player : public GameObject
{
	RTTI_ENABLE(Player,GameObject)
public:
	bool Start() override;
	update_status Update()override;
};