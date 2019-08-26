#pragma once
#include "Component.h"
#include <vector>
#include "../Animation.h"
#include "../Modules/ModuleRender.h"

class RendererComponent : public Component
{
	RTTI_ENABLE(RendererComponent,Component)
public:

	virtual update_status Update() override;

	void AddAnimation(const std::string& animName, Animation& anim);
	bool SetAnimation(const std::string& animName);
	void SetLayer(const Layer& layer){ m_renderLayer = layer;};
	const Layer& GetLayer() const {return m_renderLayer;};
private:
	std::map<std::string, std::vector<Animation>> m_animations;
	Animation* m_currentAnimation = nullptr;
	Layer m_renderLayer = Layer::Game;
};