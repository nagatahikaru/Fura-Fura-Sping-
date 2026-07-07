#include "stdafx.h"
#include "Result.h"
#include"Source/Scene/Titer/Titer.h"
#include"Source/Sound/SoundManager.h"
#include"Source/Scene/Ranking/Ranking.h"

bool Result::Start()
{
	// ▼ BGM（マスターは PlayingSound 側で掛ける前提）
	float v = g_soundManager->m_bgmVolume / 100.0f;
	float curved = powf(v, 2.0f);
	g_bgm = g_soundManager->PlayingSound(enSound_ResultBGM, false, curved);

	// ▼ ここから歓声(SE2)の制御
	m_se2Timer = 0.0f;

	// ★ Master が 0 なら歓声は即止める
	if (g_soundManager->m_masterVolume <= 0.0f) {
		g_soundManager->StopSE2();      // ← ここで完全停止
		m_isFadingSE2 = false;
	}
	else if (g_soundManager->m_seVolume <= 0.0f || !m_hasScore) {
		m_isFadingSE2 = false;
	}
	else {
		m_se2Volume = 3.0f;
		m_isFadingSE2 = true;
	}

	m_spriteRender.Init("Assets/sprite/result2.dds", 1920.0f, 1080.0f);

	m_rezarut.Init("Assets/sprite/risarut.dds", 800.0f, 600.0f);
	m_rezarut.SetPosition({ 0.0f, 400.0f, 0.0f });

	m_B.Init("Assets/sprite/AAA.dds", 220.0f, 170.0f);
	m_B.SetPosition({ 830.0f, -490.0f, 0.0f });

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
	m_se2Timer = 0.0f;   // ★ SE2 の経過時間

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

		// ★ 経過時間を進める
		m_se2Timer += dt;

		// ★ 4.5秒までは音量そのまま
		if (m_se2Timer < 4.5f) {
			// 何もしない（音量固定）
		}
		else {
			// ★ 4.5秒経過後 → 2秒かけてフェードアウト
			float fadeTime = m_se2Timer - 4.5f;  // 0〜2秒

			// 2.5 → 0 を 2秒で
			float t = fadeTime / 2.0f;
			if (t > 1.0f) t = 1.0f;

			m_se2Volume = 2.5f * (1.0f - t);

			if (m_se2Volume <= 0.0f) {
				m_se2Volume = 0.0f;
				m_isFadingSE2 = false;
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

			// ★ スキップ：表示だけ最終値にする
			m_displayGuruguru = m_guruguru;
			m_displayKm = (float)m_originalKm;
			m_displayFinalScore = (float)m_km;

			m_phase = enPhase_WaitKey;

			// ★ ここでは m_isScoreFixed を触らない！
			m_isSkipped = true;

			// ★ SE2 フェードアウトは止める
			m_isFadingSE2 = false;
		}
		else {
			StartFadeOut(1.0f, [this]() {

				// BGM は即消しでOK
				if (g_bgm) g_bgm->SetVolume(0.0f);

				// ★ SE2 が残っていたら 1秒フェードアウト
				if (g_soundManager && g_soundManager->GetSE2()) {
					g_soundManager->FadeOutSE2(0.5f);
				}

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

			if (m_phaseTimer >= 1.5f) {
				m_displayGuruguru = m_guruguru;
				m_guruguruAccumulator = 0.0f;
				m_phase = enPhase_ScoreStep2;
				m_phaseTimer = 0.0f;

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

		Ranking* ranking = NewGO<Ranking>(0, "ranking");
		ranking->Load();
		if (ranking->AddScore(m_km, m_originalKm, m_guruguru)) {
			m_isNewRecord = true;
			m_newRecord.SetMulColor({ 1,1,1,1 });

			g_soundManager->PlaySE(enSound_SE4);

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

				g_soundManager->PlaySE(enSound_SE4);
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

	// ★ 0回＝1倍、40回＝40倍 の反比例カーブ
	double t = (double)guruguru / 50.0;
	if (t < 0.0) t = 0.0;
	if (t > 1.0) t = 1.0;

	// カーブの急さ（2.0〜3.0で調整可能）
	double p = 2.5;

	// 1 + 39 * t^p
	double maxMultiplier = 50.0; // ハード（デフォルトは50倍マックス）

	if (m_difficulty == Difficulty::Easy) {
		maxMultiplier = 10.0;     // イージーは10倍マックス
	}
	else if (m_difficulty == Difficulty::Normal) {
		maxMultiplier = 45.0;     // ノーマルは25倍マックス
	}
	else if (m_difficulty == Difficulty::Hard) {
		maxMultiplier = 100.0;     // ノーマルは25倍マックス
	}

	// 各難易度に応じた倍率計算式 ( 1.0 + (マックス倍率 - 1.0) * t^p )
	double multiplier = 1.0 + (maxMultiplier - 1.0) * pow(t, p);

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

	/*m_grobu.Update();
	m_grobu.Draw(rc);*/

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
	m_fontTopKm.SetPosition(-630, 200, 0); // 位置は適宜調整
	m_fontTopKm.SetColor(1, 1, 1, 1);
	m_fontTopKm.Draw(rc);

	//スコア
	swprintf_s(buf, L"スコア　%.2f", m_displayFinalScore / 100.0f);
	m_moto.SetText(buf); // 既存のスコア用フォントを使用
	m_moto.SetPosition(-630, -50, 0);
	m_moto.SetScale(2.5f); // 最終結果なので大きく
	m_moto.SetColor(1, 1, 1, 1); // 黄色などで強調
	m_moto.Draw(rc);

	// ★★★ ぐるぐる倍率の表示 ★★★
	if (m_phase >= enPhase_Guruguru) {
		// ① 動的な係数計算
		double currentMul = 1.0 + (m_multiplier - 1.0) *((double)m_displayGuruguru / (m_guruguru > 0 ? m_guruguru : 1));

		// 小数2桁に整形
		wchar_t mulBuf[64];
		swprintf_s(mulBuf, L"%.4f", currentMul);

		// --- 上の行：ぐるぐる回数と倍率 ---
		swprintf_s(buf, L"ぐるぐる回数: %d回  倍率(%ls倍)", m_displayGuruguru, mulBuf);
		m_fontGuruguru.SetText(buf);
		m_fontGuruguru.SetPosition(-630, 120, 0);
		m_fontGuruguru.SetColor(1, 1, 1, 1);
		m_fontGuruguru.Draw(rc);

		// --- 下の行：計算式（距離 × 倍率） ---
		swprintf_s(buf, L"最終スコア = %.2fm * %ls倍", m_displayKm / 100.0f, mulBuf);
		m_fontFormula.SetText(buf);
		m_fontFormula.SetPosition(-630, 40, 0);
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
			m_fontThreeShots[i].SetPosition(-630 + i * 350, -200, 0);
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
			m_fontThreeShotsValue[i].SetPosition(-630 + i * 350 + 20, -200, 0);
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