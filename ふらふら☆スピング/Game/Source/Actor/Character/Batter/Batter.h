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

	namespace BatBasicSettings{
		const Vector3 INITIAL_COORDINATE = Vector3(0.0f, 0.0f, 0.0f);//初期座標
		const Vector3 INITIAL_SCALE = Vector3(10.0f, 10.0f, 10.0f); //初期スケール
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
	void Swing();

	bool IsPlayAnimation()
	{
		/* nullチェック。*/
		if (m_modelRender != nullptr)
			/* アニメーションが再生終了しているかを返す。*/
			return m_modelRender->IsPlayingAnimation();

		return false;
	}

	/* 指定したボーンのワールド行列を取得する。
* @param boneName 取得したいボーンの名前を指定する。
*/
	Matrix GetWorldMatrix(const wchar_t* boneName);

private:
	std::unique_ptr<BatterStateMachine> m_stateMachine;
	AnimationClip m_animationClips[enAnimationClip_Num];
	EnAnimationClip m_setAnimation = enAnimationClip_Idle;
	int m_UniformNumber = BatterNumber::UniformNumber_0;
	Vector3 newPosition;
	Vector3 m_facingDir = Vector3(0.0f, 0.0f, -1.0f); // 初期向き
	Vector3 pivotOffset = { 0.0f, 0.0f, 10.0f }; // 例: 回転の軸となるオフセット座標（ローカル）
	FontRender m_fontRender;
	CollisionObject* m_collisionObject;	

	//以下、装備のための変数
	ModelRender* m_batModelRender;/* バットのモデルを管理する変数。*/	
	CollisionObject* m_batCollisionObject;/* バットの当たり判定を管理する変数。*/
	std::wstring m_swingBoneName;
	Quaternion m_matrixRotation;
	Matrix m_handMatrix;			/* 武器を装備させるときの右手のボーンの行列を管理する変数。*/
	Matrix m_rotationMatrix;		/* 武器を装備させたときの武器の傾きを制御。*/
	Vector3 m_xAxis;				/* 武器を装備させるときの右手のボーンの行列から抽出したX軸を管理する変数。*/
	Vector3 m_yAxis;				/* 武器を装備させるときの右手のボーンの行列から抽出したY軸を管理する変数。*/
	Vector3 m_zAxis;				/* 武器を装備させるときの右手のボーンの行列から抽出したZ軸を管理する変数。*/
	Vector3 m_matrixPosition;
	Vector3 m_weaponOffset;			/* 武器の位置のずれを管理する変数。*/
	Vector3 m_offsetPosition;
};

