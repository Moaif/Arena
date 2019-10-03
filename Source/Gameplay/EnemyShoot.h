#pragma once
#include "Shoot.h"

class EnemyShoot : public Shoot
{
	RTTI_ENABLE(EnemyShoot, Shoot)
public:
	bool Start() override;
};