#include "GameObject.h"
#include "Components/Component.h"

using namespace std;

RTTI_REGISTER(GameObject);

GameObject::GameObject(GameObject * parent)
{
	m_localTransfrom.setIdentity();
	if(parent)
	{
		SetParent(*parent);
	}
}

GameObject::GameObject(Transform t, GameObject * parent)
{
	m_localTransfrom.setIdentity();
	if(parent)
	{
		SetParent(*parent);
	}
	SetWorldTransform(t);
}

void GameObject::AddComponent(Component * component)
{
	//TODO
}

void GameObject::SetParent(GameObject & newParent)
{
	m_localTransfrom = newParent.GetWorldTransform().getInverse() * GetWorldTransform();
	parent = &newParent;
}

bool GameObject::SetToDelete(bool value)
{
	if(value)
	{
		for(vector<Component*>::iterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->SetToDelete(true);
		}
	}
	return m_toDelete=value;
}

const Transform & GameObject::GetWorldTransform()
{
	if(!parent)
	{
		m_worldTransform = m_localTransfrom;
	}
	else
	{
		//Recursive call that gets to the root parent
		m_worldTransform = m_localTransfrom * parent->GetWorldTransform();
	}
	return m_worldTransform;
}

void GameObject::SetWorldTransform(const Transform & transform)
{
	m_localTransfrom = transform * parent->GetWorldTransform().getInverse();
}
