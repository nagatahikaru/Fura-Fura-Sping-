#include "stdafx.h"
#include "GameCamera.h"

GameCamera::GameCamera() {

}

bool GameCamera::Start() {
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(5000.0f);
	return true;
}

void GameCamera::Update() {
	//ŒÅ’èƒJƒƒ‰
	m_cameraPos.x = 0.0f;
	m_cameraPos.y = 120.0f;
	m_cameraPos.z = -250.0f;
	
	//’‹“_
	m_target.x = 0.0f;
	m_target.y = 80.0f;
	m_target.z = 0.0f;

	g_camera3D->SetPosition(m_cameraPos);
	g_camera3D->SetTarget(m_target);
}
