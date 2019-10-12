#include "PlayerShoot.h"
#include "../Engine/Components/RendererComponent.h"
#include "../Engine/Components/ColliderComponent.h"
#include "../Engine/Modules/ModuleTextures.h"

RTTI_REGISTER(PlayerShoot)

const float PlayerShootSpeed = 1500.0f;

bool PlayerShoot::Start()
{
	bool ret = Shoot::Start();

	//Render component
	RendererComponent* renderer = dynamic_cast<RendererComponent*>(AddComponent("RendererComponent"));
	Animation anim = Animation();
	anim.frames.push_back({ 0, 0, 128, 128 });
	anim.texture = Textures->Load("assets/spaceshipLazer.png");
	ASSERT(anim.texture, AT("Player failed on loading it's textures"));
	renderer->AddAnimation("Basic", anim);

	//Collider component
	ColliderComponent* collider = dynamic_cast<ColliderComponent*>(AddComponent("ColliderComponent"));
	collider->SetOriginalShape(new Shape<AABB>());
	static_cast<Shape<AABB>*>(collider->GetOriginalShape())->m_Shape = AABB(fVector(-64, -64), fVector(64, 64));
	collider->SetCollisionType(CollisionType::PLAYER_SHOOT);
	collider->SetIsTrigger(true);

	speed = PlayerShootSpeed;

	return ret;
}