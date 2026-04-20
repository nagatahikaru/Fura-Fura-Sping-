#pragma once
#include "Source/Source.h"

class PauseUI :public Source
{
public:
	PauseUI() {}
	~PauseUI(){}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	SpriteRender m_spriteRender;
	SpriteRender m_spritePause;
	SpriteRender m_start;
	SpriteRender m_option;
	SpriteRender m_Title;
	SpriteRender m_yari;
	int m_cursor = 0; // 0 = ƒQ[ƒ€, 1 = ƒƒjƒ…[
};
