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
	bool AddScore(int scoreKm,int scoreMeter, int guruguru);
private:
	// ★ スコア一覧（トップ10まで入る）
	std::array<int, 5> m_scoresScore{};
	std::array<int, 5> m_scoresMeter{};
	std::array<int, 5> m_scoresGuruguru{};
	FontRender m_fontsScore[5];
	FontRender m_fontsMeter[5];
	FontRender m_fontsGuruguru[5];
	RankingUI* m_rankingUI;
	SpriteRender m_spriteRender;
	SpriteRender m_sukoa;
	SpriteRender m_mairu;
};
