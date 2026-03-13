#pragma once
class GameCamera:public IGameObject
{
public:
	GameCamera();
	void Update();
	bool Start();
private:
	/*Vector3 m_cameraPos;
	float m_angle;*/
	//Vector3 m_target;
	Vector3 m_cameraPos;
	Vector3 m_target = { 0,80,0 };
	Quaternion m_rotYaw;
	Quaternion m_rotPitch;
	Quaternion m_rot;
	Vector3 m_forward;
	float m_moveSpeed = 10.0f;
	float m_yaw = 0.0f;   // ç∂âEâÒì]äpìx
	float m_rotSpeed = 1.0f;
	float m_pitch = 0.0f;   // è„â∫âÒì]äpìx
};

