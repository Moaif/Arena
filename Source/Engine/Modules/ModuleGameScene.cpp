#include "ModuleGameScene.h"
#include "../RTTI.h"

using namespace std;

bool ModuleGameScene::Start()
{
	//TODO:: Add the first scene to the game to start it

	return true;
}

update_status ModuleGameScene::PreUpdate()
{
	if(m_popRequests > 0)
	{
		for(int i = 0; i < m_popRequests; ++i)
		{
			PopScene();
		}
		m_popRequests = 0;
	}

	if(m_nextScene)
	{
		m_sceneStack.push_back(move(m_nextScene));
		if(!m_sceneStack.back()->Start())
		{
			LOG("Failure in Starting the new requested scene");
			return UPDATE_ERROR;
		}
		m_nextScene = nullptr;
	}

	if(m_sceneStack.empty())
	{
		LOG("No scenes available in the ModuleGameScene stack PreUpdate");
		return UPDATE_ERROR;
	}

	return m_sceneStack.back()->PreUpdate();
}

update_status ModuleGameScene::Update()
{
	if(m_sceneStack.empty())
	{
		LOG("No scenes available in the ModuleGameScene stack Update");
		return UPDATE_ERROR;
	}

	return m_sceneStack.back()->Update();
}

GameScene * ModuleGameScene::AddScene(const std::string& className)
{
	RTTIInfo rtti = RTTIRepo::instance()->getByName(className);
	m_nextScene = unique_ptr<GameScene>(rtti.createInstance<GameScene>());
	return m_nextScene.get();
}

GameScene * ModuleGameScene::ReplaceSceneWithNew(const std::string& className)
{
	RemoveCurrentScene();
	return AddScene(className);
}

void ModuleGameScene::RemoveCurrentScene()
{
	if(m_popRequests < static_cast<int>(m_sceneStack.size()))
	{
		++m_popRequests;
	}
}

void ModuleGameScene::PopScene()
{
	if(!m_sceneStack.empty())
	{
		m_sceneStack.back()->CleanUp();
		m_sceneStack.pop_back();
	}
}
