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

void GameCamera::Update() {


    // ★ ポーズ中はカメラを止める
    Game* game = FindGO<Game>("game");
    if (game && game->m_isPaused) {
        return;   // ← これでカメラが完全停止
    }

  // --- ASWD で移動 ---

    //if (g_pad[0]->IsPress(enButtonB)) {
    //    m_cameraPos.z += m_moveSpeed;
    //    m_target.z += m_moveSpeed;
    //}
    //if (g_pad[0]->IsPress(enButtonY)) {
    //    m_cameraPos.z -= m_moveSpeed;
    //    m_target.z -= m_moveSpeed;
    //}
    //if (g_pad[0]->IsPress(enButtonA)) {
    //    m_cameraPos.x -= m_moveSpeed;
    //    m_target.x -= m_moveSpeed;
    //}
    //if (g_pad[0]->IsPress(enButtonX)) {
    //    m_cameraPos.x += m_moveSpeed;
    //    m_target.x += m_moveSpeed;
    //}

    //// --- 上下移動（Up：上昇、Down：下降） ---
    //if (g_pad[0]->IsPress(enButtonRB1)) {
    //    m_cameraPos.y += m_moveSpeed;
    //    m_target.y += m_moveSpeed;
    //}
    //if (g_pad[0]->IsPress(enButtonRB3)) {
    //    m_cameraPos.y -= m_moveSpeed;
    //    m_target.y -= m_moveSpeed;
    //}

    //// --- 視点の上下回転（Up：上を見る、Down：下を見る） ---
    //if (g_pad[0]->IsPress(enButtonUp)) {
    //    m_pitch += m_rotSpeed;
    //}
    //if (g_pad[0]->IsPress(enButtonDown)) {
    //    m_pitch -= m_rotSpeed;
    //}

    //// --- 回転（Q：左回転、E：右回転） ---
    //if (g_pad[0]->IsPress(enButtonLeft)) {
    //    m_yaw -= m_rotSpeed;
    //}

    //// --- 回転（Q：左回転、E：右回転） ---
    //if (g_pad[0]->IsPress(enButtonRight)) {
    //    m_yaw += m_rotSpeed;
    //}

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

        // ボールの速度
        float ballSpeed = m_ball->GetVelocity().Length();

        // ボールとホームベースの距離（Z軸方向を想定）
        float distanceFromHome = (ballPos - Vector3(0, 0, 0)).Length();

        // 速度ベースのズーム
        float zoomBySpeed = 600.0f + ballSpeed * 3.0f;

        // 距離ベースのズーム（遠くへ飛ぶほどズームアウト）
        float zoomByDistance = 600.0f + distanceFromHome * 0.2f;

        // 2つをミックス（自然なズーム）
        float followDistance = Clamp((zoomBySpeed + zoomByDistance) * 0.5f, 150.0f, 800.0f);

        Vector3 fixedDir = Vector3(0, 0, 1);
        Vector3 targetCamPos = ballPos + fixedDir * followDistance;

        // 高さ調整
       // 高さ調整（距離に応じて上げる）
        targetCamPos.y = ballPos.y + 20.0f + distanceFromHome * 0.02f;

        // スムーズ追尾
        m_cameraPos = LerpVec3(m_cameraPos, targetCamPos, 1.0f);

        // ボールを見る
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

void GameCamera::Render(RenderContext& rc) {
    //wchar_t posText[64];
    //swprintf(posText, 64, L"CamPos: X=%.1f Y=%.1f Z=%.1f\nYaw=%.1f Pitch=%.1f",
    //    m_cameraPos.x, m_cameraPos.y, m_cameraPos.z, m_yaw, m_pitch);

    //m_fontrender.SetText(posText);
    //m_fontrender.SetPosition({ 100.0f, 400.0f, 0.0f });
    //m_fontrender.SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    //m_fontrender.Draw(rc);
}