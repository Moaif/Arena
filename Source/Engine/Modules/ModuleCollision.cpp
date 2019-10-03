#include "../../Globals.h"
#include "../Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleTime.h"
#include "../GameObject.h"

using namespace std;

static const float OUT_OF_BOUNDS_MOVE_SPEED = 1.0f;

ModuleCollision::ModuleCollision()
{
	m_hits[DEFAULT][DEFAULT] = true;
	m_hits[DEFAULT][PLAYER] = true;
	m_hits[DEFAULT][ENEMY_SHOOT] = true;
	m_hits[DEFAULT][PLAYER_SHOOT] = true;
	m_hits[DEFAULT][ENEMY] = true;

	m_hits[PLAYER][DEFAULT] = true;
	m_hits[PLAYER][PLAYER] = false;
	m_hits[PLAYER][ENEMY_SHOOT] = true;
	m_hits[PLAYER][PLAYER_SHOOT] = false;
	m_hits[PLAYER][ENEMY] = true;

	m_hits[ENEMY_SHOOT][DEFAULT] = true;
	m_hits[ENEMY_SHOOT][PLAYER] = true;
	m_hits[ENEMY_SHOOT][ENEMY_SHOOT] = false;
	m_hits[ENEMY_SHOOT][PLAYER_SHOOT] = false;
	m_hits[ENEMY_SHOOT][ENEMY] = false;

	m_hits[PLAYER_SHOOT][DEFAULT] = true;
	m_hits[PLAYER_SHOOT][PLAYER] = false;
	m_hits[PLAYER_SHOOT][ENEMY_SHOOT] = false;
	m_hits[PLAYER_SHOOT][PLAYER_SHOOT] = false;
	m_hits[PLAYER_SHOOT][ENEMY] = true;

	m_hits[ENEMY][DEFAULT] = true;
	m_hits[ENEMY][PLAYER] = true;
	m_hits[ENEMY][ENEMY_SHOOT] = false;
	m_hits[ENEMY][PLAYER_SHOOT] = true;
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
					if(!((*it)->IsReadyToDelete() && !(*it2)->IsReadyToDelete()))
					{//If one collider is already set to delete, we dont check again, it exist no more
						if((*it)->CheckCollision(*(*it2)))
						{
							if(m_hits[(*it)->GetCollisionType()][(*it2)->GetCollisionType()])
							{
								//If trigger collision
								if((*it)->GetIsTrigger() || (*it2)->GetIsTrigger())
								{
								pair<ColliderComponent*, ColliderComponent*> pair = std::pair<ColliderComponent*, ColliderComponent*>((*it), (*it2));
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
	GameObject* g1 = col1.GetGameObject();
	GameObject* g2 = col2.GetGameObject();

	while(col1.CheckCollision(col2))//While colliding, move out of bounds
	{
		Transform t1 = g1->GetWorldTransform();
		Transform t2 = g2->GetWorldTransform();
		fVector pos1 = t1.getPosition();
		fVector pos2 = t2.getPosition();
		fVector director = (pos2 - pos1).normalize();
		//Decide which object to move based on static colliders
		if(col1.GetIsStatic())
		{
			t2.setPosition(pos2 + director * OUT_OF_BOUNDS_MOVE_SPEED * Time->GetDeltaTime());
		}
		else if(col2.GetIsStatic())
		{
			t1.setPosition(pos1 - director * OUT_OF_BOUNDS_MOVE_SPEED * Time->GetDeltaTime());
		}
		else
		{
			t1.setPosition(pos1 - director * OUT_OF_BOUNDS_MOVE_SPEED/2 * Time->GetDeltaTime());
			t2.setPosition(pos2 + director * OUT_OF_BOUNDS_MOVE_SPEED/2 * Time->GetDeltaTime());
		}

		g1->SetWorldTransform(t1);
		g2->SetWorldTransform(t2);
	}
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