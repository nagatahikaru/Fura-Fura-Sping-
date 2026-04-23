#pragma once
#include "Source/Source.h"
class RankingUI;
class Ranking : public Source
{
public:
	Ranking() : m_rankingUI(nullptr) {}
	~Ranking() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc); 
	void Load();
	void Save();
	void AddScore(int scoreKm,int scoreMeter);
private:
	// ★ スコア一覧（トップ10まで入る）
	std::vector<int> m_scoresScore;
	std::vector<int> m_scoresMeter;   // メートル部門（倍率前）
	// ★ ランキング表示用フォント（最大10行）
	FontRender m_fontsScore[5];
	FontRender m_fontsMeter[5];
	RankingUI* m_rankingUI;
	SpriteRender m_spriteRender;
	SpriteRender m_sukoa;
	SpriteRender m_mairu;
};
