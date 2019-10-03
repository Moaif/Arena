#pragma once
#include "Invader.h"

class InvaderYellow : public Invader
{
	RTTI_ENABLE(InvaderYellow, Invader)
public:
	bool Start() override;
};