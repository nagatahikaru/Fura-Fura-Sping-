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
	SpriteRender m_start;
	SpriteRender m_option;
	SpriteRender m_Title;
	SpriteRender m_ranking;
	SpriteRender m_gurobu;
	int m_cursor = 0; // 0 = ƒQ[ƒ€, 1 = ƒƒjƒ…[
};

