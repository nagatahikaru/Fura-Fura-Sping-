#pragma once
#include "Source/Source.h"
enum ReturnType {
	ReturnToTitle,
	ReturnToPause
};
class SoundTestUI :public Source
{
public:
	SoundTestUI() {}
	~SoundTestUI() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	ReturnType m_returnType = ReturnToTitle;
private:
	SpriteRender m_spriteRender;
	SpriteRender m_spriteBGM;
	SpriteRender m_spriteSE;
	SpriteRender m_spriteBallBGM;
	SpriteRender m_spriteBallSE;
	SpriteRender m_spriteFontBGM;
	SpriteRender m_spriteFontSE;
	SpriteRender m_BGM;
	SpriteRender m_SE;
	SpriteRender m_B;
	SpriteRender m_grobu;
	// スライダー位置
	float m_bgmX = 0.0f;
	float m_seX = 0.0f;

	// スライダーの範囲
	const float m_minX = -400.0f;
	const float m_maxX = 400.0f;

	// 今どのスライダーを操作しているか
	int m_select = 0; // 0 = BGM, 1 = SE

	FontRender m_fontBGM;
	FontRender m_fontSE;
	
};

