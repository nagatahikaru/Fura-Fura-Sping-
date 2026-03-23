#include "stdafx.h"
#include "SoundTest.h"
#include"Source/UI/SoundTestUI/SoundTestUI.h"
#include"Source/Scene/Titer/Titer.h"

bool SoundTest::Start()
{
	// UI ‚ð’Ç‰Á
	 m_soundTestUI= NewGO<SoundTestUI>(0,"soundtest");
	return true;
}

void SoundTest::Update()
{

}

void SoundTest::Render(RenderContext& rc)
{

}