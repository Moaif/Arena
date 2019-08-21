#pragma once
#include "../../Globals.h"
#include <stack>
#include <memory>
#include "Module.h"

class GameScene;

class ModuleGameScene : public Module
{
public:

	virtual bool Init() override;
	virtual bool Start() override;
	virtual update_status PreUpdate() override;
	virtual update_status Update() override;
	virtual update_status PostUpdate() override;
	virtual bool CleanUp() override;

	void AddScene(GameScene& scene);

private:
	std::stack<std::unique_ptr<GameScene>> sceneStack;
};