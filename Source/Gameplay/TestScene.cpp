#include "TestScene.h"

RTTI_REGISTER(TestScene)

bool TestScene::Start()
{
	bool ret = GameScene::Start();

	Instantiate("Player");
	Instantiate("Obstacle", fVector(50,50));

	return ret;
}
