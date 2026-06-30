#pragma once
#include "Source/Source.h"
#include"Source/Scene/InGame/Game.h"
class PauseUI :public Source
{
public:
	PauseUI() {}
	~PauseUI(){}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void SetDifficulty(Difficulty difficulty) { m_difficulty = difficulty; }
private:
	SpriteRender m_spriteRender;
	SpriteRender m_spritePause;
	SpriteRender m_start;
	SpriteRender m_option;
	SpriteRender m_Title;
	SpriteRender m_yari;

	SpriteRender m_start2;
	SpriteRender m_option2;
	SpriteRender m_Title2;
	SpriteRender m_yari2;
	int m_cursor = 0; // 0 = ÉQÅ[ÉÄ, 1 = ÉÅÉjÉÖÅ[
	Difficulty m_difficulty = Difficulty::Easy;
};
