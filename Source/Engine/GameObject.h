#pragma once
#include "Vector.h"
#include <vector>
#include "../Globals.h"
#include "RTTI.h"

class Component;
class Collider;

class GameObject {

	RTTI_ENABLE_BASE(GameObject)
	GameObject& operator=(const GameObject&){};
	GameObject(const GameObject&);

public:
	GameObject(GameObject* parent = nullptr);
	GameObject(Transform t, GameObject* parent = nullptr);

	virtual ~GameObject(){};

	virtual bool Init(){return true;};
	virtual bool Start(){return true;};
	virtual update_status Update(){return update_status::UPDATE_CONTINUE;};
	virtual bool CleanUp(){return true;};

	virtual void OnCollision(Collider& other) {}

	void AddComponent(Component* component);
	//TODO template<class TYPE> TYPE* GetComponent();

	GameObject* GetParent()const{return parent;};
	void SetParent(GameObject& newParent);
	bool IsActive()const {return m_active;};
	void SetActive(bool value){m_active = value;};
	bool IsReadyToDelete()const{return m_toDelete;};
	bool SetToDelete(bool value = true);
	const Transform& GetWorldTransform();
	void SetWorldTransform(const Transform& transform);
	const Transform& GetLocalTransform(){return m_localTransfrom;};
	void SetLocalTransform(const Transform& transform){m_localTransfrom = transform;};

private:
	GameObject* parent = nullptr;
	bool m_toDelete = false;
	bool m_active = true;
	Transform m_localTransfrom;
	Transform m_worldTransform;
	std::vector<Component*> m_components;//TODO: if not handled by a factory, components should be unique_ptr in the gameobject
};

