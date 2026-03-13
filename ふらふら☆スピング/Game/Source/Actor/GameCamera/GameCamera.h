#pragma once
class GameCamera:public IGameObject
{
public:
	GameCamera();
	void Update();
	bool Start();
private:
	Vector3 m_cameraPos;
	float m_angle;
	Vector3 m_target;
};

