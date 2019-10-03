#include "Shoot.h"
#include "../Engine/Components/RendererComponent.h"
#include "../Engine/Components/ColliderComponent.h"
#include "../Engine/Modules/ModuleTextures.h"

RTTI_REGISTER(Shoot)

bool Shoot::Start() {
	bool ret = GameObject::Start();

	GetLocalTransform().setScale(fVector(INVADERS_SCALE, INVADERS_SCALE));

	return ret;
}

update_status Shoot::Update()
{
	update_status ret = GameObject::Update();

	GetLocalTransform().translate(GetLocalTransform().Forward() * speed * Time->GetDeltaTime());

	//if out of screen, destroy
	if (GetWorldTransform().getPosition().y > SCREEN_HEIGHT / 2 || GetWorldTransform().getPosition().y < -SCREEN_HEIGHT / 2)
		Destroy();

	return ret;
}

void Shoot::OnTriggerEnter(ColliderComponent& other)
{
	Destroy();
}
