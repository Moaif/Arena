#pragma once
#include "RTTI.h"
#include <memory>
#include "Vector.h"
#include "../Globals.h"

class GameObject;

class GameScene
{
	RTTI_ENABLE_BASE(GameScene)
public:
	GameScene();
	virtual ~GameScene();

	virtual bool Init(){return true;};
	virtual bool Start(){return true;};
	virtual update_status PreUpdate();
	virtual update_status Update();
	virtual bool CleanUp(){return true;};

	GameObject* Instantiate(const std::string& ClassName);
	GameObject* Instantiate(const std::string& ClassName,fVector position, float angle = 0, GameObject* parent = nullptr);

private:
	std::vector<std::unique_ptr<GameObject>> toStartGameObjects;
	std::vector<std::unique_ptr<GameObject>> gameObjects;
};