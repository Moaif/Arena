#pragma once
#include "../Engine/GameObject.h"

class Background : public GameObject
{
	RTTI_ENABLE(Background, GameObject)
public:
	virtual bool Start() override;
};