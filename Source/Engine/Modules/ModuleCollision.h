#pragma once
#include<list>
#include "Module.h"
#include <SDL.h>
#include "../Components/ColliderComponent.h"
#include <set>

class GameObject;

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate() override;
	update_status Update() override;

	bool CleanUp()override;

	void SubscribeCollider(ColliderComponent& collider);
	void UnsubscribeCollider(ColliderComponent& collider);
	void DebugDraw()const;
	void MoveOutOfBounds(ColliderComponent& col1, ColliderComponent& col2);

private:

	std::list<ColliderComponent*> m_colliders;
	std::set<std::pair<ColliderComponent*, ColliderComponent*>> m_collidingTriggers;
	bool m_debug = false;
	bool m_hits[MAX][MAX];
};