#include "Invader.h"
#include "..//Engine/Application.h"
#include "../Engine/Components/ColliderComponent.h"
#include "../Engine/Modules/ModuleTime.h"
#include "Game.h"

RTTI_REGISTER(Invader)

const float HorizontalSpeed = 250.0f;
const float VerticalSpeed = 2000.0f;
const float BombDropChance = 0.10f;
const float minShootCD = 1.0f;
const float maxShootCD = 2.0f;

bool Invader::Start() {
	bool ret = GameObject::Start();

	GetLocalTransform().setScale(fVector(INVADERS_SCALE, INVADERS_SCALE));
	m_tryToShootTimer = ((getRand() % static_cast<int>(((maxShootCD - minShootCD + 1) * 100.0f)))) / 100 + minShootCD;//Multiply by 100 to add more range, as % only works with integers, and then divide by it to make it the same scale than before

	return ret;
}

update_status Invader::Update()
{
	update_status ret = GameObject::Update();

	m_tryToShootTimer -= Time->GetDeltaTime();

	return ret;
}

void Invader::Right()
{
	GetLocalTransform().translate(GetLocalTransform().Right() * HorizontalSpeed * Time->GetDeltaTime());
	if (GetWorldTransform().getPosition().x >= SCREEN_WIDTH / 2) {
		Game* castedScene = dynamic_cast<Game*>(GetGameScene());
		ASSERT(castedScene, "error can't cast currentGameScene to \"Game\" scene");
		castedScene->ChangeInvaderDirection(Direction::LEFT);
	}
}

void Invader::Left()
{
	GetLocalTransform().translate(GetLocalTransform().Right() * -HorizontalSpeed * Time->GetDeltaTime());
	if (GetWorldTransform().getPosition().x <= -SCREEN_WIDTH / 2) {
		Game* castedScene = dynamic_cast<Game*>(GetGameScene());
		ASSERT(castedScene, "error can't cast currentGameScene to \"Game\" scene");
		castedScene->ChangeInvaderDirection(Direction::RIGHT);
	}

}

void Invader::Down()
{
	GetLocalTransform().translate(GetLocalTransform().Forward() * -VerticalSpeed * Time->GetDeltaTime());
	if (GetWorldTransform().getPosition().y < -SCREEN_HEIGHT / 2) {
		Game* castedScene = dynamic_cast<Game*>(GetGameScene());
		ASSERT(castedScene, "error can't cast currentGameScene to \"Game\" scene");
		castedScene->EndLevel();
	}
}

void Invader::TryDropBomb()
{
	if(m_tryToShootTimer <= 0){
		int value = getRand() % 100;
		if (value / 100.0f < BombDropChance) {
			Instantiate("EnemyShoot", GetWorldTransform().getPosition());			
		}
		m_tryToShootTimer = ((getRand()%static_cast<int>(((maxShootCD-minShootCD + 1) *100.0f))))/100 + minShootCD;//Multiply by 100 to add more range, as % only works with integers, and then divide by it to make it the same scale than before
	}
}

void Invader::OnTriggerEnter(ColliderComponent& other)
{
	Destroy();
	Game* castedScene = dynamic_cast<Game*>(GetGameScene());
	ASSERT(castedScene, "error can't cast currentGameScene to \"Game\" scene");
	castedScene->InvaderDestroyed(*this);
}
