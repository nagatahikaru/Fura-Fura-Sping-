#pragma once
#include "Source/Actor/Character/Character.h"
#include "Source/Difficulty.h"

class Ball :public Character
{
private:
	Vector3 m_position;  //ボールの位置
	Vector3 m_velocity;  //ボールの速度
	Vector3 m_throwStartPos = Vector3::Zero;
	Vector3 m_throwEndPos = Vector3::Zero;
	Vector3 m_prevPosition;

	//Vector3 m_rotation;

	float m_rotateSpeed = 7660.0f;
	float m_rotationAngle = 0.0f;
	float m_currentRotationSpeed = 2880.0f;
	float m_speedRate = 1.0f;
	float m_rotationSpeed = 0.0f;
	float m_curveDir = 0;
	float m_throwTimer = 0.0f;
	float m_radius = 10.0f;
	float m_baseScale = 50.0f;
	float m_baseGravity = 0.0f; // 元々のベース重力
	ModelRender m_modelRender;
	bool m_hasReached5500 = false;
	bool m_hasFixed = false;
	Vector3 m_hitStartPos = Vector3::Zero;

	CollisionObject* m_collisionObject = nullptr; // 衝突オブジェクト
	bool m_isFlying = false;     // ★ 飛行中フラグ
	bool m_hasLanded = false;    // ★ 着地フラグ
	bool m_hasStrike = false;
	bool m_hasShownPrediction = false;
	float m_storedPredictedDistance = 0.0f;
	bool m_hasPlayedSE6 = false;
	float m_replayTimer = 0.0f;
	bool m_isMagicBall = false;// 魔球かどうかのフラグ
	bool m_hasPlayedDisappearEffect = false;  // 魔球が消える瞬間のエフェクト再生済みフラグ
	bool m_hasPlayedReappearEffect = false;   // 魔球が再出現した瞬間のエフェクト再生済みフラグ
	float m_replayVelY = 0.0f;
	float m_initialSpeedZ = 0.0f; // 投げた瞬間の初期Z速度を保存する変数
	// スライダーの曲がり方を調整するパラメータ
	float m_breakAmount = 80.0f;      // 最大横移動量（大きいほど鋭く曲がる）
	float m_breakStartRatio = 0.55f;  // 投球の何割地点から曲がり始めるか(0〜1)
	float m_outwardAmount = 0.0f;     // 外側への揺れ量
	float m_baseBallSpeed = 0.0f; // ボールの基本速度
	float m_baseBallGravity = 0.0f; // ボールの基本重力

	std::vector<Vector3> m_replayPath;
	bool m_isRecording = false;
	bool m_isMove = false;
	bool m_hasHit = false;
	bool m_isPaused = true;
	bool m_isDifficultyConfigured = false;// 難易度設定済みかどうかのフラグ

	float m_dropStartRatioOverride;// 投球の何割地点から落下し始めるか(0〜1)���上書きする変数

	enum BallType
	{
		Straight,        //真っ直ぐ
		Curve,           //カーブ
		ShakeHorizontal, //横揺れ
		ShakeVertical,   //縦揺れ
		SlowBall
	};

	BallType m_ballType = Straight;
	Ball* m_ball;

public:
	Ball();
	virtual ~Ball();
	virtual bool Start();
	virtual void Update();
	virtual void Render(RenderContext& rc);
	bool IsHit() const { return m_hasHit; }
	//ボールを投げる関数
	void Throw(const Vector3& targetPos);
	void SetPosition(const Vector3& pos);
	Vector3 GetPosition() const { return m_position; }
	void GetFlightRay(Vector3& startPos, Vector3& endPos) const;
	Vector3 GetFlightDirection() const;
	float GetFlightLength() const
	{
		return (m_throwEndPos - m_throwStartPos).Length();
	}
	bool CheckCollision(const Vector3& pos, float radius);
	//const Vector3& GetPosition() const { return m_position; }
	const Vector3& GetVelocity() const { return m_velocity; }

	void AnimationUpdate();

	void HitBall(const Vector3& hitDirection, float hitPower);
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }
	void SetVelocity(const Vector3& v) { m_velocity = v; }
	void ResetBall();
	float PredictLandingDistance();
	void ResetThrowTimer()
	{
		m_throwTimer = 0.0f;
	}

	Vector3 GetPrevPosition() const
	{
		return m_prevPosition;
	}

	void Slider(float dt);
	void SetHasHit(bool hasHit) { m_hasHit = hasHit; }
	bool GetHasHit() const { return m_hasHit; }
	void SetIsMagicBall(bool isMagicBall) { m_isMagicBall = isMagicBall; }
	bool GetIsMagicBall() const { return m_isMagicBall; }
	void SetIsMove(bool isMove) { m_isMove = isMove; }
	bool GetIsMove() const { return m_isMove; }
	void SetReplayPath(const std::vector<Vector3>& path) { m_replayPath = path; }
	std::vector<Vector3> GetReplayPath() const { return m_replayPath; }
	void SetIsRecording(bool isRecording) { m_isRecording = isRecording; }
	bool GetIsRecording() const { return m_isRecording; }
	void SetIsPaused(bool isPaused) { m_isPaused = isPaused; }
	bool GetIsPaused() const { return m_isPaused; }

	void SelectBallType(Difficulty diff);

};

