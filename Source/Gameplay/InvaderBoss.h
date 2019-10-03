#pragma once
#include "Invader.h"

class InvaderBoss : public Invader
{
	RTTI_ENABLE(InvaderBoss, Invader)
public:
	bool Start() override;
};