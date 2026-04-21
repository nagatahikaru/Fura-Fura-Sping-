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
    m_cameraPos = { 1080.0f, 630.0f, 0.0f };  // 斜め上から
    m_target = { 0.0f, 300.0f, 0.0f };        // 固定ターゲット（例）
    m_yaw = 165.0f;
    m_pitch = 3.0f;
    m_followMode = Follow_None;   // ★ 追加
}

void GameCamera::SetFollowBallCamera() {
    m_cameraPos = { -3650.0f, 2580.0, 6050.0f }; // ボール後方
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

void GameCamera::Update() {


    // ★ ポーズ中はカメラを止める
    Game* game = FindGO<Game>("game");
    if (game && game->m_isPaused) {
        return;   // ← これでカメラが完全停止
    }

    // ★ 100m超え → カメラ完全停止
    if (m_isFrozen) {
        g_camera3D->SetPosition(m_frozenPos);
        g_camera3D->SetTarget(m_frozenTarget);
        return;   // ← 追尾も回転も一切しない
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

        Vector3 ballPos = m_ball->GetPosition();

        // ボールの進行方向
        Vector3 dir = m_ball->GetVelocity();
        if (dir.LengthSq() > 0.001f) {
            dir.Normalize();
        }
        else {
            dir = Vector3(0, 0, 1);
        }

        float ballSpeed = m_ball->GetVelocity().Length();
        float distanceFromHome = (ballPos - Vector3(0, 0, 0)).Length();

        float zoomBySpeed = 600.0f + ballSpeed * 3.0f;
        float zoomByDistance = 600.0f + distanceFromHome * 0.2f;

        float followDistance = Clamp((zoomBySpeed + zoomByDistance) * 0.5f, 150.0f, 1000.0f);

        Vector3 fixedDir = Vector3(0, 0, 1);
        Vector3 targetCamPos = ballPos + fixedDir * followDistance;

        targetCamPos.y = ballPos.y - 200.0f + distanceFromHome * 0.02f;

        m_cameraPos = LerpVec3(m_cameraPos, targetCamPos, 1.0f);
        m_target = ballPos;
    }
    else if (m_followMode == Follow_Side && m_ball != nullptr) {

        // 横カメラは固定向き
        m_target = m_ball->GetPosition();
    }
    else {
        m_target = m_cameraPos - m_forward * 100.0f;
    }

    // --- カメラ反映 ---
    g_camera3D->SetPosition(m_cameraPos);
    g_camera3D->SetTarget(m_target);
}

void GameCamera::StartHitMomentCamera()
{
    m_isHitMoment = true;
    m_hitMomentTimer = 3.5f;
}

void GameCamera::FreezeCamera()
{
    m_followMode = Follow_None;

    // ★ 現在のカメラ位置とターゲットを保存
    m_frozenPos = m_cameraPos;
    m_frozenTarget = m_target;

    m_isFrozen = true;
}

// GameCamera.cpp
void GameCamera::UnfreezeCamera()
{
    m_isFrozen = false;
}

void GameCamera::Render(RenderContext& rc) {
  
}