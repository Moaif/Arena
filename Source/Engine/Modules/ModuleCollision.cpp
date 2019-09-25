#include "../../Globals.h"
#include "../Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleTime.h"
#include "../GameObject.h"

using namespace std;

ModuleCollision::ModuleCollision()
{
	m_hits[DEFAULT][DEFAULT] = true;
	m_hits[DEFAULT][PLAYER] = true;
	m_hits[DEFAULT][OBSTACLE] = true;
	m_hits[DEFAULT][SHOOT] = true;
	m_hits[DEFAULT][ENEMY] = true;

	m_hits[PLAYER][DEFAULT] = true;
	m_hits[PLAYER][PLAYER] = false;
	m_hits[PLAYER][OBSTACLE] = true;
	m_hits[PLAYER][SHOOT] = true;
	m_hits[PLAYER][ENEMY] = true;

	m_hits[OBSTACLE][DEFAULT] = true;
	m_hits[OBSTACLE][PLAYER] = true;
	m_hits[OBSTACLE][OBSTACLE] = true;
	m_hits[OBSTACLE][SHOOT] = true;
	m_hits[OBSTACLE][ENEMY] = true;

	m_hits[SHOOT][DEFAULT] = true;
	m_hits[SHOOT][PLAYER] = true;
	m_hits[SHOOT][OBSTACLE] = true;
	m_hits[SHOOT][SHOOT] = false;
	m_hits[SHOOT][ENEMY] = true;

	m_hits[ENEMY][DEFAULT] = true;
	m_hits[ENEMY][PLAYER] = true;
	m_hits[ENEMY][OBSTACLE] = true;
	m_hits[ENEMY][SHOOT] = true;
	m_hits[ENEMY][ENEMY] = false;
}

// Destructor
ModuleCollision::~ModuleCollision()
{
	m_colliders.clear();
	m_collidingTriggers.clear();
}

update_status ModuleCollision::PreUpdate()
{
	//Check for OnTriggerExit
	for(std::pair<ColliderComponent*, ColliderComponent*> p : m_collidingTriggers)
	{
		if(p.first->IsActive() && p.second->IsActive())
		{
			if(!(p.first->IsReadyToDelete() || p.second->IsReadyToDelete()))
			{
				if(p.first->CheckCollision(*p.second))
				{
					if(m_hits[p.first->GetCollisionType()][p.second->GetCollisionType()])
					{
						if(p.first->GetIsTrigger() || p.second->GetIsTrigger())
						{
							continue;//Still triggering
						}
					}
				}
			}
		}
		//If triggering anymore
		p.first->GetGameObject()->OnTriggerExit(*p.second);
		p.second->GetGameObject()->OnTriggerExit(*p.first);
		m_collidingTriggers.erase(p);
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleCollision::Update()
{
	for(list<ColliderComponent*>::iterator it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		if((*it)->IsActive())
		{
			for(list<ColliderComponent*>::iterator it2 = it; it2 != m_colliders.end(); ++it2)
			{
				if(it == it2)//Dont check collider with himself
					continue;

				if((*it2)->IsActive() && (*it)->IsActive())
				{//Needed to check again, in case that it changes inside the for loop
					if(!((*it)->IsReadyToDelete() || (*it2)->IsReadyToDelete()))
					{//If one collider is already set to delete, we dont check again, it exist no more
						if((*it)->CheckCollision(*(*it2)))
						{
							if(m_hits[(*it)->GetCollisionType()][(*it2)->GetCollisionType()])
							{
								//If trigger collision
								if((*it)->GetIsTrigger() || (*it2)->GetIsTrigger())
								{
								std:pair<ColliderComponent*, ColliderComponent*> pair = std::pair<ColliderComponent*, ColliderComponent*>((*it), (*it2));
									if(!m_collidingTriggers.count(pair))//If not already colliding
									{
										(*it)->GetGameObject()->OnTriggerEnter(*(*it2));
										(*it2)->GetGameObject()->OnTriggerEnter(*(*it));
										//Set colliders into colliding list to make update/exit trigger flow
										m_collidingTriggers.insert(pair);
									}
									else
									{
										(*it)->GetGameObject()->OnTriggerStay(*(*it2));
										(*it2)->GetGameObject()->OnTriggerStay(*(*it));
									}
								}
								else//Basic collision
								{
									(*it)->GetGameObject()->OnCollision(*(*it2));
									(*it2)->GetGameObject()->OnCollision(*(*it));
									MoveOutOfBounds(*(*it), *(*it2));
								}
							}
						}
					}
				}
			}
		}
	}

	if(Input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		m_debug = !m_debug;

	if(m_debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()const
{
	for (list<ColliderComponent*>::const_iterator it = m_colliders.cbegin(); it != m_colliders.cend(); ++it)
		if ((*it)->IsActive()) {
			(*it)->DebugDraw();
		}
}

void ModuleCollision::MoveOutOfBounds(ColliderComponent & col1, ColliderComponent & col2)
{
	Transform temp = col2.GetGameObject()->GetWorldTransform();
	col2.GetGameObject()->SetWorldTransform(temp.setPosition(col1.PushCollider(col2)));
}

bool ModuleCollision::CleanUp()
{
	m_colliders.clear();

	return true;
}

void ModuleCollision::SubscribeCollider(ColliderComponent& collider)
{
	m_colliders.push_back(&collider);
}

void ModuleCollision::UnsubscribeCollider(ColliderComponent& collider)
{
	m_colliders.remove(&collider);
}