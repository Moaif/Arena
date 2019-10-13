#include "GameObject.h"

using namespace std;

RTTI_REGISTER(GameObject);

GameObject::GameObject()
{
	m_localTransfrom = Transform().setIdentity();
}

update_status GameObject::PreUpdate()
{
	for(list<unique_ptr<Component>>::iterator it = m_toStartComponents.begin(); it != m_toStartComponents.end(); ++it)
	{
		if(!(*it)->Start())
		{
			return UPDATE_ERROR;
		}
		m_components.push_back(move((*it)));
		it = m_toStartComponents.erase(it);
	}

	for(vector<unique_ptr<Component>>::iterator it = m_components.begin(); it != m_components.end(); ++it)
	{
		if((*it)->IsReadyToDelete())
		{
			it = m_components.erase(it);
		}
	}

	return UPDATE_CONTINUE;
}

update_status GameObject::Update()
{
	update_status ret = UPDATE_CONTINUE;
	for(vector<unique_ptr<Component>>::iterator it = m_components.begin(); it != m_components.end() && ret == UPDATE_CONTINUE; ++it)
	{
		if((*it)->IsActive())
		{
			ret = (*it)->Update();
		}
	}

	return ret;
}

bool GameObject::CleanUp()
{
	bool ret = true;

	for(list<unique_ptr<Component>>::iterator it = m_toStartComponents.begin(); it != m_toStartComponents.end() && ret; ++it)
	{
		ret = (*it)->CleanUp();
	}

	for(vector<unique_ptr<Component>>::iterator it = m_components.begin(); it != m_components.end() && ret; ++it)
	{
		ret = (*it)->CleanUp();
	}

	return ret;
}

void GameObject::SetParent(GameObject & newParent)
{
	m_localTransfrom = newParent.GetWorldTransform().getInverse() * GetWorldTransform();
	m_parent->RemoveChild(*this);
	m_parent = &newParent;
	m_parent->AddChild(*this);
}

void GameObject::SetToDelete(bool value)
{
	if(value)
	{
		for (list<unique_ptr<Component>>::iterator it = m_toStartComponents.begin(); it != m_toStartComponents.end(); ++it)
		{
			(*it)->SetToDelete(true);
		}
		for(vector<unique_ptr<Component>>::iterator it = m_components.begin(); it != m_components.end(); ++it)
		{
			(*it)->SetToDelete(true);
		}
	}
}

const Transform & GameObject::GetWorldTransform()
{
	if(!m_parent)
	{
		m_worldTransform = m_localTransfrom;
	}
	else
	{
		//Recursive call that gets to the root parent
		m_worldTransform = m_localTransfrom * m_parent->GetWorldTransform();
	}
	return m_worldTransform;
}

void GameObject::SetWorldTransform(const Transform & transform)
{
	if(m_parent)
	{
		m_localTransfrom = transform * m_parent->GetWorldTransform().getInverse();
	}
	else
	{
		m_localTransfrom = transform;
	}
}
