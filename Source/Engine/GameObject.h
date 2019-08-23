#pragma once
#include "Vector.h"
#include <vector>
#include <list>
#include <memory>
#include "../Globals.h"
#include "RTTI.h"
#include "GameScene.h"
#include "Components/Component.h"

class Collider;

class GameObject {

	RTTI_ENABLE_BASE(GameObject)

public:

	virtual ~GameObject(){};

	virtual bool Init(){return true;};
	virtual bool Start(){return true;};
	virtual update_status PreUpdate();
	virtual update_status Update();
	virtual bool CleanUp();

	virtual void OnCollision(Collider& other) {}

	void Destroy(){SetToDelete(true);};

	Component* AddComponent(const std::string& className);
	template<class TYPE> 
	TYPE* GetComponent();

	GameObject* GetParent()const{return parent;};
	void SetParent(GameObject& newParent);
	std::list<GameObject*>& GetChildren(){return children;};
	void AddChild(GameObject& child){children.push_back(&child);};
	void RemoveChild(GameObject& child){children.remove(&child);};
	bool IsActive()const {return m_active;};
	void SetActive(bool value){m_active = value;};
	bool IsReadyToDelete()const{return m_toDelete;};
	bool SetToDelete(bool value = true);
	const Transform& GetWorldTransform();
	void SetWorldTransform(const Transform& transform);
	const Transform& GetLocalTransform(){return m_localTransfrom;};
	void SetLocalTransform(const Transform& transform){m_localTransfrom = transform;};

protected:
	GameObject();
	//Won't be used but dont want to allow them public
	GameObject& operator=(const GameObject&){};
	GameObject(const GameObject&){};
	GameObject& operator=(GameObject&&){};
	GameObject(GameObject&&){};
	friend GameObject* GameScene::Instantiate(const std::string& className); //This will be the constructor of this class

private:
	GameObject* parent = nullptr;
	std::list<GameObject*> children;
	bool m_toDelete = false;
	bool m_active = true;
	Transform m_localTransfrom;
	Transform m_worldTransform;
	std::list<std::unique_ptr<Component>> m_toStartComponents;
	std::vector<std::unique_ptr<Component>> m_components;
};

template<class TYPE>
TYPE * GameObject::GetComponent()
{
	TYPE* ret = nullptr;
	//First check with the started components
	for(std::vector<std::unique_ptr<Component>>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		ret = rtti_cast<TYPE>((*it).get());
		if(ret)
		{
			return ret;
		}
	}
	//Then with not started ones
	for(std::list<std::unique_ptr<Component>>::iterator it = m_toStartComponents.begin(); it != m_toStartComponents.end(); ++it)
	{
		ret = rtti_cast<TYPE>((*it).get());
		if(ret)
		{
			return ret;
		}
	}
	//Otherwise return nullptr;
	return ret;
}
