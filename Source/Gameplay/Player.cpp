#include "Player.h"
#include "../Engine/Components/RendererComponent.h"
#include "../Engine/Components/ColliderComponent.h"
#include "../Engine/Modules/ModuleTextures.h"
#include "../Engine/Modules/ModuleInput.h"
#include "../Engine/Modules/ModuleTime.h"
#include "Game.h"

RTTI_REGISTER(Player)

const int InitLives = 3;
const float Speed = 700;
const float ShootCD = 0.5f;

bool Player::Start()
{
	bool ret = GameObject::Start();

	//Render component
	RendererComponent* renderer = dynamic_cast<RendererComponent*>(AddComponent("RendererComponent"));
	Animation anim = Animation();
	anim.frames.push_back({ 0, 0, 256, 256 });
	anim.texture = Textures->Load("assets/Spaceship.png");
	ASSERT(anim.texture, AT("Player failed on loading it's textures"));
	renderer->AddAnimation("Basic",anim);

	//Collider component
	ColliderComponent* collider = dynamic_cast<ColliderComponent*>(AddComponent("ColliderComponent"));
	collider->SetOriginalShape(new Shape<AABB>());
	static_cast<Shape<AABB>*>(collider->GetOriginalShape())->m_Shape = AABB(fVector(-128,-128),fVector(128,128));
	collider->SetCollisionType(CollisionType::PLAYER);
	collider->SetIsTrigger(true);

	GetLocalTransform().setScale(fVector(INVADERS_SCALE, INVADERS_SCALE));

	m_remainingLives = InitLives;
	m_shootTimer = 0;

	return ret;
}

update_status Player::Update()
{
	update_status ret = GameObject::Update();

	if(Input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if(GetWorldTransform().getPosition().x < SCREEN_WIDTH/2)
			GetLocalTransform().translate(GetLocalTransform().Right() * Speed * Time->GetDeltaTime());
	}
	else if(Input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (GetWorldTransform().getPosition().x > -SCREEN_WIDTH / 2)
			GetLocalTransform().translate(GetLocalTransform().Right() * -Speed *Time->GetDeltaTime());
	}

	if (Input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && m_shootTimer <= 0) 
	{
		Instantiate("PlayerShoot",GetWorldTransform().getPosition());
		m_shootTimer = ShootCD;
	}
	m_shootTimer -= Time->GetDeltaTime();

	return ret;
}

void Player::OnTriggerEnter(ColliderComponent & other)
{
	if (--m_remainingLives == 0) {
		Game* castedScene = dynamic_cast<Game*>(GetGameScene());
		ASSERT(castedScene, "error can't cast currentGameScene to \"Game\" scene");
		castedScene->EndLevel();
	}
}
