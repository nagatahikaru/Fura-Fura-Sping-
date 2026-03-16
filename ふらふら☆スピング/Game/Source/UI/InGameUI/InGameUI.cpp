#include "stdafx.h"
#include "InGameUI.h"

InGameUI::InGameUI() {
	m_spriteRender.Init("Assets/sprite/waku.DDS", 600.0f, 400.0f);
}

InGameUI::~InGameUI() {

}

bool InGameUI::Start() {
	return true;
}

void InGameUI::Update() {

}

void InGameUI::Render(RenderContext& rc) {
	m_spriteRender.SetPosition(Vector3{ 0.0f, -100.0f, 0.0f }); // âÊñ ç∂è„
	m_spriteRender.Update();
	m_spriteRender.Draw(rc);
}