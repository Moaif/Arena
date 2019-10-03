#include "Game.h"
#include "..//Engine/Application.h"
#include "../Engine/Modules/ModuleGameScene.h"

RTTI_REGISTER(Game)

const float PlayerYPosOffset = 128.0f;

const int columns = 9;
const int redRows = 2;
const int yellowRows = 2;
const int pinkRows = 1;

const float invadersSize = 170*INVADERS_SCALE;// more size so they are not that close one from another
const float bossSize = 256* INVADERS_SCALE;

bool Game::Start() 
{
	bool ret = GameScene::Start();
	Instantiate("Background");
	Instantiate("Player", fVector(0,-SCREEN_HEIGHT/2 + PlayerYPosOffset));
	NewWave();

	m_currentDirection = Direction::RIGHT;

	return ret;
}

update_status Game::Update()
{
	update_status ret = GameScene::Update();
	
	MoveWaveAndShoot();

	return ret;
}

void Game::InvaderDestroyed(const Invader& destroyed)
{
	std::vector<Invader*>::iterator it = std::find(m_wave.begin(), m_wave.end(), &destroyed);
	if (it != m_wave.end()) {
		m_wave.erase(it);
	}
	
	if (m_wave.size() == 0){
		NewWave();
	}
}

void Game::ChangeInvaderDirection(Direction dir)
{
	if (m_currentDirection != dir) {
		m_currentDirection = dir;
		MoveWaveDown();
	}
}

void Game::EndLevel()
{
	Scene->ReplaceSceneWithNew("Menu");
}

void Game::NewWave()
{
	int totalRows = redRows + yellowRows + pinkRows;
	fVector minWavePos = fVector(-columns/2*invadersSize,(SCREEN_HEIGHT/2) - (totalRows * invadersSize + bossSize));

	for (int column = 0; column < columns; ++column) {

		for (int redRow = 0; redRow < redRows; ++redRow) {
			m_wave.push_back(dynamic_cast<Invader*>(Instantiate("InvaderRed", minWavePos + fVector(column * invadersSize, redRow * invadersSize))));
		}

		for (int yellowRow = 0; yellowRow < yellowRows; ++yellowRow) {
			m_wave.push_back(dynamic_cast<Invader*>(Instantiate("InvaderYellow", minWavePos + fVector(column * invadersSize, (redRows + yellowRow) * invadersSize))));
		}

		for (int pinkRow = 0; pinkRow < pinkRows; ++pinkRow) {
			m_wave.push_back(dynamic_cast<Invader*>(Instantiate("InvaderPink", minWavePos + fVector(column * invadersSize, (redRows + yellowRows+pinkRow) * invadersSize))));
		}
	}

	m_wave.push_back(dynamic_cast<Invader*>(Instantiate("InvaderBoss", fVector(0, minWavePos.y + (totalRows) * invadersSize))));
}

void Game::MoveWaveAndShoot()
{
	for each (Invader * i in m_wave)
	{
		switch (m_currentDirection)
		{
		case Direction::RIGHT:
			i->Right();
			break;
		case Direction::LEFT:
			i->Left();
			break;
		}
		i->TryDropBomb();
	}
}

void Game::MoveWaveDown()
{
	for each (Invader * i in m_wave)
	{
		i->Down();
	}
}
