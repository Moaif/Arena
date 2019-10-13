#include "DefaultObject.h"
#include "..//Engine/Components/RendererComponent.h"
#include "..//Engine/Modules/ModuleTextures.h"

RTTI_REGISTER(DefaultObject);

bool DefaultObject::Start()
{
	bool ret = GameObject::Start();

	RendererComponent* renderer = AddComponent<RendererComponent>("RendererComponent");
	Animation anim = Animation();
	anim.frames.push_back({ 0, 0, 256, 256 });
	anim.texture = Textures->Load("assets/Spaceship.png");
	ASSERT(anim.texture, AT("Player failed on loading it's textures"));
	renderer->AddAnimation("Basic", anim);

	return ret;
}
