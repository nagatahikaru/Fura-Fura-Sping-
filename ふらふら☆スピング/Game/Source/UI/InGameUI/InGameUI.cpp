#include "stdafx.h"
#include "InGameUI.h"

InGameUI::InGameUI() {
	m_spriteRender.Init("Assets/sprite/waku.DDS", 850.0f, 600.0f);
	m_spriteRenderBat.Init("Assets/sprite/batto.DDS", 330.0f, 430.0f);
	m_spriteRenderMeet.Init("Assets/sprite/mi-to.DDS", 45.0f, 45.0f);
	m_spriteRenderReplay.Init("Assets/sprite/REPLAY.DDS", 300.0f, 300.0f);
	m_spriteRenderBall.Init("Assets/sprite/ball.DDS", 30.0f, 30.0f);
}

InGameUI::~InGameUI() {

}

bool InGameUI::Start() {
	m_batPositionRight = Vector3{-50.0f, -100.0f, 0.0f };  // ← 初期位置
	m_batPositionLeft = Vector3{ 50.0f,-100.0f,0.0f };
	m_meetPositionRight = Vector3{ 35.0f, 5.0f, 0.0f };
	m_meetPositionLeft = Vector3{ -70.0f,7.0f,0.0f };
	return true;
}

void InGameUI::Update() {

}

//バットの位置を設定
void InGameUI::SetBatPosition(const Vector3& pos) {
	m_batPositionRight = pos;
}

//バットの回転を設定
void InGameUI::SetBatRotation(float rotDeg) {
	m_rad = rotDeg * 3.14159265f / 180.0f;
	m_batRotation.SetRotation(Vector3::AxisZ, m_rad);
}

void InGameUI::SetBatterSide(bool isLeft) {
	m_isLeftBatter = isLeft;
}

void InGameUI::SetPause(bool isPaused) {
	m_isPaused = isPaused;
}

void InGameUI::SetPredictedBallPos(const Vector3& pos3D) {

	// ★ 固定中は上書き禁止！
	if (m_isBallUIFixed) {
		return;
	}

	// ★ 後ろに完全に飛んだときだけエラー（誤差 20 を無視）
	if (pos3D.z > m_startZ + 20.0f) {
		m_isError = true;
	}

	m_predictedBallPos3D = pos3D;
	m_hasPredictedBall = true;
}

Vector3 InGameUI::ConvertBall3DToUI(const Vector3& ballPos3D)
{
	// --- X方向（左右） ---
	float uiX = ballPos3D.x * 0.20f;

	// --- Y方向（高さ） ---
	// 3Dの初期高さ 750 → UI の初期位置 -80 に合わせる
	float uiY = (ballPos3D.y - 750.0f) * 0.15f + (-80.0f);

	return Vector3{ uiX, uiY, 0.0f };
}

void InGameUI::FixBallUI(const Vector3& pos3D)
{
	m_isBallUIFixed = true;
	m_fixedBallUIPos = pos3D;
}

void InGameUI::SetMeetCursorPosition(Vector3 m_inputOffset)
{
	
	m_batPos = m_isLeftBatter ? m_batPositionLeft : m_batPositionRight;
	Vector3 meetOffset = m_isLeftBatter ? m_meetPositionLeft : m_meetPositionRight;

	Vector3 baseMeetPos = m_batPos + meetOffset;

	m_meetPos = baseMeetPos + m_inputOffset;
	m_batPos = m_meetPos - meetOffset;
}

void InGameUI::SetStartZ(float z) {
	m_startZ = z;
	m_isError = false;   // 打つたびにリセット
}

void InGameUI::Render(RenderContext& rc) {

	if (m_isPaused) {
		return; // ← ポーズ中は UI を一切描画しない
	}

	if (m_isUIVisible) {

		//赤い枠
		m_spriteRender.SetPosition(Vector3{ 0.0f, -80.0f, 0.0f });
		m_spriteRender.Update();
		m_spriteRender.Draw(rc);

		// --- 左右でバット位置を切り替える ---
		Vector3 batPos = m_isLeftBatter ? m_batPositionLeft : m_batPositionRight;
		Vector3 meetOffset = m_isLeftBatter ? m_meetPositionLeft : m_meetPositionRight;

		//バット
		batScaleX = m_isLeftBatter?-1.0f:1.0f;
		m_spriteRenderBat.SetPosition(m_batPos);
		m_spriteRenderBat.SetRotation(m_batRotation);
		m_spriteRenderBat.SetScale(Vector3{ batScaleX,1.0f,1.0f });
		m_spriteRenderBat.Update();
		m_spriteRenderBat.Draw(rc);

		// --- ミートゾーン（バットと同じ位置・回転を参照） ---	
		m_meetScaleX = m_isLeftBatter ? -1.0f : 1.0f;
		m_spriteRenderMeet.SetPosition(m_meetPos);
		m_spriteRenderMeet.SetRotation(m_batRotation);
		m_spriteRenderMeet.SetScale(Vector3{ m_meetScaleX, 1.0f, 1.0f });
		m_spriteRenderMeet.Update();
		m_spriteRenderMeet.Draw(rc);

		if (m_hasPredictedBall) {

			Vector3 uiPos;

			if (m_isBallUIFixed) {
				// ★ 固定モード：変換しない、動かさない
				uiPos = ConvertBall3DToUI(m_fixedBallUIPos);
			}
			else {
				// ★ 通常モード：リアルタイムで動く
				uiPos = ConvertBall3DToUI(m_predictedBallPos3D);
			}

			m_spriteRenderBall.SetPosition(uiPos);
			m_spriteRenderBall.Update();
			m_spriteRenderBall.Draw(rc);
		}
	}

	if (m_isFontVisible) {
		wchar_t text[256];
		swprintf_s(text,256, L"ぐるぐる:%d", m_guruGuruCount);
		m_fontRender.SetText(text);
		m_fontRender.SetPosition(-900.0f, 500.0f, 0.0f);
		m_fontRender.SetColor(0.0f, 0.78f, 0.31f, 1.0f);
		m_fontRender.Draw(rc);

		wchar_t boll[256];

		if (m_isError) {
			swprintf_s(boll, L"???m");
		}
		else {
			swprintf_s(boll, L"%3dm", (int)m_km);
		}
		m_fontBollRender.SetText(boll);
		m_fontBollRender.SetPosition(800.0f, 500.0f, 0.0f);
		m_fontBollRender.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_fontBollRender.Draw(rc);
	}

	if (m_isReplayVisible) {
		m_spriteRenderReplay.SetPosition(Vector3{ -800.0f, 450.0f, 0.0f });
		m_spriteRenderReplay.Update();
		m_spriteRenderReplay.Draw(rc);
	}

}