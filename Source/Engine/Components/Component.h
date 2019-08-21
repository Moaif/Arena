#pragma once
#include "../../Globals.h"
#include "../RTTI.h"

class GameObject;

class Component
{
	RTTI_ENABLE_BASE(Component)
public:
	Component(){};
	virtual ~Component(){};

	virtual bool Init(){return true;};
	virtual bool Start(){return true;};
	virtual update_status Update(){return update_status::UPDATE_CONTINUE;};
	virtual bool CleanUp(){return true;};

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