#include "stdafx.h"
#include "Ranking.h"
#include"Source/UI/RankingUI/RankingUI.h"
#include"Source/Scene/Titer/Titer.h"
#include <fstream>
#include <algorithm>

bool Ranking::Start() {
    m_spriteRender.Init("Assets/sprite/Ranking.DDS", 1920.0f, 1080.0f);
    m_sukoa.Init("Assets/sprite/sukoa.DDS", 420.0f, 300.0f);
    m_mairu.Init("Assets/sprite/mairu.DDS", 420.0f, 300.0f);
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
    m_scoresScore.clear();
    m_scoresMeter.clear();
    m_scoresGuruguru.clear();

    std::ifstream ifs("ranking.dat");
    if (!ifs) {
        // ファイルが無い → 全部0で初期化
        m_scoresScore.assign(5, 0);
        m_scoresMeter.assign(5, 0);
        m_scoresGuruguru.assign(5, 0);
        Save();
        return;
    }

    int s;

    // スコア部門
    for (int i = 0; i < 5; i++) {
        if (ifs >> s) m_scoresScore.push_back(s);
        else m_scoresScore.push_back(0);
    }

    // メートル部門
    for (int i = 0; i < 5; i++) {
        if (ifs >> s) m_scoresMeter.push_back(s);
        else m_scoresMeter.push_back(0);
    }

    // ぐるぐる部門
    for (int i = 0; i < 5; i++) {
        if (ifs >> s) m_scoresGuruguru.push_back(s);
        else m_scoresGuruguru.push_back(0);
    }

    // ★★★ ここが重要！必ず 5 個に揃える（壊れたファイル対策）★★★
    while (m_scoresScore.size() < 5) m_scoresScore.push_back(0);
    while (m_scoresMeter.size() < 5) m_scoresMeter.push_back(0);
    while (m_scoresGuruguru.size() < 5) m_scoresGuruguru.push_back(0);

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
    // ★ 更新前の1位
    int oldBest = m_scoresScore.empty() ? -1 : m_scoresScore[0];

    // スコア部門
    // ★ まず全ての配列を5個に揃える（不足分は0で埋める）
    while (m_scoresScore.size() < 5) m_scoresScore.push_back(0);
    while (m_scoresMeter.size() < 5) m_scoresMeter.push_back(0);
    while (m_scoresGuruguru.size() < 5) m_scoresGuruguru.push_back(0);

    // ★ 新しいスコアを追加
    m_scoresScore.push_back(scoreKm);
    m_scoresGuruguru.push_back(guruguru);

    // ★ スコアとぐるぐるをセットでソート
    std::vector<std::pair<int, int>> scorePairs;
    for (int i = 0; i < m_scoresScore.size(); i++) {
        scorePairs.push_back({ m_scoresScore[i], m_scoresGuruguru[i] });
    }

    std::sort(scorePairs.begin(), scorePairs.end(),
        [](auto& a, auto& b) { return a.first > b.first; });

    // 上位5件に戻す
    m_scoresScore.clear();
    m_scoresGuruguru.clear();
    for (int i = 0; i < 5; i++) {
        m_scoresScore.push_back(scorePairs[i].first);
        m_scoresGuruguru.push_back(scorePairs[i].second);
    }

    // ★ 更新後の1位
    int newBest = m_scoresScore[0];

    // ★ 1位更新したか？
    bool isNewRecord = (scoreKm == newBest && newBest > oldBest);

    // メートル部門
    m_scoresMeter.push_back(scoreMeter);
    std::sort(m_scoresMeter.begin(), m_scoresMeter.end(), std::greater<int>());
    if (m_scoresMeter.size() > 5) m_scoresMeter.resize(5);

    Save();

    return isNewRecord;     
}

void Ranking::Render(RenderContext& rc) {
    m_spriteRender.Draw(rc);
    m_sukoa.SetPosition({ -350, 160, 0 });
    m_sukoa.Update();
    m_sukoa.Draw(rc);
    m_mairu.SetPosition({ 350, 160, 0 });
    m_mairu.Update();
    m_mairu.Draw(rc);
    wchar_t buf[256];
    int count = (int)m_scoresScore.size();
    if (count > 5) count = 5;
    // スコア部門
    for (int i = 0; i < 5; i++) {
        swprintf_s(buf, L"位:%.2f", m_scoresScore[i] / 100.0); // スコアのみ表示
        m_fontsScore[i].SetText(buf);
        m_fontsScore[i].SetPosition(-500, 100 - i * 90, 0);
        m_fontsScore[i].SetColor(0, 0, 0, 1);  // ← 白
        m_fontsScore[i].SetScale(1.3f);
        m_fontsScore[i].Draw(rc);
        // ★ このスコアが何回ぐるぐるしたか
        int c = m_scoresGuruguru[i];
        float r2, g2, b2;

        // 0〜9：黒
        if (c < 10) {
            r2 = 0.0f; g2 = 0.0f; b2 = 0.0f;
        }
        // 10〜14：青
        else if (c < 15) {
            r2 = 0.0f; g2 = 0.0f; b2 = 1.0f;
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
        m_fontsGuruguru[i].SetPosition(-220, 100 - i * 90, 0);
        m_fontsGuruguru[i].SetScale(1.2f);
        m_fontsGuruguru[i].SetColor(r2, g2, b2, 1.0f);
        m_fontsGuruguru[i].Draw(rc);
    }

    // メートル部門
    for (int i = 0; i < 5; i++) {
        swprintf_s(buf, L"位:%.2f m", m_scoresMeter[i] / 100.0);
        m_fontsMeter[i].SetText(buf);
        m_fontsMeter[i].SetPosition(200, 100 - i * 90, 0);
        m_fontsMeter[i].SetColor(0, 0, 0, 1);  // ← 白
        m_fontsMeter[i].SetScale(1.3f);
        m_fontsMeter[i].Draw(rc);
    }

}
