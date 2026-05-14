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
	m_newRecord.SetPosition({ -500, 130, 0 });
	m_newRecord.SetMulColor({ 1,1,1,0 }); // 最初は非表示

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
			m_phase = enPhase_WaitKey;
			m_isSkipped = true;
		}
		else {
			// 【タイトルへ】演出が終わっていれば遷移
			NewGO<Titer>(0);
			DeleteGO(this);
			return;
		}
	}

	if (m_phase != enPhase_WaitKey) {
		m_phaseTimer += dt;

		switch (m_phase) {
		case enPhase_ScoreStep1: // 【1.5秒】 0 ～ 元のスコア
			m_displayKm += m_originalKmAddPerFrame * dt;
			if (m_phaseTimer >= 2.0f) {
				m_displayKm = (float)m_originalKm; // 補正
				m_phase = enPhase_Guruguru;
				m_phaseTimer = 0.0f;
			}
			break;
		case enPhase_Guruguru:
			m_guruguruAccumulator += m_countSpeed * dt;
			m_displayGuruguru = (int)m_guruguruAccumulator;

			if (m_phaseTimer >= 1.0f) {
				m_displayGuruguru = m_guruguru;
				m_guruguruAccumulator = 0.0f; // メンバ変数をリセット
				m_phase = enPhase_ScoreStep2;
				m_phaseTimer = 0.0f;
			}
			break;
		case enPhase_ScoreStep2: // 【1.5秒】 元のスコア ～ 最終スコア
			m_displayKm += m_kmAddPerFrame * dt;
			if (m_phaseTimer >= 2.0f) {
				m_displayKm = (float)m_km;         // 最終確定
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
	if (guruguru >= 30) {
		multiplier = 1.4;
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
	m_originalKmAddPerFrame = (float)m_originalKm / 2.0f;
	// 2.0秒で guruguru まで
	m_countSpeed = (float)m_guruguru / 1.0f;
	// 1.5秒で差分(km - originalKm)を増やす
	m_kmAddPerFrame = (float)(m_km - m_originalKm) / 2.0f;

	m_phase = enPhase_ScoreStep1;
	m_phaseTimer = 0.0f;
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
	// ぐるぐる
	if (m_phase >= enPhase_Guruguru) {
		swprintf_s(buf, L"ぐるぐる: %d回", m_displayGuruguru);
		m_fontGuruguru.SetText(buf);
		m_fontGuruguru.SetPosition(-200, 190, 0);
		m_fontGuruguru.SetScale(1.5f);
		m_fontGuruguru.SetColor(1, 1, 1, 1);
		m_fontGuruguru.Draw(rc);
	}

	swprintf_s(buf, L"スコア%.2f",m_displayKm/100.0);
	m_moto.SetText(buf);
	m_moto.SetPosition(-300, 80, 0);
	m_moto.SetScale(2.0f);
	m_moto.SetColor(1,1,1,1);
	m_moto.Draw(rc);

	// ★★★ ここに入れる！ ★★★
	int bestIndex = 0;
	for (int i = 1; i < 3; i++) {
		if (m_threeShots[i] > m_threeShots[bestIndex]) {
			bestIndex = i;
		}
	}
	// ★★★ ここに追加する！ ★★★
	wchar_t buf2[256];
	for (int i = 0; i < 3; i++) {
		double meter = (double)m_threeShots[i] / 100.0;

		// ★ 赤い「1:」「2:」「3:」
		wchar_t numBuf[32];
		swprintf_s(numBuf, L"%d:", i + 1);

		m_fontThreeShots[i].SetText(numBuf);
		m_fontThreeShots[i].SetPosition(-500 + i * 350, -60, 0);
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
		m_fontThreeShotsValue[i].SetPosition(-500 + i * 350 + 20, -60, 0);
		// ↑ 数字の後ろに少し右へずらす
		m_fontThreeShotsValue[i].SetScale(1.3f);
		m_fontThreeShotsValue[i].SetColor(1, 1, 1, 1);  // ← 白
		m_fontThreeShotsValue[i].Draw(rc);
	}
	// ★★★ ぐるぐる倍率の表示 ★★★
	if (m_phase >= enPhase_Guruguru) {
		double currentMul = 1.0 + (m_multiplier - 1.0) * ((double)m_displayGuruguru / (m_guruguru > 0 ? m_guruguru : 1));
		wchar_t mulBuf[64];
		swprintf_s(mulBuf, L"%.4f", currentMul);

		// ★ 末尾の0を削る
		int len = (int)wcslen(mulBuf);
		while (len > 0 && mulBuf[len - 1] == L'0') {
			mulBuf[--len] = L'\0';
		}
		// 小数点だけ残ったら消す
		if (len > 0 && mulBuf[len - 1] == L'.') {
			mulBuf[--len] = L'\0';
		}

		wchar_t mulText[128];
		swprintf_s(mulText, L"(%ls倍)", mulBuf);
		m_fontMultiplier.SetText(mulText);
		m_fontMultiplier.SetPosition(270, 190, 0);
		m_fontMultiplier.SetScale(1.5f);
		m_fontMultiplier.SetColor(1, 1, 0.2f, 1); // 黄色で強調
		m_fontMultiplier.Draw(rc);
	}
}