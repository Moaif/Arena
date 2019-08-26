#include "Component.h"

RTTI_REGISTER(Component)

Component::Component()
{
	ASSERT(Init(),"Failed to Initialize component");
}