#pragma once
class Ball;
enum FollowMode {
	Follow_None,   // 追尾しない（横カメラ）
	Follow_Back,   // 後ろから追尾
	Follow_Side    // 横から見る（固定）
};
class GameCamera:public IGameObject
{
public:
	GameCamera();
	void Update();
	bool Start();
	void Render(RenderContext& rc);
	void SetCatcherCamera();
	void SetReplayCamera();
	void SetFollowBallCamera();
	void SetFollowBallBackCamera();
	// ★ ボールをセットする関数
	void SetBall(Ball* ball) { m_ball = ball; }
	void StartHitMomentCamera();
	void FreezeCamera();
	void UnfreezeCamera();   // ★ 追加
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
	float m_yaw = 0.0f;   // 左右回転角度
	float m_rotSpeed = 1.0f;
	float m_pitch = 0.0f;   // 上下回転角度
	FontRender m_fontrender;
	float m_viewAngle = Math::DegToRad(0.0f);
	// ★★★ これが無いからエラーになっていた ★★★
	Ball* m_ball = nullptr;        // ボールへのポインタ
	bool m_isFollowBall = false;   // 追尾モードフラグ
	FollowMode m_followMode = Follow_None;
	bool m_isHitMoment = false;
	float m_hitMomentTimer = 0.0f;
	Vector3 m_frozenPos = Vector3::Zero;
	Vector3 m_frozenTarget = Vector3::Zero;
	bool m_isFrozen = false;
};

