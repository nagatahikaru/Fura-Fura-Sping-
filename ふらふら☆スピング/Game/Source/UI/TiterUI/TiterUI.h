#pragma once
#include "Source/Source.h"

class TiterUI :public Source
{
	public:
		TiterUI();
	virtual ~TiterUI();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
private:
	SpriteRender m_startButton;
	SpriteRender m_titleMenu;
	SpriteRender m_start;
	SpriteRender m_option;
	int m_cursor = 0; // 0 = ƒQ[ƒ€, 1 = ƒƒjƒ…[
};

