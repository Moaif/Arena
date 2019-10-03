#pragma once
#include "Invader.h"

class InvaderRed: public Invader
{
	RTTI_ENABLE(InvaderRed, Invader)
public:
	bool Start() override;
};