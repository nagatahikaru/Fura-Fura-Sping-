#pragma once
#include "Source/Actor/Character/Character.h"
#include "BatterStateMachine.h"

enum EnAnimationClip {
	enAnimationClip_Idle,
	enAnimationClip_Num
};


namespace {
	namespace BatterNumber {
		enum {
			UniformNumber_1,
			Num
		};
	}

	namespace BasicSettings
	{
		const Vector3 INITIAL_COORDINATE = Vector3(500.0f, 1500.0f, 0.0f);//初期座標
		const float GRAVITY = 50.0f; //重力
		const uint8_t ACCELERATION_TIME = 5;//加速時間
		const float BASICS_SPEED = 400.0f; //基本速度
		const Vector3 VECTOR_UP = Vector3(0.0f, 1.0f, 0.0f); //上方向ベクトル
		const Vector3 INITIAL_SCALE = Vector3(1.5f, 1.5f, 1.5f); //初期スケール
		const Vector3 COLLISION_SCALE = Vector3(50.0f, 35.0f, 50.0f); //当たり判定スケール
	}
};

class Batter :public Character
{
public:
	Batter();
	virtual ~Batter();
	virtual bool Start();
	virtual void Update();

	virtual void Render(RenderContext& rc);
	void SetPlayAnimation(int enAnimationClip);
	const EnAnimationClip GetEnAnimationClip() const
	{
		if (!g_pad[0]->IsPressAnyKey())
		{
			return enAnimationClip_Idle;
		}
		return m_setAnimation;
	}
	const bool GetIsOnGround() const
	{
		return m_characterController.IsOnGround();
	}
	void Move();
	void MoveUpdate();





private:
	std::unique_ptr<BatterStateMachine> m_stateMachine;
	ModelRender m_modelRender[BatterNumber::Num];
	AnimationClip m_animationClips[enAnimationClip_Num];
	EnAnimationClip m_setAnimation = enAnimationClip_Idle;
	int m_UniformNumber = BatterNumber::UniformNumber_1;
};

