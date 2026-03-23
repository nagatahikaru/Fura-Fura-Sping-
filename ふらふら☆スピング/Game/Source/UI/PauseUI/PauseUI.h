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
	SpriteRender m_startButton;
	SpriteRender m_titleMenu;
	SpriteRender m_soundRender;
	SpriteRender m_spritePause;
	int m_cursor = 0; // 0 = ƒQ[ƒ€, 1 = ƒƒjƒ…[
};
