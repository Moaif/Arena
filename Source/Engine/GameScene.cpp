#include "GameScene.h"
#include "GameObject.h"

using namespace std;

RTTI_REGISTER(GameScene);

GameScene::GameScene()
{}

GameScene::~GameScene()
{}

update_status GameScene::PreUpdate()
{
	bool loopRet = true;
	for(std::vector<std::unique_ptr<GameObject>>::iterator it = toStartGameObjects.begin(); it != toStartGameObjects.end() && loopRet; ++it)
	{
		loopRet &= (*it)->Start();
	}

	if(!loopRet)
	{
		return UPDATE_ERROR;
	}

	for(std::vector<std::unique_ptr<GameObject>>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it)
	{
		if((*it)->IsReadyToDelete())
		{
			it = gameObjects.erase(it);
		}
	}

	return UPDATE_CONTINUE;
}

update_status GameScene::Update()
{
	update_status ret = UPDATE_CONTINUE;
	for(std::vector<std::unique_ptr<GameObject>>::iterator it = gameObjects.begin(); it != gameObjects.end() && ret == UPDATE_CONTINUE; ++it)
	{
		ret = (*it)->Update();
	}

	return ret;
}

GameObject * GameScene::Instantiate(const std::string & ClassName)
{
	RTTIInfo rtti = RTTIRepo::instance()->getByName(ClassName);
	GameObject* g = rtti.createInstance<GameObject>();
	gameObjects.push_back(make_unique<GameObject>(g));
	return gameObjects[gameObjects.size()-1].get();
}

GameObject * GameScene::Instantiate(const std::string & ClassName, fVector position, float angle, GameObject * parent)
{
	GameObject* g = Instantiate(ClassName);
	Transform transform = Transform().setIdentity();
	transform.setTranslation(position);
	transform.setRotation(angle);
	g->SetWorldTransform(transform);
	if(parent)
	{
		g->SetParent(*parent);
	}
}
