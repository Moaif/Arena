#pragma once
#include "../Engine/GameScene.h"

class Menu: public GameScene
{
	RTTI_ENABLE(Menu, GameScene)

public:
	virtual bool Start() override;
	virtual update_status Update() override;
	virtual bool CleanUp() override;

private:
	const class Font* font = nullptr;
	int fontLineReference = 0;
};