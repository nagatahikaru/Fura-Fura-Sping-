#include "stdafx.h"
#include "GameCamera.h"
#include"Source/Scene/InGame/Game.h"
#include"Source/Actor/Character/Ball/Ball.h"
#include "Source/Actor/Character/Catcher/Catcher.h"

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
    m_cameraPos = { -50.0f, 475.0f, 6300.0f };
    m_target = { 0.0f, 300.0f, 0.0f };

    m_yaw = 0.0f;
    m_pitch = -6.0f;

    g_camera3D->SetViewAngle(Math::DegToRad(50.0f));
    m_followMode = Follow_None;
}

void GameCamera::SetkakuteiCamera() {
    m_cameraPos = { 6.5f, 250.0f, 6600.0f };
    m_target = { 0.0f, 1200.0f, 0.0f };

    m_yaw = 0.0f;
    m_pitch = 0.0f;

    g_camera3D->SetViewAngle(Math::DegToRad(50.0f));
    m_followMode = Follow_None;
}

void GameCamera::SetReplayCamera() {
    m_cameraPos = { 1080.0f, 630.0f, -1000.0f };  // 斜め上から
    Vector3 catcherPos = CATCHER::CatcherBasicSettings::INITIAL_COORDINATE;
    m_target = catcherPos;   // ★ 最初からキャッチャー方向を向かせる

    m_yaw = 165.0f;
    m_pitch = 3.0f;

    // ズーム用の状態もここで初期化
    m_replayZoomStartPos = m_cameraPos;
    m_replayZoomStartTarget = m_target;
    m_replayZoomActive = true;
    m_replayZoomTimer = 0.0f;

    // Follow_None を経由せず最初から ReplayZoom にする
    m_followMode = Follow_ReplayZoom;
}

void GameCamera::StartReplayZoomToBall() {
    m_replayZoomStartPos = m_cameraPos;

    Vector3 catcherPos = CATCHER::CatcherBasicSettings::INITIAL_COORDINATE;
    m_target = catcherPos;   // ★ ズレの原因だった固定値をキャッチャーの実座標に変更

    m_replayZoomStartTarget = m_target;
    m_replayZoomActive = true;
    m_replayZoomTimer = 0.0f;
    m_followMode = Follow_ReplayZoom;
}

void GameCamera::SetImpactGlanceCamera() {
    // 現在のカメラ位置は据え置いたまま、ターゲットだけボールに向ける
    // 位置は今のカメラ位置を維持したいので変更しない
    m_followMode = Follow_ImpactGlance;
}

//void GameCamera::SetReplayCamera() {
//    m_cameraPos = { 6.5f, 300.0f, 6500.0f };
//    m_target = { 0.0f, 2000.0f, 0.0f };
//
//    m_yaw = 0.0f;
//    m_pitch = 0.0f;
//
//    g_camera3D->SetViewAngle(Math::DegToRad(50.0f));
//    m_followMode = Follow_None;   // ★ 追加
//}

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
    if (game && game->GetIsPaused()) {
        return;   // ← これでカメラが完全停止
    }

    // ★ 100m超え → カメラ完全停止
    if (m_isFrozen) {
        g_camera3D->SetPosition(m_frozenPos);
        g_camera3D->SetTarget(m_frozenTarget);
        return;   // ← 追尾も回転も一切しない
    }

    //イントロ演出中のyaw/pitchを毎フレーム補間する
    if (m_followMode == Follow_GuruGuruIntro)
    {
        m_guruIntroTimer += g_gameTime->GetFrameDeltaTime();
        float t = Clamp(m_guruIntroTimer / m_guruIntroDuration, 0.0f, 1.0f);

        m_yaw = m_guruIntroStartYaw;
        m_pitch = m_guruIntroStartPitch;

        //ズーム
        float currentZoom = m_guruIntroStartZoom + (m_guruIntroEndZoom - m_guruIntroStartZoom) * t;

        g_camera3D->SetViewAngle(Math::DegToRad(currentZoom));

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

        targetCamPos.y = ballPos.y - 300.0f + distanceFromHome * 0.02f;

        m_cameraPos = LerpVec3(m_cameraPos, targetCamPos, 0.97f);
        m_target = ballPos;
    }
    else if (m_followMode == Follow_ImpactGlance && m_ball != nullptr) {
        // ボールの方向を軽く見るだけ。位置は動かさずターゲットだけ寄せる
        Vector3 ballPos = m_ball->GetPosition();
        m_target = LerpVec3(m_target, ballPos, 0.01f); // ゆっくり視線が寄る
    }
    else if (m_followMode == Follow_ReplayZoom)
    {
        m_replayZoomTimer += g_gameTime->GetFrameDeltaTime();

        if (m_replayZoomTimer < m_replayZoomDuration) {
            Vector3 catcherPos = CATCHER::CatcherBasicSettings::INITIAL_COORDINATE;

            // キャッチャー方向へ最小距離まで寄った位置を目標とする
            Vector3 dirToCatcher = catcherPos - m_replayZoomStartPos;
            float distToCatcher = dirToCatcher.Length();

            float minDistance = 400.0f;
            Vector3 desiredPos = m_replayZoomStartPos;
            if (distToCatcher > minDistance) {
                Vector3 dirNorm = dirToCatcher;
                dirNorm.Normalize();
                desiredPos = catcherPos - dirNorm * minDistance;
            }

            // 最大移動速度でクランプしてカメラ位置を近づける
            Vector3 toDesired = desiredPos - m_cameraPos;
            float distToDesired = toDesired.Length();

            float maxSpeed = m_replayZoomMaxSpeed;  // 1フレームあたりの最大移動量
            if (distToDesired > 0.001f) {
                Vector3 dirNorm = toDesired;
                dirNorm.Normalize();

                float moveAmount = (distToDesired < maxSpeed) ? distToDesired : maxSpeed;
                m_cameraPos = m_cameraPos + dirNorm * moveAmount;
            }

            float zoomProgress = m_replayZoomTimer / m_replayZoomDuration;
            if (zoomProgress > 1.0f) zoomProgress = 1.0f;
            m_target = LerpVec3(m_replayZoomStartTarget, catcherPos, zoomProgress);
        }
    }
    else {
        m_target = m_cameraPos - m_forward * 100.0f;
    }
    float minCameraHeight = 300.0f;
    if (m_followMode != Follow_None && m_cameraPos.y < minCameraHeight) {
        m_cameraPos.y = minCameraHeight;
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

void GameCamera::StartGuruGuruIntroCamera()
{
    //カメラの位置はキャッチャーカメラと同じ場所に固定(位置は動かさない)
    m_cameraPos = { -50.0f,475.0f,6300.0f };

    //開始角度:斜め右下を見る
    m_guruIntroStartYaw = 25.0f; //右方向の角度
    m_guruIntroStartPitch = -20.0;    //下方向の角度

    m_guruIntroEndYaw = 0.0f;
    m_guruIntroEndPitch = -6.0f;

    //ズーム開始・終了
    m_guruIntroStartZoom = 50.0f;
    m_guruIntroEndZoom = 35.0f;
    m_yaw = m_guruIntroStartYaw;
    m_pitch = m_guruIntroStartPitch;

    m_guruIntroTimer = 0.0f;
    m_guruIntroDuration = 5.0f; //ぐるぐる時間に合わせる

    g_camera3D->SetViewAngle(Math::DegToRad(m_guruIntroStartZoom));
    m_followMode = Follow_GuruGuruIntro;
}

void GameCamera::Render(RenderContext& rc) {

}