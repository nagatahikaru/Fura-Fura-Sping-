#include "stdafx.h"
#include "SoundTestUI.h"
bool SoundTestUI::Start() {
	m_spriteRender.Init("Assets/sprite/SoundTest.DDS", 1920.0f, 1080.0f);
	return true;
}
void SoundTestUI::Update() {
	if (g_pad[0]->IsTrigger(enButtonB)) {
		DeleteGO(this);
	}

}
void SoundTestUI::Render(RenderContext& rc) {
	m_spriteRender.Draw(rc);
}