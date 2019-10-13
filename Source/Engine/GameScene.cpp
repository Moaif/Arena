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

	for(vector<unique_ptr<GameObject>>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end() && loopRet;)
	{
		if((*it)->IsReadyToDelete())
		{
			loopRet &= (*it)->CleanUp();
			it = m_gameObjects.erase(it);
		}
		else
		{
			++it;
		}
	}

	if (!loopRet)
	{
		return UPDATE_ERROR;
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

bool GameScene::CleanUp()
{
	bool ret = true;

	for (list<unique_ptr<GameObject>>::iterator it = m_toStartGameObjects.begin(); it != m_toStartGameObjects.end() && ret;)
	{
		ret &= (*it)->CleanUp();
		it = m_toStartGameObjects.erase(it);
	}

	for (vector<unique_ptr<GameObject>>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end() && ret;)
	{
		(*it)->CleanUp();
		it = m_gameObjects.erase(it);
	}

	return ret;
}

GameObject * GameScene::Instantiate(const string & className, const string& gameObjectName)
{
	RTTIInfo rtti = RTTIRepo::instance()->getByName(className);
	GameObject* gameObject = rtti.createInstance<GameObject>();
	m_toStartGameObjects.push_back(unique_ptr<GameObject>(gameObject));
	gameObject->SetGameScene(this);
	gameObject->SetName(gameObjectName.empty() ? className : gameObjectName);
	return m_toStartGameObjects.back().get();
}

GameObject * GameScene::Instantiate(const string & className, fVector position, float angle, GameObject * parent, const string& gameObjectName)
{
	GameObject* g = Instantiate(className,gameObjectName);
	Transform transform = Transform().setIdentity();
	transform.setPosition(position);
	transform.setRotation(angle);
	g->SetWorldTransform(transform);
	if(parent)
	{
		g->SetParent(*parent);
	}

	return g;
}

void GameScene::SetToDelete(bool value)
{
	if (value)
	{
		for (list<unique_ptr<GameObject>>::iterator it = m_toStartGameObjects.begin(); it != m_toStartGameObjects.end(); ++it)
		{
			(*it)->SetToDelete(true);
		}
		for (vector<unique_ptr<GameObject>>::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
		{
			(*it)->SetToDelete(true);
		}
	}
}
