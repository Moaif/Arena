#pragma once
#include "Shoot.h"

class PlayerShoot : public Shoot
{
	RTTI_ENABLE(PlayerShoot, Shoot)
public:
	bool Start() override;
};