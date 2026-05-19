#include "stdafx.h"
#include "Result.h"
#include"Source/Scene/Titer/Titer.h"
#include"Source/Sound/SoundManager.h"
#include"Source/Scene/Ranking/Ranking.h"

bool Result::Start()
{
	// ▼ 保存された音量を取得
	float v = g_soundManager->m_bgmVolume / 100.0f;

	// ▼ カーブ適用（1.8乗など）
	float curved = powf(v, 2.0f);
	g_bgm = g_soundManager->PlayingSound(enSound_ResultBGM, false, curved);

	m_spriteRender.Init("Assets/sprite/result2.dds", 1920.0f, 1080.0f);

	m_rezarut.Init("Assets/sprite/risarut.dds", 800.0f, 600.0f);
	m_rezarut.SetPosition({ 0.0f, 400.0f, 0.0f });

	m_B.Init("Assets/sprite/AAA.dds", 220.0f, 170.0f);
	m_B.SetPosition({ 830.0f, -400.0f, 0.0f });

	m_grobu.Init("Assets/sprite/guro-bu.dds", 450.0f, 430.0f);
	m_grobu.SetPosition({ 830.0f, -400.0f, 0.0f});

	/*m_burakku.Init("Assets/sprite/burakku.dds", 600.0f, 50.0f);
	m_burakku.SetPosition({ 190.0f, -40.0f ,0.0f });*/

	m_skip.Init("Assets/sprite/Askep.dds", 220.0f, 170.0f);
	m_skip.SetPosition({ 830.0f, -400.0f, 0.0f });

	m_newRecord.Init("Assets/sprite/new.dds", 600.0f, 600.0f);
	m_newRecord.SetPosition({ 450, 130, 0 });
	m_newRecord.SetMulColor({ 1,1,1,0 }); // 最初は非表示

	m_fadeSprite.Init("Assets/sprite/kuro.dds", 1920.0f, 1080.0f);
	m_fadeAlpha = 0.0f;

	m_hasScore = false;
	for (int i = 0; i < 3; i++) {
		if (m_threeShots[i] > 0) {
			m_hasScore = true;
			break;
		}
	}

	// ★ SE 音量が 0 の場合は SE2 を即削除
	if (g_soundManager->m_seVolume <= 0.0f||!m_hasScore) {
		m_isFadingSE2 = false;
	}
	else {
		m_se2Volume = 3.0f;
		m_isFadingSE2 = true;
	}

	return true;
}


void Result::Update()
{
	float dt = g_gameTime->GetFrameDeltaTime();
	if (m_isFadingSE2) {

		m_se2Timer += dt;

		if (m_se2Timer >= 3.5f) {
			m_se2Volume -= 0.05f;
			if (m_se2Volume <= 0.0f) {
				m_se2Volume = 0.0f; m_isFadingSE2 = false;
			}
		}

		// 音量反映
		auto se2 = g_soundManager->GetSE2();
		if (se2) {
			se2->SetVolume(m_se2Volume);
		}
	}

	if (g_pad[0]->IsTrigger(enButtonA)) {
		if (m_phase != enPhase_WaitKey) {
			// 【スキップ】即座に全数値を最大にする
			m_displayGuruguru = m_guruguru;
			m_displayKm = (float)m_km;
			m_displayFinalScore = (float)m_km;
			m_phase = enPhase_WaitKey;
			m_isSkipped = true;
		}
		else {
			StartFadeOut(1.0f, [this]() {

				// ★ BGM フェードアウト
				if (g_bgm) g_bgm->SetVolume(0.0f);

				// ★ SE2 もフェードアウト
				auto se2 = g_soundManager->GetSE2();
				if (se2) se2->SetVolume(0.0f);

				// ★ フェード完了後にタイトルへ
				NewGO<Titer>(0);
				DeleteGO(this);
			});
		}
	}

	if (m_phase != enPhase_WaitKey) {
		m_phaseTimer += dt;

		switch (m_phase) {
		case enPhase_ScoreStep1: // 【2.0秒】 飛距離 ＆ スコア（暫定）カウントアップ
			m_displayKm += m_originalKmAddPerFrame * dt;
			// ★スコアも飛距離と同じ速度で増やす
			m_displayFinalScore = m_displayKm;

			if (m_phaseTimer >= 1.5f) {
				m_displayKm = (float)m_originalKm;
				m_displayFinalScore = (float)m_originalKm; // 飛距離で一旦止める
				m_phase = enPhase_Guruguru;
				m_phaseTimer = 0.0f;
			}
			break;

		case enPhase_Guruguru:  // 【1.0秒】 ぐるぐる演出
			m_guruguruAccumulator += m_countSpeed * dt;
			m_displayGuruguru = (int)m_guruguruAccumulator;

			// ★ここでは m_displayFinalScore は m_originalKm のまま維持

			if (m_phaseTimer >= 1.5f) {
				m_displayGuruguru = m_guruguru;
				m_guruguruAccumulator = 0.0f;
				m_phase = enPhase_ScoreStep2;
				m_phaseTimer = 0.0f;

				// ★最終スコアまでの差分を 0.5秒で埋める計算
				// (m_km - 現在のスコア) を 0.5秒で割る
				m_kmAddPerFrame = (m_km - m_displayFinalScore) / 2.0f;
			}
			break;

		case enPhase_ScoreStep2: // 【0.5秒で最大飛距離から最終スコアへ】
			m_displayFinalScore += m_kmAddPerFrame * dt;
			if (m_displayFinalScore >= (float)m_km || m_phaseTimer >= 2.0f) {
				m_displayFinalScore = (float)m_km;
				m_phase = enPhase_WaitKey;
			}
			break;
		}
	}

	if (m_phase == enPhase_WaitKey && !m_isScoreFixed) {
		m_isScoreFixed = true;
		m_isSkipped = true;

		// (ランキング保存やNewRecord判定など、元々あった「if(!m_isScoreFixed)」の中身をここに)
		auto se2 = g_soundManager->GetSE2();
		if (se2) { se2->Stop(); DeleteGO(se2); g_soundManager->ClearSE2(); }

		Ranking* ranking = NewGO<Ranking>(0, "ranking");
		ranking->Load();
		if (ranking->AddScore(m_km, m_originalKm, m_guruguru)) {
			m_isNewRecord = true;
			m_newRecord.SetMulColor({ 1,1,1,1 });
			auto se = g_soundManager->PlaySE(enSound_SE4);
			if (se) se->SetVolume(1.5f);
			m_isBlinking = true;
			m_blinkCount = 0;
			m_blinkTimer = 0.0f;
		}
		DeleteGO(ranking);
	}

	if (!m_isSkipped) {
		// ★ スキップ前
		m_B.SetMulColor({ 1,1,1,0 });
		m_skip.SetMulColor({ 1,1,1,1 });
	}
	else {
		m_B.SetMulColor({ 1,1,1,1 });
		m_skip.SetMulColor({ 1,1,1,0 });
	}

	// ★★★ NEW!! 点滅処理 ★★★
	if (m_isBlinking) {

		m_blinkTimer += dt;

		if (m_blinkTimer >= 0.25f) {
			m_blinkTimer = 0.0f;
			float alpha = (m_newRecord.GetMulColor().w > 0.5f) ? 0.0f : 1.0f;
			m_newRecord.SetMulColor({ 1,1,1,alpha });
			if (alpha > 0.5f) {
				m_blinkCount++;
				auto se = g_soundManager->PlaySE(enSound_SE4);
				if (se) se->SetVolume(1.5f);
			}
			if (m_blinkCount >= 3) {
				m_isBlinking = false; m_newRecord.SetMulColor({ 1,1,1,1 });
			}
		}
	}

	// ★ フェードアウト処理
	if (m_isFadeOut) {
		m_fadeAlpha += m_fadeSpeed * dt;
		if (m_fadeAlpha >= 1.0f) {
			m_fadeAlpha = 1.0f;
			m_isFadeOut = false;
			if (m_onFadeOutFinished) m_onFadeOutFinished();
		}
	}
}

void Result::SetResultValues(int guruguru, int bestKm, int scores[3]) {

	for (int i = 0; i < 3; i++) {
		m_threeShots[i] = scores[i];
	}

	// ★ 基本倍率
	double multiplier = pow(1.01, (double)guruguru);

	// ★ 追加倍率
	int step = guruguru / 3;
	double extra = step * 0.003;

	multiplier *= (1.0 + extra);

	// ★ 小数点第4位で切り捨て
	multiplier = floor(multiplier * 10000.0) / 10000.0;

	// ★★★ 30回MAXボーナス（強制1.4倍） ★★★
	if (guruguru >= 40) {
		multiplier = 1.5;
	}

	// --- (倍率計算のロジックは変更なし) ---
	m_km = (int)(bestKm * multiplier);
	m_originalKm = bestKm;
	m_guruguru = guruguru;
	m_multiplier = multiplier;

	// --- ここから修正：カウントスピードを秒数ベースに変更 ---
	m_displayGuruguru = 0;
	m_displayKm = 0;
	m_displayOriginalKm = 0;

	// 1.5秒で originalKm まで
	m_originalKmAddPerFrame = (float)m_originalKm / 1.5f;
	// 2.0秒で guruguru まで
	m_countSpeed = (float)m_guruguru / 1.5f;
	// 1.5秒で差分(km - originalKm)を増やす
	m_kmAddPerFrame = (float)(m_km - m_originalKm) / 2.0f;

	m_phase = enPhase_ScoreStep1;
	m_phaseTimer = 0.0f;
}

void Result::StartFadeOut(float speed, std::function<void()> onFinished)
{
	m_isFadeOut = true;
	m_fadeSpeed = speed;
	m_onFadeOutFinished = onFinished;
}

void Result::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);

	m_grobu.Update();
	m_grobu.Draw(rc);

	/*m_burakku.Update();
	m_burakku.Draw(rc);*/

	m_B.Update();
	m_B.Draw(rc);

	m_skip.Update();
	m_skip.Draw(rc);

	m_rezarut.Update();
	m_rezarut.Draw(rc);

	if (m_isNewRecord) {
		m_newRecord.Update();
		m_newRecord.Draw(rc);
	}

	wchar_t buf[256];

	// --- ① 一番上の「距離」 (最初から表示) ---
	swprintf_s(buf, L"距離　%.2fm", m_displayKm / 100.0f);
	m_fontTopKm.SetText(buf);
	m_fontTopKm.SetPosition(-600, 200, 0); // 位置は適宜調整
	m_fontTopKm.SetColor(1, 1, 1, 1);
	m_fontTopKm.Draw(rc);

	//スコア
	swprintf_s(buf, L"スコア　%.2f", m_displayFinalScore / 100.0f);
	m_moto.SetText(buf); // 既存のスコア用フォントを使用
	m_moto.SetPosition(-600, -50, 0);
	m_moto.SetScale(2.5f); // 最終結果なので大きく
	m_moto.SetColor(1, 1, 1, 1); // 黄色などで強調
	m_moto.Draw(rc);

	// ★★★ ぐるぐる倍率の表示 ★★★
	if (m_phase >= enPhase_Guruguru) {
		// ① 動的な係数計算
		double currentMul = 1.0 + (m_multiplier - 1.0) * ((double)m_displayGuruguru / (m_guruguru > 0 ? m_guruguru : 1));

		// ② パーセント表記用の計算 (例: 1.1402 -> 0.1402 -> 14.02)
		double bonusPercent = (currentMul - 1.0) * 100.0;

		// ③ パーセント文字列の作成
		wchar_t percentBuf[64];
		swprintf_s(percentBuf, L"%.2f", bonusPercent);

		// ④ 末尾の無駄な 0 を削る (14.00 -> 14 / 14.50 -> 14.5)
		int pLen = (int)wcslen(percentBuf);
		while (pLen > 0 && percentBuf[pLen - 1] == L'0') {
			percentBuf[--pLen] = L'\0';
		}
		if (pLen > 0 && percentBuf[pLen - 1] == L'.') {
			percentBuf[--pLen] = L'\0';
		}

		// --- 上の行：ボーナス表記に変更 ---
		// 「%%」と2つ書くことで、画面に1つの「%」が表示されます
		swprintf_s(buf, L"ぐるぐる回数: %d回    ボーナス(+%ls%%)", m_displayGuruguru, percentBuf);
		m_fontGuruguru.SetText(buf);
		m_fontGuruguru.SetPosition(-600, 120, 0);
		m_fontGuruguru.SetColor(1, 1, 1, 1);
		m_fontGuruguru.Draw(rc);

		// --- 下の行：計算式の表示 ---
		wchar_t mulBuf[64];
		swprintf_s(mulBuf, L"%.4f", currentMul);
		int len = (int)wcslen(mulBuf);
		while (len > 0 && mulBuf[len - 1] == L'0') { mulBuf[--len] = L'\0'; }
		if (len > 0 && mulBuf[len - 1] == L'.') { mulBuf[--len] = L'\0'; }

		swprintf_s(buf, L"最終距離 = %.2fm * %ls", m_displayKm / 100.0f, mulBuf);
		m_fontFormula.SetText(buf);
		m_fontFormula.SetPosition(-600, 40, 0);
		m_fontFormula.SetColor(1, 1, 1, 1);
		m_fontFormula.Draw(rc);
	}

	//３つの飛距離
	if (m_phase == enPhase_WaitKey) {
		int bestIndex = 0;
		for (int i = 1; i < 3; i++) {
			if (m_threeShots[i] > m_threeShots[bestIndex]) {
				bestIndex = i;
			}
		}

		wchar_t buf2[256];
		for (int i = 0; i < 3; i++) {
			double meter = (double)m_threeShots[i] / 100.0;

			// ★ 赤い「1:」「2:」「3:」
			wchar_t numBuf[32];
			swprintf_s(numBuf, L"%d:", i + 1);

			m_fontThreeShots[i].SetText(numBuf);
			m_fontThreeShots[i].SetPosition(-600 + i * 350, -200, 0);
			m_fontThreeShots[i].SetScale(1.3f);
			m_fontThreeShots[i].SetColor(1, 0, 0, 1);   // ← 赤
			// ★ 1番だけ黄色、それ以外は赤
			if (i == bestIndex) {
				m_fontThreeShots[i].SetColor(1, 0.84, 0, 1);   // 黄色
			}
			else {
				m_fontThreeShots[i].SetColor(1, 0, 0, 1);   // 赤
			}
			m_fontThreeShots[i].Draw(rc);

			// ★ 白い「100.00 m」部分
			wchar_t meterBuf[64];
			swprintf_s(meterBuf, L" %.2f m", meter);

			m_fontThreeShotsValue[i].SetText(meterBuf);
			m_fontThreeShotsValue[i].SetPosition(-600 + i * 350 + 20, -200, 0);
			// ↑ 数字の後ろに少し右へずらす
			m_fontThreeShotsValue[i].SetScale(1.3f);
			m_fontThreeShotsValue[i].SetColor(1, 1, 1, 1);  // ← 白
			m_fontThreeShotsValue[i].Draw(rc);
		}
	}

	if (m_fadeAlpha > 0.0f) {
		m_fadeSprite.SetMulColor({ 0,0,0, m_fadeAlpha });
		m_fadeSprite.SetPosition({ 0,0,0 });
		m_fadeSprite.Update();
		m_fadeSprite.Draw(rc);
	}
}