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
	m_grobu.SetPosition({ 730.0f, -400.0f, 0.0f });
	return true;
}

void Result::Update()
{
	if (g_pad[0]->IsTrigger(enButtonB)) {

		NewGO<Titer>(0);
		DeleteGO(this);
	}
	
}

void Result::SetResultValues(int guruguru, int km) {
	m_guruguru = guruguru;
	m_km = km;
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
	swprintf_s(buf, L"ぐるぐる: %d", m_guruguru);
	m_fontGuruguru.SetText(buf);
	m_fontGuruguru.SetPosition(-130, 130, 0);
	m_fontGuruguru.SetScale(1.5f);
	m_fontGuruguru.Draw(rc);

	// km
	swprintf_s(buf, L"%d km", m_km);
	m_fontKm.SetText(buf);
	m_fontKm.SetPosition(-50, 0, 0);
	m_fontKm.SetScale(1.5f);
	m_fontKm.Draw(rc);
}