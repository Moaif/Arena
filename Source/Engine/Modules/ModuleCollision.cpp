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
	hits[DEFAULT][DEFAULT] = true;
	hits[DEFAULT][PLAYER] = true;
	hits[DEFAULT][OBSTACLE] = true;
	hits[DEFAULT][SHOOT] = true;
	hits[DEFAULT][ENEMY] = true;

	hits[PLAYER][DEFAULT] = true;
	hits[PLAYER][PLAYER] = false;
	hits[PLAYER][OBSTACLE] = true;
	hits[PLAYER][SHOOT] = true;
	hits[PLAYER][ENEMY] = true;

	hits[OBSTACLE][DEFAULT] = true;
	hits[OBSTACLE][PLAYER] = true;
	hits[OBSTACLE][OBSTACLE] = true;
	hits[OBSTACLE][SHOOT] = true;
	hits[OBSTACLE][ENEMY] = true;

	hits[SHOOT][DEFAULT] = true;
	hits[SHOOT][PLAYER] = true;
	hits[SHOOT][OBSTACLE] = true;
	hits[SHOOT][SHOOT] = false;
	hits[SHOOT][ENEMY] = true;

	hits[ENEMY][DEFAULT] = true;
	hits[ENEMY][PLAYER] = true;
	hits[ENEMY][OBSTACLE] = true;
	hits[ENEMY][SHOOT] = true;
	hits[ENEMY][ENEMY] = false;
}

// Destructor
ModuleCollision::~ModuleCollision()
{}

update_status ModuleCollision::Update()
{
	for (list<ColliderComponent*>::iterator it = colliders.begin(); it != colliders.end(); ++it) {
		if((*it)->IsActive()){
			for (list<ColliderComponent*>::iterator it2 = it; it2 != colliders.end(); ++it2) {
				if ((*it2)->IsActive() && (*it)->IsActive()) {//Needed to check again, in case that it changes inside the for loop
					if (!((*it)->IsReadyToDelete() || (*it2)->IsReadyToDelete())) {//If one collider is already set to delete, we dont check again, it exist no more
						if ((*it)->CheckCollision(*(*it2))) {
							if (hits[(*it)->GetCollisionType()][(*it2)->GetCollisionType()]) {
								(*it)->GetGameObject()->OnCollision(*(*it2));
								(*it2)->GetGameObject()->OnCollision(*(*it));
							}
						}
					}
				}
			}
		}
	}

	if(Input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(debug == true)
		DebugDraw();

	return UPDATE_CONTINUE;
}

void ModuleCollision::DebugDraw()const
{
	LOG("Before Loop");
	for (list<ColliderComponent*>::const_iterator it = colliders.cbegin(); it != colliders.cend(); ++it)
		if ((*it)->IsActive()) {
			LOG("Loop");
			(*it)->DebugDraw();
		}
}

bool ModuleCollision::CleanUp()
{
	colliders.clear();

	return true;
}

void ModuleCollision::SubscribeCollider(ColliderComponent& collider)
{
	colliders.push_back(&collider);
}

void ModuleCollision::UnsubscribeCollider(ColliderComponent& collider)
{
	colliders.remove(&collider);
}