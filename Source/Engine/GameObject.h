#pragma once
#include "Vector.h"
#include <vector>

class Component;
class Collider;

class GameObject {
public:
	GameObject()
	{
		m_transform.setIdentity();
	}

	GameObject& operator=(const GameObject&);
	GameObject(const GameObject&);

	virtual ~GameObject();

	virtual void Init();
	virtual void Update();
	virtual void Render();

	virtual void OnCollision(Collider& other) {}

	void AddComponent(Component* component);
	//TODO template<class TYPE> TYPE* GetComponent();

	Transform& GetTransform(){return m_transform;};
	void SetTransform(const Transform& transform){m_transform = transform;};

public:
	Transform m_transform;
	std::vector<Component*> m_components;//TODO: if not handled by a factory, components should be unique_ptr in the gameobject
};

