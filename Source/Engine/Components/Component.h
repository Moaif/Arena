#pragma once

#include "../../Globals.h"

class GameObject;

class Component
{
public:
	virtual ~Component(){};

	virtual bool Init(){return true;};
	virtual bool Start(){return true;};
	virtual update_status PreUpdate()
	{
		if(m_toDelete)
		{
			CleanUp();
		}
		//TODO::Destroy component
		return update_status::UPDATE_CONTINUE;
	};
	virtual update_status Update(){return update_status::UPDATE_CONTINUE;};
	virtual update_status PostUpdate(){return update_status::UPDATE_CONTINUE;};
	virtual bool CleanUp(){return true;};

	GameObject* GetGameObject()const{return m_gameObject;};
	void SetGameObject(GameObject& gameObject){this->m_gameObject = &gameObject;};
	bool IsActive()const {return m_active;};
	void SetActive(bool value){m_active = value;};
	bool IsReadyToDelete()const{return m_toDelete;};
	bool SetToDelete(bool value){m_toDelete = value;};

private:
	GameObject* m_gameObject = nullptr;
	bool m_toDelete = false;
	bool m_active = true;;
};