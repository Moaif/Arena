#include "ColliderComponent.h"
#include "../Application.h"
#include "../Modules/ModuleCollision.h"
#include "../GameObject.h"

RTTI_REGISTER(ColliderComponent)

ColliderComponent::ColliderComponent(BaseShape * originalShape): m_originalShape(originalShape),
m_shape(nullptr), m_collisionType(CollisionType::DEFAULT)
{
}

bool ColliderComponent::Init()
{
	bool ret = Component::Init();
	if(m_originalShape)
	{
		m_shape = m_originalShape->transform(GetGameObject()->GetWorldTransform());
	}
	Collision->SubscribeCollider(*this);
	return ret;
}

update_status ColliderComponent::Update()
{
	update_status ret = Component::Update();
	if(m_shape)
	{
		m_originalShape->transform(m_shape,GetGameObject()->GetWorldTransform());
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
	if(!other.m_shape || !m_shape)
	{
		return false;
	}
	return m_shape->intersect(*other.m_shape);
}

void ColliderComponent::DebugDraw()const
{
	m_shape->DebugDraw(GetGameObject()->GetWorldTransform());
}

void ColliderComponent::SetOriginalShape(BaseShape * shape)
{
	ASSERT(shape,"Invalid Shape in ColliderComponent");

	m_originalShape = shape;
	m_shape = m_originalShape->transform(GetGameObject()->GetWorldTransform());
}
