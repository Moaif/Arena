#include "TestScene.h"

RTTI_REGISTER(TestScene)

bool TestScene::Start()
{
	bool ret = GameScene::Start();

	Instantiate("Player");

	return ret;
}
