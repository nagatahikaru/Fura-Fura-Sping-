#pragma once
#include "Source/Source.h"

class Start1 : public Source
{
public:
	Start1() {} // コンストラクタは戻り値型を指定しない
	~Start1() {}
	bool Start(); // これは通常のメンバ関数
	void Update();
	void Render(RenderContext& rc);
private:
	SpriteRender m_1;
	SpriteRender m_2;
	SpriteRender m_3;
	SpriteRender m_Start;
	float m_timer = 0.0f;   // ★ カウントダウン用タイマー
	bool m_playedStartSE = false;
};

