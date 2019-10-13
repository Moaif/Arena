#pragma once
#include "..//Engine/GameObject.h"

class DefaultObject : public GameObject {
	RTTI_ENABLE(DefaultObject, GameObject);
public:
	virtual bool Start()override;
};