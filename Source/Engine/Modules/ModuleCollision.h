#pragma once
#include<list>
#include "Module.h"
#include <SDL.h>
#include "../Components/ColliderComponent.h"

class GameObject;

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status Update()override;

	bool CleanUp()override;

	void SubscribeCollider(ColliderComponent& collider);
	void UnsubscribeCollider(ColliderComponent& collider);
	void DebugDraw();

private:

	std::list<ColliderComponent*> colliders;
	bool debug = false;
	bool hits[MAXIMO][MAXIMO];
};