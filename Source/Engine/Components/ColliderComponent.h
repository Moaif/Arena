#pragma once
#include "Component.h"
#include "../Collision.h"

enum CollisionType
{
	PLAYER,
	LASER,
	ENEMY_SHOOT,
	ENEMY,
	NO_DMG_ENEMY,
	MAXIMO
};

class ColliderComponent : public Component
{
	RTTI_ENABLE(ColliderComponent,Component)
public:
	ColliderComponent(BaseShape * originalShape = nullptr);

	virtual bool Init()override;
	virtual update_status Update()override;
	virtual bool CleanUp() override;

	bool CheckCollision(const ColliderComponent& other) const;
	void DebugDraw();

	CollisionType GetCollisionType()const{return m_collisionType;};
	void SetCollisionType(CollisionType type){m_collisionType = type;};

private:
	CollisionType m_collisionType;
	BaseShape* originalShape;
	BaseShape* m_shape;
};