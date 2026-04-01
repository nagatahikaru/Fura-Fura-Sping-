#pragma once
#include "Source/Actor/Character/Character.h"
#include"Source/Scene/InGame/Game.h"
#include "Source/Actor/Character/Ball/Ball.h"

namespace {
	namespace CatcherNumber {
		enum {
			UniformNumber_0,
			UniformNumber_1,
			UniformNumber_2,
			UniformNumber_3,
			UniformNumber_4,
			UniformNumber_5,
			UniformNumber_6,
			UniformNumber_7,
			UniformNumber_8,
			UniformNumber_9,
			Num
		};
	}

	namespace CatcherBasicSettings
	{
		const Vector3 INITIAL_COORDINATE = Vector3(0.0f, -50.0f, 6000.0f);//初期座標
		const float GRAVITY = 50.0f; //重力
		const uint8_t ACCELERATION_TIME = 5;//加速時間
		const float BASICS_SPEED = 400.0f; //基本速度
		const Vector3 VECTOR_UP = Vector3(0.0f, 1.0f, 0.0f); //上方向ベクトル
		const Vector3 INITIAL_SCALE = Vector3(10.0f, 10.0f, 10.0f); //初期スケール
		const Vector3 COLLISION_SCALE = Vector3(50.0f, 35.0f, 50.0f); //当たり判定スケール
		const float NONE_SPEED = 0.0f;//速度なし
	}
};


class Catcher :public Character
{
public:
	Catcher() {};
	virtual ~Catcher();
	virtual bool Start();
	virtual void Update();
	void PlayeAnimation(int animationNo);
	void Catch();
	void AnimationUpdate()
	{
		
		m_modelRender.Update();
	}
	virtual void Render(RenderContext& rc);

	bool m_isPaused = false; // アニメーションの一時停止フラグ
private:
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Num
	};
	ModelRender m_modelRender;
	Ball* m_ball = nullptr;
	Game* m_game = nullptr;
	AnimationClip m_animationClips[enAnimationClip_Num];
	int m_UniformNumber = CatcherNumber::UniformNumber_1;
	CharacterController m_characterController;
	CollisionObject* m_collisionObject = nullptr; // 衝突オブジェクト
};

