#pragma once
#include "Source/Source.h"
#include "Source/Scene/Load/Load.h"
enum TitleState {
	State_MainMenu,        // スタート、ランキング、オプション
	State_DifficultySelect // 難易度選択
};
class TiterUI :public Source
{
	public:
		TiterUI();
	virtual ~TiterUI();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
private:
	TitleState m_state = State_MainMenu;
	SpriteRender m_start;
	SpriteRender m_option;
	SpriteRender m_Title;
	SpriteRender m_ranking;
	SpriteRender m_spriteRender;
	SpriteRender m_easySprite;
	SpriteRender m_normalSprite;
	SpriteRender m_hardSprite;
	SpriteRender m_spritekuro;
	SpriteRender m_nanido;
	int m_cursor = 0; // 0 = ゲーム, 1 = メニュー
	int m_selectedDifficulty = 0; // 難易度選択のカーソル用
	bool m_isDeciding = false;     // 決定演出中か
	float m_decideTimer = 0.0f;    // 決定演出タイマー
	float m_fadeAlpha = 0.0f;      // 黒フェードのアルファ値
};

