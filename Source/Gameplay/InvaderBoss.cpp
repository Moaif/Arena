#include "InvaderBoss.h"
#include "../Engine/Components/RendererComponent.h"
#include "../Engine/Components/ColliderComponent.h"
#include "../Engine/Modules/ModuleTextures.h"

RTTI_REGISTER(InvaderBoss)

bool InvaderBoss::Start()
{
	bool ret = Invader::Start();

	//Render component
	RendererComponent* renderer = dynamic_cast<RendererComponent*>(AddComponent("RendererComponent"));
	Animation anim = Animation();
	anim.frames.push_back({ 0, 0, 256, 256 });
	anim.texture = Textures->Load("assets/invadership.png");
	ASSERT(anim.texture, AT("Player failed on loading it's textures"));
	renderer->AddAnimation("Basic", anim);

	//Collider component
	ColliderComponent* collider = dynamic_cast<ColliderComponent*>(AddComponent("ColliderComponent"));
	collider->SetOriginalShape(new Shape<AABB>());
	static_cast<Shape<AABB>*>(collider->GetOriginalShape())->m_Shape = AABB(fVector(-128, -128), fVector(128, 128));
	collider->SetCollisionType(CollisionType::ENEMY);
	collider->SetIsTrigger(true);

	return ret;
}