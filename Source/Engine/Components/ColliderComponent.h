#pragma once
#include "Component.h"
#include "../Collision.h"

enum CollisionType
{
	DEFAULT,
	PLAYER,
	OBSTACLE,
	SHOOT,
	ENEMY,
	MAX
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
	void DebugDraw()const;

	CollisionType GetCollisionType()const{return m_collisionType;};
	void SetCollisionType(CollisionType type){m_collisionType = type;};

	BaseShape* GetOriginalShape()const{return m_originalShape;};
	void SetOriginalShape(BaseShape* shape);

private:
	CollisionType m_collisionType;
	BaseShape* m_originalShape;
	BaseShape* m_shape;
};