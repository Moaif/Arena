#pragma once
#include "Object.h"
#include "Vector.h"
#include <vector>
#include <list>
#include <memory>
#include "../Globals.h"
#include "RTTI.h"
#include "GameScene.h"
#include "Components/Component.h"

class ColliderComponent;

class GameObject: public Object {

	RTTI_ENABLE(GameObject,Object)

public:

	virtual ~GameObject(){};

	virtual update_status PreUpdate()override;
	virtual update_status Update()override;
	virtual bool CleanUp()override;

	virtual void OnCollision(ColliderComponent& other) {};

	virtual void OnTriggerEnter(ColliderComponent& other){};
	virtual void OnTriggerExit(ColliderComponent& other){};
	virtual void OnTriggerStay(ColliderComponent& other){};

	void Destroy(){SetToDelete(true);};
	
	template<class TYPE>
	TYPE* AddComponent(const std::string& className);
	template<class TYPE> 
	TYPE* GetComponent();

	GameObject* Instantiate(const std::string& className) { return m_currentScene->Instantiate(className); };
	GameObject* Instantiate(const std::string& className, fVector position, float angle = 0, GameObject * parent = nullptr) { return m_currentScene->Instantiate(className,position,angle,parent); };

	GameScene* GetGameScene()const{return m_currentScene;};
	void SetGameScene(GameScene* gameScene){m_currentScene = gameScene;};
	GameObject* GetParent()const{return m_parent;};
	void SetParent(GameObject& newParent);
	std::list<GameObject*>& GetChildren(){return m_children;};
	void AddChild(GameObject& child){m_children.push_back(&child);};
	void RemoveChild(GameObject& child){m_children.remove(&child);};
	bool IsActive()const {return m_active;};
	void SetActive(bool value){m_active = value;};
	virtual void SetToDelete(bool value = true) override;
	const Transform& GetWorldTransform();
	void SetWorldTransform(const Transform& transform);
	Transform& GetLocalTransform(){return m_localTransfrom;};
	void SetLocalTransform(const Transform& transform){m_localTransfrom = transform;};

protected:
	GameObject();
	//Won't be used but dont want to allow them public
	GameObject& operator=(const GameObject&){};
	GameObject(const GameObject&){};
	GameObject& operator=(GameObject&&)noexcept{};
	GameObject(GameObject&&)noexcept{};
	friend GameObject* GameScene::Instantiate(const std::string& className, const std::string& gameObjectName); //This will be the constructor of this class

private:
	GameScene* m_currentScene = nullptr;
	GameObject* m_parent = nullptr;
	std::list<GameObject*> m_children;
	bool m_active = true;
	Transform m_localTransfrom;
	Transform m_worldTransform;
	std::list<std::unique_ptr<Component>> m_toStartComponents;
	std::vector<std::unique_ptr<Component>> m_components;
};

template<class TYPE>
TYPE* GameObject::AddComponent(const std::string& className)
{
	TYPE* ret = nullptr;
	RTTIInfo rtti = RTTIRepo::instance()->getByName(className);
	m_toStartComponents.push_back(unique_ptr<Component>(rtti.createInstance<Component>()));
	if (ret = rtti_cast<TYPE>(m_toStartComponents.back().get())) {
		ret->SetGameObject(*this);
		ret->Init();
	}
	ASSERT(ret,"Failed to cast on Add component, not compatible types");
	return ret;
};

template<class TYPE>
TYPE* GameObject::GetComponent()
{
	TYPE* ret = nullptr;
	//First check with the started components
	for (std::vector<std::unique_ptr<Component>>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		ret = rtti_cast<TYPE>((*it).get());
		if (ret)
		{
			return ret;
		}
	}
	//Then with not started ones
	for (std::list<std::unique_ptr<Component>>::iterator it = m_toStartComponents.begin(); it != m_toStartComponents.end(); ++it)
	{
		ret = rtti_cast<TYPE>((*it).get());
		if (ret)
		{
			return ret;
		}
	}
	//Otherwise return nullptr;
	return ret;
};
