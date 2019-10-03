#pragma once
#include "RTTI.h"
#include <memory>
#include <list>
#include "Vector.h"
#include "../Globals.h"

class GameObject;

class GameScene
{
	RTTI_ENABLE_BASE(GameScene)
public:
	//TODO: try to do the same as gameobject and make this private, already tried but as GameScene is used by
	// modulegameScene and has a unique_ptr to this, it requieres this class starts to ask for a complete definition of
	// gameObject, which would cause a header lock between those 2 files again
	GameScene();
	virtual ~GameScene();

	virtual bool Init(){return true;};
	virtual bool Start(){return true;};
	virtual update_status PreUpdate();
	virtual update_status Update();
	virtual bool CleanUp();

	GameObject* Instantiate(const std::string& className);
	GameObject* Instantiate(const std::string& className,fVector position, float angle = 0, GameObject* parent = nullptr);

	bool IsReadyToDelete() const{return m_toDelete;};
	void SetToDelete(bool value){m_toDelete = value;};

private:
	bool m_toDelete = false;
	std::list<std::unique_ptr<GameObject>> m_toStartGameObjects;
	std::vector<std::unique_ptr<GameObject>> m_gameObjects;
};