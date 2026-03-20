#include "stdafx.h"
#include "SoundTest.h"
#include"Source/UI/SoundTestUI/SoundTestUI.h"
#include"Source/Scene/Titer/Titer.h"

bool SoundTest::Start()
{
	// UI ‚ð’Ç‰Á
	 m_soundTestUI= NewGO<SoundTestUI>(0);
	return true;
}

void SoundTest::Update()
{
	if (g_pad[0]->IsTrigger(enButtonB)) {
		NewGO<Titer>(0);
		DeleteGO(this);
	}
}

void SoundTest::Render(RenderContext& rc)
{

}