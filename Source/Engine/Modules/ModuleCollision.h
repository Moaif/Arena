#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#include<list>
#include "Module.h"
#include <SDL.h>

class GameObject;

enum CollisionType
{
	PLAYER,
	LASER,
	ENEMY_SHOOT,
	ENEMY,
	NO_DMG_ENEMY,
	MAXIMO
};

struct Collider
{
	SDL_Rect rect = { 0,0,0,0 };
	float z;
	float speed;
	CollisionType type;
	GameObject* callback;
	bool to_delete = false;
	bool active = true;


	Collider(SDL_Rect rectangle,float z,float speed,CollisionType type,GameObject* callback) : 
		rect(rectangle),z(z),speed(speed),type(type),callback(callback)
	{}

	bool CheckCollision(const SDL_Rect& r, const float& z, const float& speed) const;
};

class ModuleCollision : public Module
{
public:

	ModuleCollision();
	~ModuleCollision();

	update_status PreUpdate()override;
	update_status Update()override;

	bool CleanUp()override;

	Collider* AddCollider(const SDL_Rect& rect, const float& z, const float& speed, const CollisionType& type,GameObject* callback);
	void DebugDraw();

private:

	std::list<Collider*> colliders;
	bool debug = false;
	bool hits[MAXIMO][MAXIMO];
};

#endif // __ModuleCollision_H__