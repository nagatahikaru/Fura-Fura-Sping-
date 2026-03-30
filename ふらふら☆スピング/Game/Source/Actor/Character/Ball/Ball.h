#pragma once
#include "Source/Actor/Character/Character.h"

class Ball :public Character
{
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

	void HitBall(const Vector3& hitDirection, float hitPower);
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }

private:
	Vector3 m_position;  //ボールの位置
	Vector3 m_velocity;  //ボールの速度
	Vector3 m_targetPos; //目標位置

	bool m_isMove = false; 
	float m_throwTimer = 0.0f;
	float m_radius = 10.0f;
	float m_baseScale = 50.0f;
	ModelRender m_modelRender;
	bool m_hasReached5500 = false;
	bool m_hasFixed = false;

	CollisionObject* m_collisionObject = nullptr; // 衝突オブジェクト
};

