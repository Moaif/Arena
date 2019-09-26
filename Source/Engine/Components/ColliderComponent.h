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
	virtual bool CleanUp() override;

	void UpdateCollisionTransform();
	bool CheckCollision(ColliderComponent& other);
	void DebugDraw()const;

	CollisionType GetCollisionType()const{return m_collisionType;};
	void SetCollisionType(CollisionType type){m_collisionType = type;};

	bool GetIsTrigger()const {return m_isTrigger;};
	void SetIsTrigger(bool value){m_isTrigger = value;};

	bool GetIsStatic() const {return m_isStatic;};
	void SetIsStatic(bool value) {m_isStatic = value;};

	BaseShape* GetOriginalShape()const{return m_originalShape;};
	void SetOriginalShape(BaseShape* shape);

private:
	CollisionType m_collisionType;
	bool m_isTrigger;
	bool m_isStatic;
	BaseShape* m_originalShape;
	BaseShape* m_shape;
};