#pragma once
#include "Source/Source.h"

class InGameUI:public Source
{
	public:
	InGameUI();
	virtual ~InGameUI();
	virtual void Update();
	virtual bool Start();
	virtual void Render(RenderContext& rc);
private:
	SpriteRender m_spriteRender;
};

