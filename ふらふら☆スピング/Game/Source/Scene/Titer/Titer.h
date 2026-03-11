#pragma once
#include "Source/Source.h"
#include "Source/UI/UIManager.h"

class Titer : public Source
{
public:
	Titer() {}
	~Titer() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	//UIAnimationManager* m_uiManager;
	SpriteRender m_spriteRender;
	Transform* m_transform;

	
};

