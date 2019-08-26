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
	hits[PLAYER][PLAYER] = false;
	hits[PLAYER][LASER] = false;
	hits[PLAYER][ENEMY_SHOOT] = true;
	hits[PLAYER][ENEMY] = true;
	hits[PLAYER][NO_DMG_ENEMY] = true;

	hits[LASER][PLAYER] = false;
	hits[LASER][LASER] = false;
	hits[LASER][ENEMY_SHOOT] = false;
	hits[LASER][ENEMY] = true;
	hits[LASER][NO_DMG_ENEMY] = true;

	hits[ENEMY_SHOOT][PLAYER] = true;
	hits[ENEMY_SHOOT][LASER] = false;
	hits[ENEMY_SHOOT][ENEMY_SHOOT] = false;
	hits[ENEMY_SHOOT][ENEMY] = false;
	hits[ENEMY_SHOOT][NO_DMG_ENEMY] = false;

	hits[ENEMY][PLAYER] = true;
	hits[ENEMY][LASER] = true;
	hits[ENEMY][ENEMY_SHOOT] = false;
	hits[ENEMY][ENEMY] = false;
	hits[ENEMY][NO_DMG_ENEMY] = false;

	hits[NO_DMG_ENEMY][PLAYER] = true;
	hits[NO_DMG_ENEMY][LASER] = true;
	hits[NO_DMG_ENEMY][ENEMY_SHOOT] = false;
	hits[NO_DMG_ENEMY][ENEMY] = false;
	hits[NO_DMG_ENEMY][NO_DMG_ENEMY] = false;

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

void ModuleCollision::DebugDraw()
{
	for (list<ColliderComponent*>::iterator it = colliders.begin(); it != colliders.end(); ++it)
		if ((*it)->IsActive()) {
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