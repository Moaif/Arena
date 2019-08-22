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
	//Caution, objects not started nor active wont be destroyed if marked to do so
	for(list<unique_ptr<GameObject>>::iterator it = m_toStartGameObjects.begin(); it != m_toStartGameObjects.end() && loopRet; ++it)
	{
		if((*it)->IsActive())
		{
			loopRet &= (*it)->Start();
			m_gameObjects.push_back(move((*it)));
			it = m_toStartGameObjects.erase(it);
		}
	}

	if(!loopRet)
	{
		return UPDATE_ERROR;
	}

	for(vector<unique_ptr<GameObject>>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
	{
		if((*it)->IsReadyToDelete())
		{
			it = m_gameObjects.erase(it);
		}
	}

	update_status ret = UPDATE_CONTINUE;
	for(vector<unique_ptr<GameObject>>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end() && ret == UPDATE_CONTINUE; ++it)
	{
		if((*it)->IsActive())
		{
			ret = (*it)->PreUpdate();
		}
	}

	return ret;
}

update_status GameScene::Update()
{
	update_status ret = UPDATE_CONTINUE;
	for(vector<unique_ptr<GameObject>>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end() && ret == UPDATE_CONTINUE; ++it)
	{
		if((*it)->IsActive())
		{
			ret = (*it)->Update();
		}
	}

	return ret;
}

GameObject * GameScene::Instantiate(const string & className)
{
	RTTIInfo rtti = RTTIRepo::instance()->getByName(className);
	GameObject* gameObject = rtti.createInstance<GameObject>();
	m_toStartGameObjects.push_back(unique_ptr<GameObject>(gameObject));
	return m_toStartGameObjects.back().get();
}

GameObject * GameScene::Instantiate(const string & className, fVector position, float angle, GameObject * parent)
{
	GameObject* g = Instantiate(className);
	Transform transform = Transform().setIdentity();
	transform.setTranslation(position);
	transform.setRotation(angle);
	g->SetWorldTransform(transform);
	if(parent)
	{
		g->SetParent(*parent);
	}
}
