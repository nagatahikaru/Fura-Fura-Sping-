#include "stdafx.h"
#include "Result.h"
#include"Source/Scene/Titer/Titer.h"

bool Result::Start()
{
	m_spriteRender.Init("Assets/sprite/Result.dds", 1920.0f, 1080.0f);
	return true;
}

void Result::Update()
{
	if (g_pad[0]->IsTrigger(enButtonY)) {
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

	wchar_t buf[256];
	// ‚®‚é‚®‚é
	swprintf_s(buf, L"‚®‚é‚®‚é: %d", m_guruguru);
	m_fontGuruguru.SetText(buf);
	m_fontGuruguru.SetPosition(-600, 200, 0);
	m_fontGuruguru.Draw(rc);

	// km
	swprintf_s(buf, L"%d km", m_km);
	m_fontKm.SetText(buf);
	m_fontKm.SetPosition(-600, 100, 0);
	m_fontKm.Draw(rc);
}