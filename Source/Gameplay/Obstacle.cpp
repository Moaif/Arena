#include "Obstacle.h"
#include "../Engine/Components/RendererComponent.h"
#include "../Engine/Components/ColliderComponent.h"
#include "../Engine/Modules/ModuleTextures.h"

RTTI_REGISTER(Obstacle)

bool Obstacle::Start()
{
	bool ret = GameObject::Start();

	//Render component
	RendererComponent* renderer = dynamic_cast<RendererComponent*>(AddComponent("RendererComponent"));
	Animation anim = Animation();
	anim.frames.push_back({ 1, 170, 56, 38 });
	anim.speed = 5.0f;
	anim.texture = Textures->Load("assets/ModuleParticles.png");
	ASSERT(anim.texture, AT("Player failed on loading it's textures"));
	renderer->AddAnimation("static", anim);

	//Collider component
	ColliderComponent* collider = dynamic_cast<ColliderComponent*>(AddComponent("ColliderComponent"));
	collider->SetOriginalShape(new Shape<AABB>());
	static_cast<Shape<AABB>*>(collider->GetOriginalShape())->m_Shape = AABB(fVector(-25, -25), fVector(25, 25));
	//collider->SetOriginalShape(new Shape<Circle>());
	//static_cast<Shape<Circle>*>(collider->GetOriginalShape())->m_Shape = Circle(fVector(0,0), 100);
	//collider->SetOriginalShape(new Shape<Line>());
	//static_cast<Shape<Line>*>(collider->GetOriginalShape())->m_Shape = Line(fVector(-20, -20), fVector(20, 20));
	collider->SetCollisionType(CollisionType::OBSTACLE);

	return ret;
}