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

	// ★ 元の km のカウントアップ
	if (m_displayOriginalKm < m_originalKm) {
		m_displayOriginalKm += m_originalKmAddPerFrame;

		if (m_displayOriginalKm > m_originalKm)
			m_displayOriginalKm = m_originalKm;
	}

	if (g_pad[0]->IsTrigger(enButtonA)) {

		// ★ シーン遷移前に SE2 を必ず止める
		auto se2 = FindGO<SoundSource>("SE2");
		if (se2) {
			se2->Stop();
			DeleteGO(se2);
		}

		NewGO<Titer>(0);
		DeleteGO(this);
	}
}

void Result::SetResultValues(int guruguru, int km) {
	m_guruguru = guruguru;

	// ★ 元の km を保存
	m_originalKm = km;

	// ★ ぐるぐる1回につき1.01倍する
	double multiplier = pow(1.01, (double)guruguru);

	// ★ 書けたスコア（倍率後）
	m_km = (int)(km * multiplier);

	// ★ カウントアップ初期化
	m_displayGuruguru = 0;
	m_displayKm = 0;
	m_displayOriginalKm = 0;

	// ★ 5秒（270フレーム）で到達
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

	wchar_t buf[256];
	// ぐるぐる
	swprintf_s(buf, L"ぐるぐる: %d", m_displayGuruguru);
	m_fontGuruguru.SetText(buf);
	m_fontGuruguru.SetPosition(300, 230, 0);
	m_fontGuruguru.SetScale(1.5f);
	m_fontGuruguru.SetColor(0, 0, 0, 1);
	m_fontGuruguru.Draw(rc);

	// 元の km（倍率前）
	double originalMeter = (double)m_displayOriginalKm / 100.0;
	swprintf_s(buf, L"%.2f m", originalMeter);
	m_fontKm.SetText(buf);
	m_fontKm.SetPosition(-50, 0, 0);  // ← 好きな位置に調整
	m_fontKm.SetScale(1.0f);
	m_fontKm.SetColor(1, 1, 1, 1);
	m_fontKm.Draw(rc);

	swprintf_s(buf, L"スコア%.2f",m_displayKm/100.0);
	m_moto.SetText(buf);
	m_moto.SetPosition(-200, 120, 0);
	m_moto.SetScale(2.0f);
	m_moto.SetColor(0,0,0,1);
	m_moto.Draw(rc);
}