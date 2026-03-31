#pragma once
#include "Source/Actor/Character/Character.h"
#include "Source/UI/InGameUI/InGameUI.h"
#include "BatterStateMachine.h"
#include "Source/Actor/Character/Ball/Ball.h"
#include "Source/Scene/InGame/Game.h"

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
		const Vector3 VECTOR_LOOKAT = Vector3(1.0f, 0.0f, 0.0f); //注視点ベクトル
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
		const Vector3 COLLISION_SCALE_BAT = Vector3(114.285714286f, 80.0f, 114.285714286f);
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
	// バットのSwing位置を設定する関数
	//ミートカーソルの位置に合わせてバットの位置を調整するための関数です。
	void SetBatSwingPosition();

	// Swingアニメーションを再生する関数
	void Swing()
	{
		m_setAnimation = enAnimationClip_Swing;
		m_rotation.SetRotationYFromDirectionXZ(m_facingDir);
		m_characterModel->SettRotation(m_rotation);

	}

	// アニメーションが再生中かどうかを判定する関数
	bool IsPlayAnimation()
	{
		return m_characterModel->IsPlayAnimation();
	}

	// アニメーションの更新を行う関数
	void AnimationUpdate()
	{
		m_characterModel->Update();
	}

	// 回転アニメーションを再生する関数
	void SetPlayRotation()
	{
		m_setAnimation = enAnimationClip_Idle;
	}

	void SetIdleAnimation()
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

	void SetCursorPosition();

	void SetRotationSeen(bool isRotation)
	{
		m_isRotation = isRotation;
	}

	bool GetRotationSeen() const
	{
		return m_isRotation;
	}

	Vector3 GetBatPostion() const
	{
		return m_characterModel->GetWeaponWorldPosition();
	}

	void HitBat();

	float DistancePointToSegment(const Vector3& ballpos, const Vector3& base, const Vector3& tip);

	void UpdateBatAim();

	Vector3 CalcCursorWorldPos();

	void BatHitBoxPosition();

	Vector3 ScreenToRay(
		float mouseX, float mouseY,
		float screenWidth, float screenHeight,
		const Matrix& view,
		const Matrix& proj,
		const Vector3& cameraPos)
	{
		// ① スクリーン → NDC
		float x = (2.0f * mouseX / screenWidth) - 1.0f;
		float y = 1.0f - (2.0f * mouseY / screenHeight); // Y反転

		Vector4 rayClip = Vector4(x, y, 1.0f, 1.0f);

		// ② Clip → View
		Matrix invProj = proj;
		invProj.Inverse();
		Vector4 rayView = InverseProjectionMatrix(rayClip, invProj);
		rayView.z = 1.0f;
		rayView.w = 0.0f;

		// ③ View → World
		Matrix invView = view;
		invView.Inverse();
		Vector4 rayWorld4 = InverseProjectionMatrix(rayView, invView);

		Vector3 rayDir = Vector3(rayWorld4.x, rayWorld4.y, rayWorld4.z);
		rayDir.Normalize();

		return rayDir;
	}

	Vector4 InverseProjectionMatrix(const Vector4& v, const Matrix& m)
	{
		Vector4 result;

		result.x = v.x * m._11 + v.y * m._21 + v.z * m._31 + v.w * m._41;
		result.y = v.x * m._12 + v.y * m._22 + v.z * m._32 + v.w * m._42;
		result.z = v.x * m._13 + v.y * m._23 + v.z * m._33 + v.w * m._43;
		result.w = v.x * m._14 + v.y * m._24 + v.z * m._34 + v.w * m._44;

		return result;
	}

	Vector3 RayToPlane(
		const Vector3& rayOrigin,
		const Vector3& rayDir,
		const Vector3& planePoint,
		const Vector3& planeNormal)
	{
		float denom = planeNormal.Dot(rayDir);

		if (fabs(denom) < 0.0001f)
			return rayOrigin; // 平行（適当回避）

		float t = (planePoint - rayOrigin).Dot(planeNormal) / denom;

		return rayOrigin + rayDir * t;
	}

	Vector3 GetCursorWorldPos() const
	{
		return m_meetCursorWorldPos;
	}

	void UpdateCursor3D();

	void RoundAndRoundBat();

	void UpdateRotation(float currentAngle);

	bool m_isPaused;
private:
	std::unique_ptr<BatterStateMachine> m_stateMachine;
	AnimationClip m_animationClips[enAnimationClip_Num];
	EnAnimationClip m_setAnimation = enAnimationClip_Idle;
	int m_UniformNumber = BatterNumber::UniformNumber_1;
	Game* m_game; // ゲームクラスへのポインタ
	Vector3 newPosition;
	Vector3 m_facingDir = Vector3(0.0f, 0.0f, -1.0f); // 初期向き
	Quaternion m_rotation; // 回転を保持するクォータニオン
	Vector3 pivotOffset = { 0.0f, 0.0f, 10.0f }; // 例: 回転の軸となるオフセット座標（ローカル）
	FontRender m_fontRender;
	CollisionObject* m_collisionObject;
	Quaternion m_batRotation; // バットの回転を保持するクォータニオン
	bool m_isAnimation = false; // animationの再生状態を保持するフラグ
	InGameUI* m_inGameUI; // インゲームUIへのポインタ
	Vector3 m_meetPosition; // ミートカーソルの位置を保持する変数
	bool m_isRotation = true; // 回転アニメーションの再生状態を保持するフラグ
	Ball* m_ball; // ボールへのポインタ
	bool m_isCursorMode = true;
	Vector3 m_meetCursorWorldPos;
	float m_guruGuruBatTimer = 0.0f; // グルグルバットのタイマー
	float m_prevAngle = 0.0f;
	float m_totalRotation = 0.0f;
	int m_guruGuruBatCount = 0; // グルグルバットの回数
};

