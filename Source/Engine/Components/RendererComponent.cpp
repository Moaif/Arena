#include "RendererComponent.h"
#include "../Vector.h"
#include "../GameObject.h"

RTTI_REGISTER(RendererComponent)

update_status RendererComponent::Update()
{
	if(!m_currentAnimation)
	{
		LOG("RendererComponent has no animation assigned");//TODO set names to gameObject for debugging purposes
		return UPDATE_CONTINUE;
	}
	
	m_currentAnimation->Update();

	Transform transform = GetGameObject()->GetWorldTransform();
	Renderer->AddToBlitBuffer(m_currentAnimation->texture, transform, m_renderLayer, &(m_currentAnimation->GetCurrentFrame()));

	return UPDATE_CONTINUE;
}

void RendererComponent::AddAnimation(const std::string & animName, Animation & anim)
{
	m_animations[animName].push_back(anim);
	if(!m_currentAnimation)
	{
		m_currentAnimation = &m_animations[animName][0];
	}
}

bool RendererComponent::SetAnimation(const std::string & animName)
{
	int numAnimations = m_animations.count(animName);
	if(numAnimations > 0)
	{
		int randomAnimIndex = RAND() % numAnimations;
		m_currentAnimation = &m_animations[animName][randomAnimIndex];
		return true;
	}
	return false;
}
