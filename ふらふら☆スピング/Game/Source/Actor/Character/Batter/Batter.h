#pragma once
#include "Source/Actor/Character/Character.h"
#include "BatterStateMachine.h"

namespace {
	namespace BatterNumber {
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

	namespace BatterBasicSettings
	{
		const Vector3 INITIAL_COORDINATE = Vector3(-420.0f, -50.0f, 5500.0f);//初期座標
		const float BASICS_SPEED = 400.0f; //基本速度
		const Vector3 VECTOR_UP = Vector3(0.0f, 1.0f, 0.0f); //上方向ベクトル
		const Vector3 INITIAL_SCALE = Vector3(10.0f, 10.0f, 10.0f); //初期スケール
		const Vector3 COLLISION_SCALE = Vector3(50.0f, 35.0f, 50.0f); //当たり判定スケール
		const float NONE_SPEED = 0.0f;//速度なし
	}

	namespace BatBasicSettings
	{
		const Vector3 INITIAL_SCALE = Vector3(10.0f, 10.0f, 10.0f); //初期スケール
		const Vector3 COLLISION_SCALE = Vector3(50.0f, 35.0f, 50.0f); //当たり判定スケール
		const Vector3 ROTATION_OFFSET = Vector3(1.0f, 0.0f, 0.0f); //回転の軸となるオフセット座標
		const float ROTATION_ANGLE = 90.0f; //回転角度
	}
};

class Batter :public Character
{
public:
	// グローバルでの定義の重複を避けるため、クラス内にEnumを移動（もしくは Character.h で定義されている場合は削除してください）
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Swing,
		enAnimationClip_Num
	};

	Batter();
	virtual ~Batter();
	virtual bool Start();
	virtual void Update();

	virtual void Render(RenderContext& rc);
	void SetPlayAnimation(int enAnimationClip);
	EnAnimationClip GetEnAnimationClip() const
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
	void Rotation();
	void RotationUpdate();
	void Swing()
	{
		m_setAnimation = enAnimationClip_Swing;
		m_rotation.SetRotationYFromDirectionXZ(m_facingDir);
		m_characterModel->SettRotation(m_rotation);

	}

	bool IsPlayAnimation()
	{
		return m_characterModel->IsPlayAnimation();
	}

	void AnimationUpdate()
	{
		m_characterModel->Update();
	}

	void SetPlayRotation()
	{
		m_setAnimation = enAnimationClip_Idle;
	}

	// Swingアニメーションが再生中かどうかを判定する関数
	// Swingアニメーションが再生中であればtrueを返し、そうでなければfalseを返す
	// 例えば、Swingアニメーションが再生中であれば、攻撃の当たり判定を有効にするなどの処理に利用できます。
	bool IsSwingAnimationPlaying()
	{
		return m_characterModel->IsPlayAnimation() && m_setAnimation == enAnimationClip_Swing;
	}
	bool m_isPaused = true;
private:
	std::unique_ptr<BatterStateMachine> m_stateMachine;
	AnimationClip m_animationClips[enAnimationClip_Num];
	EnAnimationClip m_setAnimation = enAnimationClip_Idle;
	int m_UniformNumber = BatterNumber::UniformNumber_1;
	Vector3 newPosition;
	Vector3 m_facingDir = Vector3(0.0f, 0.0f, -1.0f); // 初期向き
	Quaternion m_rotation; // 回転を保持するクォータニオン
	Vector3 pivotOffset = { 0.0f, 0.0f, 10.0f }; // 例: 回転の軸となるオフセット座標（ローカル）
	FontRender m_fontRender;
	CollisionObject* m_collisionObject;
	Quaternion m_batRotation; // バットの回転を保持するクォータニオン
	bool m_isAnimation = false; // animationの再生状態を保持するフラグ
	

};

