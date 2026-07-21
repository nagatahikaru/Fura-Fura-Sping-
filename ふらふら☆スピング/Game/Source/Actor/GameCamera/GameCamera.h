#pragma once
class Ball;
class  Catcher;
enum FollowMode {
	Follow_None,   // 追尾しない（横カメラ）
	Follow_Back,   // 後ろから追尾
	Follow_Side,    // 横から見る（固定）
	Follow_ImpactGlance,
	Follow_ReplayZoom
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
	void SetkakuteiCamera();
	void SetImpactGlanceCamera();
	// ★ ボールをセットする関数
	void SetBall(Ball* ball) { m_ball = ball; }
	void SetCatcher(Catcher* catcher) { m_catcher = catcher; }
	void StartHitMomentCamera();
	void FreezeCamera();
	void UnfreezeCamera();   
	void StartReplayZoomToBall();
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
	Catcher* m_catcher = nullptr;
	bool m_isFollowBall = false;   // 追尾モードフラグ
	FollowMode m_followMode = Follow_None;
	bool m_isHitMoment = false;
	float m_hitMomentTimer = 0.0f;
	Vector3 m_frozenPos = Vector3::Zero;
	Vector3 m_frozenTarget = Vector3::Zero;
	bool m_isFrozen = false;
	Vector3 m_replayZoomStartPos;   // ズーム開始時のカメラ位置
	bool    m_replayZoomActive = false;
	float   m_replayZoomTimer = 0.0f;
	float   m_replayZoomDuration = 5.0f;
	Vector3 m_replayZoomStartTarget;   // ★ ズーム開始時点のtarget保存用

};

