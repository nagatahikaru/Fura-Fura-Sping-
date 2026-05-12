#pragma once
#include "Source/Actor/Character/Character.h"

class Ball :public Character
{
	enum BallType
	{
		Straight,
		Curve,
		Slider
	};

	BallType m_ballType = Straight;
	Ball* m_ball;
	
 public:
	Ball();
	virtual ~Ball();
	virtual bool Start();
	virtual void Update();
	virtual void Render(RenderContext& rc);

	//ボールを投げる関数
	void Throw(const Vector3& targetPos);
	void SetPosition(const Vector3& pos);
	Vector3 GetPosition() const { return m_position; }

	bool IsMoving() const;
	bool CheckCollision(const Vector3& pos, float radius);
	//const Vector3& GetPosition() const { return m_position; }
	const Vector3& GetVelocity() const { return m_velocity; }
	bool m_isPaused = true;
	void AnimationUpdate();

	void HitBall(const Vector3& hitDirection, float hitPower);
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }
	void SetVelocity(const Vector3& v) { m_velocity = v; }
	void ResetBall();
	std::vector<Vector3> m_replayPath;
	bool m_isRecording = false;
	bool m_isMove = false;
	bool m_hasHit = false;
 private:
	Vector3 m_position;  //ボールの位置
	Vector3 m_velocity;  //ボールの速度
	Vector3 m_targetPos; //目標位置

	int m_curveDir = 0;
	float m_throwTimer = 0.0f;
	float m_radius = 10.0f;
	float m_baseScale = 50.0f;
	ModelRender m_modelRender;
	bool m_hasReached5500 = false;
	bool m_hasFixed = false;
	Vector3 m_hitStartPos = Vector3::Zero;
	
	CollisionObject* m_collisionObject = nullptr; // 衝突オブジェクト
	bool m_isFlying = false;     // ★ 飛行中フラグ
	bool m_hasLanded = false;    // ★ 着地フラグ
};

