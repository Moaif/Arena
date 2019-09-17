#pragma once
#include "../Engine/GameObject.h"

class Obstacle : public GameObject
{
	RTTI_ENABLE(Obstacle, GameObject)
public:
	bool Start() override;
};