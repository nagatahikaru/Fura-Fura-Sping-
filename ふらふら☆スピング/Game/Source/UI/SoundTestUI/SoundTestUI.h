#pragma once
#include "Source/Source.h"
class SoundTestUI :public Source
{
public:
	SoundTestUI() {}
	~SoundTestUI() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	SpriteRender m_spriteRender;
};

