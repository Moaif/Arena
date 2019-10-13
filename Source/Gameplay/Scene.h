#pragma once
#include "..//Engine/GameScene.h"

class Scene : public GameScene {
	RTTI_ENABLE(Scene,GameScene);
public:
	virtual bool Start() override;
};