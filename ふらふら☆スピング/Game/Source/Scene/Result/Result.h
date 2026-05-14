#pragma once
#include "Source/Source.h"

class Result : public Source
{
public:
	Result() {}
	~Result() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void SetResultValues(int guruguru, int km, int scores[3]);
private:
	SpriteRender m_spriteRender;
	int m_guruguru = 0;
	int m_km = 0;
	FontRender m_fontGuruguru;
	FontRender m_fontKm;
	SpriteRender m_grobu;
	SpriteRender m_B;
	float m_se2Volume = 1.0f;   // SE2 の現在音量（0〜1）
	bool m_isFadingSE2 = true;  // フェードアウト開始フラグ
	int m_displayGuruguru = 0;
	int m_displayKm = 0;
	float m_countSpeed = 1.5f;  // カウントアップ速度（調整可）
	float m_kmAddPerFrame = 0.0f;
	FontRender m_moto;
	int m_originalKm = 0;   // 元の km
	float m_displayOriginalKm = 0; // カウントアップ用
	float m_originalKmAddPerFrame = 0; // 1フレーム加算量
	int m_threeShots[3] = { 0,0,0 };
	FontRender m_fontThreeShots[3];
	FontRender m_fontThreeShotsValue[3];
	SpriteRender m_burakku;
	SpriteRender m_skip;
	SpriteRender m_rezarut;
	bool m_isSkipped = false;   // Aでスキップしたかどうか
	SpriteRender m_newRecord;
	bool m_isNewRecord = false;
	bool m_isScoreFixed = false;   // スコア確定＆ランキング保存済みか
	// ★ NEW!! 点滅用
	bool m_isBlinking = false;
	int m_blinkCount = 0;
	float m_blinkTimer = 0.0f;
	float m_se2Timer = 0.0f; // SE2 再生時間カウント用
	bool m_hasScore = false;
	double m_multiplier = 1.0f;
	FontRender m_fontMultiplier;
	bool m_step1 = true;   // 最大mのカウントアップ中
	bool m_step2 = false;  // ぐるぐる回数のカウントアップ中
	bool m_step3 = false;  // 最終スコアのカウントアップ中
	double m_displayMultiplier = 1.0; // 係数のカウントアップ用
};

