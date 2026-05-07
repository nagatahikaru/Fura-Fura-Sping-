#include "stdafx.h"
#include "Ranking.h"
#include"Source/UI/RankingUI/RankingUI.h"
#include"Source/Scene/Titer/Titer.h"
#include <fstream>
#include <algorithm>

bool Ranking::Start() {
    m_spriteRender.Init("Assets/sprite/Ranking.DDS", 1920.0f, 1080.0f);
    m_sukoa.Init("Assets/sprite/sukoa.DDS", 420.0f, 280.0f);
    m_mairu.Init("Assets/sprite/mairu.DDS", 420.0f, 280.0f);
    for (int i = 0; i < 5; i++) {
        m_fontsScore[i].SetPivot(0.0f, 0.5f); // 左寄せ
    }
    for (int i = 0; i < 5; i++) {
       m_fontsMeter[i].SetPivot(0.0f, 0.5f); // 左寄せ
    }
    // UI を追加
    m_rankingUI= NewGO<RankingUI>(0);
    Load();
    return true;
}

void Ranking::Load() {
    std::ifstream ifs("ranking.dat");
    if (!ifs) {
        m_scoresScore.fill(0);
        m_scoresMeter.fill(0);
        m_scoresGuruguru.fill(0);
        Save();
        return;
    }

    for (int i = 0; i < 5; i++) ifs >> m_scoresScore[i];
    for (int i = 0; i < 5; i++) ifs >> m_scoresMeter[i];
    for (int i = 0; i < 5; i++) ifs >> m_scoresGuruguru[i];

    Save();
}

void Ranking::Save() {
    std::ofstream ofs("ranking.dat");
    // スコア部門
    for (int s : m_scoresScore) ofs << s << "\n";

    // メートル部門
    for (int s : m_scoresMeter) ofs << s << "\n";

    // ぐるぐる部門
    for (int s : m_scoresGuruguru) ofs << s << "\n";
}

void Ranking::Update() { 
    // ▼ Bボタンでタイトルへ戻る
    if (g_pad[0]->IsTrigger(enButtonB)) {
        // ★ RankingUI も削除
        if (m_rankingUI) {
            DeleteGO(m_rankingUI);
            m_rankingUI = nullptr;
        }
        NewGO<Titer>(0);   // タイトルへ
        DeleteGO(this);    // ランキングUI削除
    }
}

bool Ranking::AddScore(int scoreKm, int scoreMeter, int guruguru) {
    int oldBest = m_scoresScore[0];

    // ★ スコア + ぐるぐる を 6件にしてソート
    std::array<std::pair<int, int>, 6> pairs;

    for (int i = 0; i < 5; i++) {
        pairs[i] = { m_scoresScore[i], m_scoresGuruguru[i] };
    }
    pairs[5] = { scoreKm, guruguru };

    std::sort(pairs.begin(), pairs.end(),
        [](auto& a, auto& b) { return a.first > b.first; });

    for (int i = 0; i < 5; i++) {
        m_scoresScore[i] = pairs[i].first;
        m_scoresGuruguru[i] = pairs[i].second;
    }

    int newBest = m_scoresScore[0];
    bool isNewRecord = (scoreKm == newBest && newBest > oldBest);

    // ★ メートル部門
    std::array<int, 6> meterTemp;
    for (int i = 0; i < 5; i++) meterTemp[i] = m_scoresMeter[i];
    meterTemp[5] = scoreMeter;

    std::sort(meterTemp.begin(), meterTemp.end(), std::greater<int>());

    for (int i = 0; i < 5; i++) {
        m_scoresMeter[i] = meterTemp[i];
    }

    Save();
    return isNewRecord;
}

void Ranking::Render(RenderContext& rc) {
    m_spriteRender.Draw(rc);
    m_sukoa.SetPosition({ -360, 225, 0 });
    m_sukoa.Update();
    m_sukoa.Draw(rc);
    m_mairu.SetPosition({ 340, 225, 0 });
    m_mairu.Update();
    m_mairu.Draw(rc);
    wchar_t buf[256];
    int count = (int)m_scoresScore.size();
    if (count > 5) count = 5;
    // スコア部門
    for (int i = 0; i < 5; i++) {
        swprintf_s(buf, L"位:%.2f", m_scoresScore[i] / 100.0); // スコアのみ表示
        m_fontsScore[i].SetText(buf);
        m_fontsScore[i].SetPosition(-500, 165 - i * 90, 0);
        m_fontsScore[i].SetColor(1, 1, 1, 1);  // ← 白
        m_fontsScore[i].SetScale(1.3f);
        m_fontsScore[i].Draw(rc);
        // ★ このスコアが何回ぐるぐるしたか
        int c = m_scoresGuruguru[i];
        float r2, g2, b2;

        // 0〜9：黒
        if (c < 10) {
            r2 = 1.0f; g2 = 1.0f; b2 = 1.0f;
        }
        // 10〜14：青
        else if (c < 15) {
            r2 = 0.0f; g2 = 0.7f; b2 = 1.0f;
        }
        // 15〜19：黄緑
        else if (c < 20) {
            r2 = 0.0f; g2 = 1.0f; b2 = 0.0f;
        }
        // 20〜24：オレンジ
        else if (c < 25) {
            r2 = 1.0f; g2 = 0.5f; b2 = 0.0f;
        }
        // 25以上：赤
        else {
            r2 = 1.0f; g2 = 0.0f; b2 = 0.0f;
        }
        // ★ このスコアが何回ぐるぐるしたか
        wchar_t bufG[64];
        swprintf_s(bufG, L"(%d回)", m_scoresGuruguru[i]);
        m_fontsGuruguru[i].SetText(bufG);
        m_fontsGuruguru[i].SetPosition(-220, 165 - i * 90, 0);
        m_fontsGuruguru[i].SetScale(1.2f);
        m_fontsGuruguru[i].SetColor(r2, g2, b2, 1.0f);
        m_fontsGuruguru[i].Draw(rc);
    }

    // メートル部門
    for (int i = 0; i < 5; i++) {
        swprintf_s(buf, L"位:%.2f m", m_scoresMeter[i] / 100.0);
        m_fontsMeter[i].SetText(buf);
        m_fontsMeter[i].SetPosition(200, 165 - i * 90, 0);
        m_fontsMeter[i].SetColor(1, 1, 1, 1);  // ← 白
        m_fontsMeter[i].SetScale(1.3f);
        m_fontsMeter[i].Draw(rc);
    }

}
