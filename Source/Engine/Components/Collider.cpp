#include "Collider.h"
#include "../Application.h"
#include "../Modules/ModuleCollision.h"
#include "../GameObject.h"

Collider::Collider(BaseShape * originalShape)
{}

bool Collider::Init()
{
	bool ret = Component::Init();
	Collision->SubscribeCollider(*this);
	return ret;
}

update_status Collider::Update()
{
	update_status ret = Component::Update();
	if(m_shape)
	{
		originalShape->transform(m_shape,GetGameObject()->GetTransform());
	}
	return ret;
}

bool Collider::CleanUp()
{
	bool ret = Component::CleanUp();
	Collision->UnsubscribeCollider(*this);
	return ret;
}

bool Collider::CheckCollision(const Collider & other) const
{
	return m_shape->intersect(*other.m_shape);
}

void Collider::DebugDraw()
{
}
