#pragma once
#include "Invader.h"

class InvaderPink : public Invader
{
	RTTI_ENABLE(InvaderPink, Invader)
public:
	bool Start() override;
};