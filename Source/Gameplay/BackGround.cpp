#include "Background.h"
#include "../Engine/Components/RendererComponent.h"
#include "../Engine/Modules/ModuleTextures.h"

RTTI_REGISTER(Background)

bool Background::Start() {
	bool ret = GameObject::Start();

	//Render component
	RendererComponent* renderer = dynamic_cast<RendererComponent*>(AddComponent("RendererComponent"));
	Animation anim = Animation();
	anim.frames.push_back({ 0, 0, 1024, 1024 });
	anim.texture = Textures->Load("assets/Background.png");
	ASSERT(anim.texture, AT("Player failed on loading it's textures"));
	renderer->AddAnimation("Basic", anim);
	renderer->SetLayer(Layer::Background);

	return ret;
}