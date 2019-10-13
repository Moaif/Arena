#pragma once
#include "Object.h"
#include "RTTI.h"
#include <memory>
#include <list>
#include "Vector.h"
#include "../Globals.h"

class GameObject;

class GameScene:public Object
{
	RTTI_ENABLE(GameScene,Object)
public:
	//TODO: try to do the same as gameobject and make this private, already tried but as GameScene is used by
	// modulegameScene and has a unique_ptr to this, it requieres this class starts to ask for a complete definition of
	// gameObject, which would cause a header lock between those 2 files again
	GameScene();
	virtual ~GameScene();

	virtual update_status PreUpdate()override;
	virtual update_status Update() override;
	virtual bool CleanUp()override;

	GameObject* Instantiate(const std::string& className, const std::string& gameObjectName = "");
	GameObject* Instantiate(const std::string& className,fVector position, float angle = 0, GameObject* parent = nullptr, const std::string & gameObjectName = "");

	virtual void SetToDelete(bool value)override;

private:
	bool m_toDelete = false;
	std::list<std::unique_ptr<GameObject>> m_toStartGameObjects;
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};