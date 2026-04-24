#pragma once
#include "Source/Source.h"

class RankingUI :public Source
{
public:
	RankingUI() {}
	~RankingUI() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	SpriteRender m_1;
	SpriteRender m_2;
	SpriteRender m_3;
	SpriteRender m_4;
	SpriteRender m_5;
	SpriteRender m_11;
	SpriteRender m_22;
	SpriteRender m_33;
	SpriteRender m_44;
	SpriteRender m_55;
};
