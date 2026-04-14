#include "stdafx.h"
#include "Result.h"
#include"Source/Scene/Titer/Titer.h"
#include"Source/Sound/SoundManager.h"

bool Result::Start()
{
	// ▼ 保存された音量を取得
	float v = g_soundManager->m_bgmVolume / 100.0f;

	// ▼ カーブ適用（1.8乗など）
	float curved = powf(v, 2.0f);
	g_bgm = g_soundManager->PlayingSound(enSound_ResultBGM, false, curved);

	m_spriteRender.Init("Assets/sprite/result2.dds", 1920.0f, 1080.0f);

	m_B.Init("Assets/sprite/AAA.dds", 220.0f, 170.0f);
	m_B.SetPosition({ 730.0f, -400.0f, 0.0f });

	m_grobu.Init("Assets/sprite/guro-bu.dds", 450.0f, 430.0f);
	m_grobu.SetPosition({ 730.0f, -400.0f ,0.0f});

	m_burakku.Init("Assets/sprite/burakku.dds", 600.0f, 50.0f);
	m_burakku.SetPosition({ 190.0f, -40.0f ,0.0f });

	// ★ SE 音量が 0 の場合は SE2 を即削除
	if (g_soundManager->m_seVolume <= 0.0f) {
		auto se2 = g_soundManager->GetSE2();
		if (se2) {
			se2->Stop();
			DeleteGO(se2);
			g_soundManager->ClearSE2();
		}
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

	if (m_isFadingSE2) {
		m_se2Volume -= 0.012f;

		if (m_se2Volume <= 0.0f) {
			m_se2Volume = 0.0f;
			m_isFadingSE2 = false;
		}

		auto se2 = g_soundManager->GetSE2();
		if (se2) {
			se2->SetVolume(m_se2Volume);

			// 完全に 0 になったら消してしまうならここで
			if (!m_isFadingSE2) {
				se2->Stop();
				DeleteGO(se2);
				g_soundManager->ClearSE2();
			}
		}
	}

	// ★ ぐるぐるのカウントアップ
	if (m_displayGuruguru < m_guruguru) {
		m_displayGuruguru += (int)(m_countSpeed * 60); // 1秒で60増える
		if (m_displayGuruguru > m_guruguru)
			m_displayGuruguru = m_guruguru;
	}

	// ★ km のカウントアップ
	// ★ km を必ず5秒でカウントアップ
	if (m_displayKm < m_km) {
		m_displayKm += m_kmAddPerFrame;

		if (m_displayKm > m_km)
			m_displayKm = m_km;
	}

	// ★ 元の km のカウントアップ
	if (m_displayOriginalKm < m_originalKm) {
		m_displayOriginalKm += m_originalKmAddPerFrame;

		if (m_displayOriginalKm > m_originalKm)
			m_displayOriginalKm = m_originalKm;
	}

	if (g_pad[0]->IsTrigger(enButtonA)) {

		// ★ まだカウントアップ中なら即終了させる
		if (m_displayKm < m_km || m_displayOriginalKm < m_originalKm || m_displayGuruguru < m_guruguru) {

			// ぐるぐる
			m_displayGuruguru = m_guruguru;

			// 元の km
			m_displayOriginalKm = m_originalKm;

			// 倍率後 km
			m_displayKm = m_km;

			return; // ← ここで終了。次のフレームから A で進める
		}

		// ★ ここに来たらスコアはすでに完成しているので次へ進む
		auto se2 = g_soundManager->GetSE2();
		if (se2) {
			se2->Stop();
			DeleteGO(se2);
			g_soundManager->ClearSE2();
		}

		NewGO<Titer>(0);
		DeleteGO(this);
	}

}

void Result::SetResultValues(int guruguru, int bestKm, int scores[3]) {

	// 3球分の km を保存（倍率なし）
	for (int i = 0; i < 3; i++) {
		m_threeShots[i] = scores[i];
	}

	// ★ ここを追加：ぐるぐる値をメンバに保存
	m_guruguru = guruguru;

	// 元の km（最大距離）
	m_originalKm = bestKm;

	// ぐるぐる倍率
	double multiplier = pow(1.01, (double)guruguru);

	// 倍率後 km（スコア）
	m_km = (int)(bestKm * multiplier);

	// カウントアップ初期化
	m_displayGuruguru = 0;
	m_displayKm = 0;
	m_displayOriginalKm = 0;

	m_kmAddPerFrame = (float)m_km / 258.0f;
	m_originalKmAddPerFrame = (float)m_originalKm / 180.0f;
}

void Result::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);

	m_grobu.Update();
	m_grobu.Draw(rc);

	m_B.Update();
	m_B.Draw(rc);

	m_burakku.Update();
	m_burakku.Draw(rc);

	wchar_t buf[256];
	// ぐるぐる
	swprintf_s(buf, L"ぐるぐる: %d", m_displayGuruguru);
	m_fontGuruguru.SetText(buf);
	m_fontGuruguru.SetPosition(300, 230, 0);
	m_fontGuruguru.SetScale(1.5f);
	m_fontGuruguru.SetColor(0, 0, 0, 1);
	m_fontGuruguru.Draw(rc);

	swprintf_s(buf, L"スコア%.2f",m_displayKm/100.0);
	m_moto.SetText(buf);
	m_moto.SetPosition(-200, 120, 0);
	m_moto.SetScale(2.0f);
	m_moto.SetColor(0,0,0,1);
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
		m_fontThreeShots[i].SetPosition(-110 + i * 200, -20, 0);
		m_fontThreeShots[i].SetScale(0.8f);
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
		m_fontThreeShotsValue[i].SetPosition(-110 + i * 200 + 20, -20, 0);
		// ↑ 数字の後ろに少し右へずらす
		m_fontThreeShotsValue[i].SetScale(0.8f);
		m_fontThreeShotsValue[i].SetColor(1, 1, 1, 1);  // ← 白
		m_fontThreeShotsValue[i].Draw(rc);
	}

}