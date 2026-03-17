#include "stdafx.h"
#include "InGameUI.h"

InGameUI::InGameUI() {
	m_spriteRender.Init("Assets/sprite/waku.DDS", 600.0f, 400.0f);
	m_spriteRenderBat.Init("Assets/sprite/batto.DDS", 200.0f, 300.0f);
	m_spriteRenderMeet.Init("Assets/sprite/mi-to.DDS", 30.0f, 30.0f);
}

InGameUI::~InGameUI() {

}

bool InGameUI::Start() {
	m_batPosition = Vector3{-50.0f, -100.0f, 0.0f };  // ← 初期位置
	m_meetPosition = Vector3{ 35.0f, 5.0f, 0.0f };
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
	//赤い枠
	m_spriteRender.SetPosition(Vector3{ 0.0f, -100.0f, 0.0f }); 
	m_spriteRender.Update();
	m_spriteRender.Draw(rc);

	//バット
	m_spriteRenderBat.SetPosition(m_batPosition);
	m_spriteRenderBat.SetRotation(m_batRotation);
	m_spriteRenderBat.Update();
	m_spriteRenderBat.Draw(rc);

	// --- ミートゾーン（バットと同じ位置・回転を参照） ---
	m_meetPos = m_batPosition + m_meetPosition;
	m_spriteRenderMeet.SetPosition(m_meetPos);
	m_spriteRenderMeet.SetRotation(m_batRotation);
	m_spriteRenderMeet.Update();
	m_spriteRenderMeet.Draw(rc);

}