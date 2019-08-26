#pragma once
#include "../Engine/GameScene.h"

class TestScene : public GameScene
{
	RTTI_ENABLE(TestScene,GameScene)

public:

	virtual bool Start() override;

};