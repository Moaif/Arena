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

class Collider: public Component
{
	RTTI_ENABLE(Collider,Component)
public:
	Collider(BaseShape * originalShape = nullptr);

	virtual bool Init()override;
	virtual update_status Update()override;
	virtual bool CleanUp() override;

	bool CheckCollision(const Collider& other) const;
	void DebugDraw();

	CollisionType GetCollisionType()const{return m_collisionType;};
	void SetCollisionType(CollisionType type){m_collisionType = type;};

private:
	CollisionType m_collisionType;
	BaseShape* originalShape;
	BaseShape* m_shape;
};