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
	UIManager m_uiManager;
	SpriteRender m_spriteRender;

	
};

