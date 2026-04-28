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
	SpriteRender m_spritekuro;
	SpriteRender m_spriteRender;
	int m_cursor = 0; // 0 = ゲーム, 1 = メニュー
	bool m_isDeciding = false;     // 決定アニメ中
	float m_decideTimer = 0.0f;    // 点滅用
	float m_fadeAlpha = 0.0f;      // フェードアウト用
};

