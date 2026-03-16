#include "stdafx.h"
#include "InGameUI.h"

InGameUI::InGameUI() {
	m_spriteRender.Init("Assets/sprite/waku.DDS", 600.0f, 400.0f);
	m_spriteRenderBat.Init("Assets/sprite/batto.DDS", 200.0f, 300.0f);
}

InGameUI::~InGameUI() {

}

bool InGameUI::Start() {
	m_batPosition = Vector3{0.0f, -100.0f, 0.0f };  // ← 初期位置
	return true;
}

void InGameUI::Update() {

}

//バットの位置を設定
void InGameUI::SetBatPosition(const Vector3& pos) {
	m_batPosition = pos;
}

//バットの回転を設定
void InGameUI::SetBatRotation(float rotDeg) {
	m_rad = rotDeg * 3.14159265f / 180.0f;
	m_batRotation.SetRotation(Vector3::AxisZ, m_rad);
}

void InGameUI::Render(RenderContext& rc) {
	m_spriteRender.SetPosition(Vector3{ 0.0f, -100.0f, 0.0f }); 
	m_spriteRender.Update();
	m_spriteRender.Draw(rc);
	m_spriteRenderBat.SetPosition(m_batPosition);
	m_spriteRenderBat.SetRotation(m_batRotation);
	m_spriteRenderBat.Update();
	m_spriteRenderBat.Draw(rc);
}