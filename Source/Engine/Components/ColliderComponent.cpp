#include "ColliderComponent.h"
#include "../Application.h"
#include "../Modules/ModuleCollision.h"
#include "../GameObject.h"

RTTI_REGISTER(ColliderComponent)

ColliderComponent::ColliderComponent(BaseShape * originalShape)
{}

bool ColliderComponent::Init()
{
	bool ret = Component::Init();
	Collision->SubscribeCollider(*this);
	return ret;
}

update_status ColliderComponent::Update()
{
	update_status ret = Component::Update();
	if(m_shape)
	{
		originalShape->transform(m_shape,GetGameObject()->GetWorldTransform());
	}
	return ret;
}

bool ColliderComponent::CleanUp()
{
	bool ret = Component::CleanUp();
	Collision->UnsubscribeCollider(*this);
	return ret;
}

bool ColliderComponent::CheckCollision(const ColliderComponent & other) const
{
	return m_shape->intersect(*other.m_shape);
}

void ColliderComponent::DebugDraw()
{
	//TODO
}
