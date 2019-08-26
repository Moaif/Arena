#include "Player.h"
#include "../Engine/Components/RendererComponent.h"
#include "../Engine/Modules/ModuleTextures.h"
#include "../Engine/Modules/ModuleInput.h"
#include "../Engine/Modules/ModuleTime.h"

RTTI_REGISTER(Player)

const float angleToRotate = 1;
const float speed = 10;

bool Player::Start()
{
	bool ret = GameObject::Start();

	RendererComponent* renderer = dynamic_cast<RendererComponent*>(AddComponent("RendererComponent"));
	Animation anim = Animation();
	anim.frames.push_back({ 4, 4, 20, 47 });
	anim.frames.push_back({ 25, 4, 20, 47 });
	anim.frames.push_back({ 49, 2, 25, 49 });
	anim.frames.push_back({ 75, 3, 21, 47 });
	anim.speed = 5.0f;
	anim.texture = Textures->Load("assets/character.png");
	ASSERT(anim.texture, AT("Player failed on loading it's textures"));
	renderer->AddAnimation("run",anim);

	return ret;
}

update_status Player::Update()
{
	update_status ret = GameObject::Update();

	if(Input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		GetLocalTransform().rotate(angleToRotate);
	}
	else if(Input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		GetLocalTransform().rotate(-angleToRotate);
	}
	else if(Input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		//TODO make transform.forward to move in front
		GetLocalTransform().translate(fVector(0, -speed));
	}
	else if(Input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		GetLocalTransform().translate(fVector(0, speed));
	}

	return ret;
}
