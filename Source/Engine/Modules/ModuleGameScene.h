#pragma once
#include "../../Globals.h"
#include <vector>
#include <memory>
#include "Module.h"
#include "../GameScene.h"

class ModuleGameScene : public Module
{
public:
	virtual bool Start() override;
	virtual update_status PreUpdate() override;
	virtual update_status Update() override;

	GameScene* AddScene(const std::string& className);
	GameScene* ReplaceSceneWithNew(const std::string& className);
	void RemoveCurrentScene();

private:
	void PopScene();

private:
	std::vector<std::unique_ptr<GameScene>> m_sceneStack;
	std::unique_ptr<GameScene> m_nextScene = nullptr;
	int m_popRequests = 0;
};