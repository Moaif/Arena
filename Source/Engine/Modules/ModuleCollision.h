#pragma once
#include<list>
#include "Module.h"
#include <SDL.h>
#include "../Components/Collider.h"

class GameObject;

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status Update()override;

	bool CleanUp()override;

	void SubscribeCollider(Collider& collider);
	void UnsubscribeCollider(Collider& collider);
	void DebugDraw();

private:

	std::list<Collider*> colliders;
	bool debug = false;
	bool hits[MAXIMO][MAXIMO];
};