#include "stdafx.h"
#include "GameCamera.h"

GameCamera::GameCamera() {

}

bool GameCamera::Start() {
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(100000.0f);

    m_cameraPos.Set( 0,250,6900 );

    // ★ 初期位置を反映
    g_camera3D->SetPosition(m_cameraPos);
    g_camera3D->SetTarget(m_target);
	return true;
}

void GameCamera::Update() {

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

    // --- target 更新 ---
    m_target = m_cameraPos - m_forward * 100.0f;

    // --- カメラ反映 ---
    g_camera3D->SetPosition(m_cameraPos);
    g_camera3D->SetTarget(m_target);
}
