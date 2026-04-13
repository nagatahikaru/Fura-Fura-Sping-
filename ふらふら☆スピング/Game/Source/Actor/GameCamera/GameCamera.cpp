#include "stdafx.h"
#include "GameCamera.h"
#include"Source/Scene/InGame/Game.h"
#include"Source/Actor/Character/Ball/Ball.h"


inline Vector3 LerpVec3(const Vector3& a, const Vector3& b, float t)
{
    return a + (b - a) * t;
}

inline float Clamp(float v, float minV, float maxV)
{
    if (v < minV) return minV;
    if (v > maxV) return maxV;
    return v;
}

GameCamera::GameCamera() {

}

bool GameCamera::Start() {
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(100000.0f);

    SetCatcherCamera();
	return true;
}

void GameCamera::SetCatcherCamera() {
    m_cameraPos = { 10.0f, 450.0f, 6600.0f };
    m_target = { 0.0f, 300.0f, 0.0f };

    m_yaw = 0.0f;  
    m_pitch = 0.0f;   

    g_camera3D->SetViewAngle(Math::DegToRad(50.0f));
    m_followMode = Follow_None;   // ★ 追加
}

void GameCamera::SetReplayCamera() {
    m_cameraPos = { 1080.0f, 630.0f, 1470.0f };  // 斜め上から
    m_target = { 0.0f, 300.0f, 0.0f };        // 固定ターゲット（例）
    m_yaw = 165.0f;
    m_pitch = 3.0f;
    m_followMode = Follow_None;   // ★ 追加
}

void GameCamera::SetFollowBallCamera() {
    m_cameraPos = {-3650.0f, 2580.0, 6050.0f}; // ボール後方
    m_target = { 0.0f, 300.0f, 0.0f };
    m_yaw = -55.0f;
    m_pitch = -53.0f;
    g_camera3D->SetViewAngle(Math::DegToRad(15.0f));
    m_followMode = Follow_Side;   // ← 横追尾
}

void GameCamera::SetFollowBallBackCamera() {
    m_cameraPos = { 0.0f, 450.0f, 4600.0f };
    m_target = { 0.0f, 300.0f, 0.0f };

    m_yaw = 0.0f;
    m_pitch = 0.0f;

    g_camera3D->SetViewAngle(Math::DegToRad(50.0f));
    m_followMode = Follow_Back;   // ← 後ろ追尾
}

void GameCamera::StartHitMomentCamera()
{
    m_isHitMoment = true;
    m_hitMomentTimer = 3.5f;   // ← 1秒間だけ“打った瞬間カメラ”を使う
}

void GameCamera::Update() {


    // ★ ポーズ中はカメラを止める
    Game* game = FindGO<Game>("game");
    if (game && game->m_isPaused) {
        return;   // ← これでカメラが完全停止
    }

    // --- 回転（Yaw + Pitch） ---
    m_rotYaw.SetRotationDeg(Vector3::AxisY, m_yaw);

    m_rotPitch.SetRotationDeg(Vector3::AxisX, m_pitch);

    // 合成（Pitch → Yaw）
    m_rot = m_rotPitch * m_rotYaw;

    // --- forward 計算 ---
    m_forward = Vector3::AxisZ;
    m_rot.Apply(m_forward);
    if (m_followMode == Follow_Back && m_ball != nullptr) {

        if (m_isHitMoment) {
            // ★ 打った瞬間だけこの式（fixedDir = -1）
            Vector3 ballPos = m_ball->GetPosition();

            Vector3 dir = m_ball->GetVelocity();
            if (dir.LengthSq() > 0.001f) dir.Normalize();
            else dir = Vector3(0, 0, 1);

            float ballSpeed = m_ball->GetVelocity().Length();
            float distanceFromHome = (ballPos - Vector3(0, 0, 0)).Length();

            float zoomBySpeed = 600.0f + ballSpeed * 3.0f;
            float zoomByDistance = 600.0f + distanceFromHome * 0.5f;

            float followDistance = Clamp((zoomBySpeed + zoomByDistance) * 0.5f, 600.0f, 2000.0f);

            Vector3 fixedDir = Vector3(0, 0, -1);
            Vector3 targetCamPos = ballPos + fixedDir * followDistance;

            targetCamPos.y = ballPos.y + 50.0f + distanceFromHome * 0.1f;

            m_cameraPos = LerpVec3(m_cameraPos, targetCamPos, 0.85f);
            m_target = ballPos;
        }
        else {
            // ★ 1秒後はこの式（あなたが最後に貼った式）
            Vector3 ballPos = m_ball->GetPosition();

            float ballSpeed = m_ball->GetVelocity().Length();
            float distanceFromHome = (ballPos - Vector3(0, 0, 0)).Length();

            float zoomBySpeed = 600.0f + ballSpeed * 3.0f;
            float zoomByDistance = 600.0f + distanceFromHome * 0.2f;

            float followDistance = Clamp((zoomBySpeed + zoomByDistance) * 0.5f, 150.0f, 1000.0f);

            Vector3 fixedDir = Vector3(0, 0, 1);
            Vector3 targetCamPos = ballPos + fixedDir * followDistance;

            targetCamPos.y = ballPos.y + 20.0f + distanceFromHome * 0.02f;

            m_cameraPos = LerpVec3(m_cameraPos, targetCamPos, 1.0f);
            m_target = ballPos;
        }
    }
    else if (m_followMode == Follow_Side && m_ball != nullptr) {

        // 横カメラは固定向き
        m_target = m_ball->GetPosition();
    }
    else {
        m_target = m_cameraPos - m_forward * 100.0f;
    }

    // --- 打撃直後カメラタイマー ---
    if (m_isHitMoment) {
        m_hitMomentTimer -= g_gameTime->GetFrameDeltaTime();
        if (m_hitMomentTimer <= 0.0f) {
            m_isHitMoment = false;
        }
    }

    // --- カメラ反映 ---
    g_camera3D->SetPosition(m_cameraPos);
    g_camera3D->SetTarget(m_target);
}

void GameCamera::Render(RenderContext& rc) {
    //wchar_t posText[64];
    //swprintf(posText, 64, L"CamPos: X=%.1f Y=%.1f Z=%.1f\nYaw=%.1f Pitch=%.1f",
    //    m_cameraPos.x, m_cameraPos.y, m_cameraPos.z, m_yaw, m_pitch);

    //m_fontrender.SetText(posText);
    //m_fontrender.SetPosition({ 100.0f, 400.0f, 0.0f });
    //m_fontrender.SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    //m_fontrender.Draw(rc);
}