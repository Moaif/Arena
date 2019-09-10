#pragma once
#include "../../Globals.h"
#include "../RTTI.h"

class GameObject;

class Component
{
	RTTI_ENABLE_BASE(Component)
public:
	//TODO: try to do the same as gameobject and make this private, already tried but as gameObject is used by
	// gameScene and has a unique_ptr to this, it requieres the complete definition so gameScene can build its own
	// unique_ptr of gameObjects, so i was stuck in a header include lock
	Component(){};
	virtual ~Component(){};

	virtual bool Init(){return true;};
	virtual bool Start(){return true;};
	virtual update_status Update(){return update_status::UPDATE_CONTINUE;};
	virtual bool CleanUp(){return true;};

	void Destroy(){SetToDelete(true);};

	GameObject* GetGameObject()const{return m_gameObject;};
	void SetGameObject(GameObject& gameObject){this->m_gameObject = &gameObject;};
	bool IsActive()const {return m_active;};
	void SetActive(bool value){m_active = value;};
	bool IsReadyToDelete()const{return m_toDelete;};
	bool SetToDelete(bool value = true){m_toDelete = value;};
private:
	GameObject* m_gameObject = nullptr;
	bool m_toDelete = false;
	bool m_active = true;;
};