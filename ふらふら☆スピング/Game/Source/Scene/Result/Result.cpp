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

	m_B.Init("Assets/sprite/Bback.dds", 220.0f, 170.0f);
	m_B.SetPosition({ 730.0f, -400.0f, 0.0f });

	m_grobu.Init("Assets/sprite/guro-bu.dds", 450.0f, 430.0f);
	m_grobu.SetPosition({ 730.0f, -400.0f ,0.0f});

	// ★ SE 音量が 0 の場合は SE2 を即削除
	if (g_soundManager->m_seVolume <= 0.0f) {
		auto se2 = FindGO<SoundSource>("SE2");
		if (se2) {
			se2->Stop();
			DeleteGO(se2);
		}
		m_isFadingSE2 = false;  // フェード処理もしない
	}
	else {
		// ★ フェードアウト開始
		m_se2Volume = 3.0f;
		m_isFadingSE2 = true;
	}

	return true;
}


void Result::Update()
{
	// ★ SE2 フェードアウト処理
	if (m_isFadingSE2) {
		m_se2Volume -= 0.012f;   // ← フェード速度（調整可）

		if (m_se2Volume <= 0.0f) {
			m_se2Volume = 0.0f;
			m_isFadingSE2 = false;
		}

		// ★ 実際の音量に反映
		// SE2 の SoundSource を全部探して音量を下げる
		auto se2 = FindGO<SoundSource>("SE2");
		if (se2) {
			se2->SetVolume(m_se2Volume);
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


	if (g_pad[0]->IsTrigger(enButtonB)) {
		NewGO<Titer>(0);
		DeleteGO(this);
	}
}

void Result::SetResultValues(int guruguru, int km) {
	m_guruguru = guruguru;
	m_km = km;

	m_displayGuruguru = 0;
	m_displayKm = 0;

	m_kmAddPerFrame = (float)m_km / 270.0f;
}


void Result::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);

	m_grobu.Update();
	m_grobu.Draw(rc);

	m_B.Update();
	m_B.Draw(rc);

	wchar_t buf[256];
	// ぐるぐる
	swprintf_s(buf, L"ぐるぐる: %d", m_displayGuruguru);
	m_fontGuruguru.SetText(buf);
	m_fontGuruguru.SetPosition(-130, 130, 0);
	m_fontGuruguru.SetScale(1.5f);
	m_fontGuruguru.Draw(rc);

	// km
	swprintf_s(buf, L"%d km", m_displayKm);
	m_fontKm.SetText(buf);
	m_fontKm.SetPosition(-50, 0, 0);
	m_fontKm.SetScale(1.5f);
	m_fontKm.Draw(rc);
}