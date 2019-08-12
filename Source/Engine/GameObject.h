#ifndef _GAMEOBJECT_
#define _GAMEOBJECT_

struct Collider;

class GameObject {
public:
	virtual void OnCollision(Collider* other) {}

public:
	Collider* collider=nullptr;
};

#endif // !_GAMEOBJECT_

