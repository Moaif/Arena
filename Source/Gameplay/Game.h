#pragma once
#include "../Engine/GameScene.h"
#include "Invader.h"

enum class Direction {
	RIGHT,
	LEFT
};

class Game : public GameScene
{
	RTTI_ENABLE(Game, GameScene)

public:

	virtual bool Start() override;
	virtual update_status Update() override;

	void InvaderDestroyed(const Invader& destroyed);
	void ChangeInvaderDirection(Direction dir);
	void EndLevel();

private:
	void NewWave();
	void MoveWaveAndShoot();
	void MoveWaveDown();

private:
	std::vector<Invader*> m_wave;
	Direction m_currentDirection = Direction::RIGHT;
};