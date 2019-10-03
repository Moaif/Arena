#include "Player.h"
#include "../Engine/Components/RendererComponent.h"
#include "../Engine/Components/ColliderComponent.h"
#include "../Engine/Modules/ModuleTextures.h"
#include "../Engine/Modules/ModuleInput.h"
#include "../Engine/Modules/ModuleTime.h"

RTTI_REGISTER(Player)

const float angleToRotate = 1;
const float speed = 10;

bool Player::Start()
{
	bool ret = GameObject::Start();

	//Render component
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

	//Collider component
	ColliderComponent* collider = dynamic_cast<ColliderComponent*>(AddComponent("ColliderComponent"));
	collider->SetOriginalShape(new Shape<AABB>());
	static_cast<Shape<AABB>*>(collider->GetOriginalShape())->m_Shape = AABB(fVector(-20,-20),fVector(20,20));
	//collider->SetOriginalShape(new Shape<Circle>());
	//static_cast<Shape<Circle>*>(collider->GetOriginalShape())->m_Shape = Circle(fVector(0,0), 100);
	//collider->SetOriginalShape(new Shape<Line>());
	//static_cast<Shape<Line>*>(collider->GetOriginalShape())->m_Shape = Line(fVector(-20, -20), fVector(20, 20));
	collider->SetCollisionType(CollisionType::PLAYER);

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
		GetLocalTransform().translate(GetLocalTransform().Forward()*-speed);
	}
	else if(Input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		GetLocalTransform().translate(GetLocalTransform().Forward()*speed);
	}

	if(Input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		ColliderComponent* collider = GetComponent<ColliderComponent>();
		collider->SetIsTrigger(!collider->GetIsTrigger());
	}

	if (Input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) 
	{
		Instantiate("Obstacle",fVector(-50,-50));
	}

	return ret;
}

void Player::OnCollision(ColliderComponent & other)
{
	LOG("Collision");
}

void Player::OnTriggerEnter(ColliderComponent & other)
{
	LOG("Trigger Enter");
}

void Player::OnTriggerExit(ColliderComponent & other)
{
	LOG("Trigger Exit");
}

void Player::OnTriggerStay(ColliderComponent & other)
{
	LOG("Trigger Stay");
}
