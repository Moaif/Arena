#include "Scene.h"
#include "..//Engine/GameObject.h"

RTTI_REGISTER(Scene)

bool Scene::Start() {
	bool ret = GameScene::Start();

	GameObject* g = Instantiate("DefaultObject","DeadStarCOOLname");
	LOG(g->GetName().c_str());
	LOG(g->GetGameScene()->GetName().c_str());

	return ret;
}