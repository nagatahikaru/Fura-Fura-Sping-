#pragma once
#include "Source/Actor/Character/Character.h"
#include "Source/UI/InGameUI/InGameUI.h"
#include "BatterStateMachine.h"
#include "Source/Actor/Character/Ball/Ball.h"
#include "Source/Scene/InGame/Game.h"
#include "Source/Effect/EffectManager.h"

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
		const Vector3 COLLISION_SCALE_BAT = Vector3(114.285714286f, 80.0f, 40.0f);
	}
};

class Batter :public Character
{
public:	
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Rotation,
		enAnimationClip_Swing,		
		enAnimationClip_Num
	};

	Batter();
	virtual ~Batter();
	virtual bool Start();
	virtual void Update();

	virtual void Render(RenderContext& rc);
	void SetPlayAnimation(int enAnimationClip)
	{
		m_characterModel->PlayAnimation(enAnimationClip,0.2);
	}


	EnAnimationClip GetEnAnimationClip() const
	{
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
	void SetBatSwingPosition()
	{
		// バットの位置をミートカーソルの位置に合わせて調整する処理
		// ここでは例として、ミートカーソルの位置を取得してバットの位置を更新するコードを示します。
		// 実際のミートカーソルの位置はゲームのロジックに応じて取得してください。
		// 例: ミートカーソルの位置を取得
		Vector3 meetCursorPosition = m_inGameUI->GetMeetCursorPosition(); // この関数は実装されていると仮定
		// バットの位置をミートカーソルの位置に合わせて更新
		m_characterModel->SetWeaponOffset(meetCursorPosition - m_transform.m_position);
	}

	// Swingアニメーションを再生する関数
	void Swing()
	{
		m_setAnimation = enAnimationClip_Swing;
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
		m_setAnimation = enAnimationClip_Rotation;
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

	float DistancePointToSegment(const Vector3& ballpos, const Vector3& base, const Vector3& tip)
	{
		Vector3 ab = tip - base;
		Vector3 ac = ballpos - base;
		float lenSq = ab.Dot(ab);
		if (lenSq < 0.0001f)
		{
			return (ballpos - base).Length(); // 線じゃなく点扱い
		}
		float t = ac.Dot(ab) / lenSq;
		t = max(0.0f, min(1.0f, t)); // std::max, std::min を使うために <algorithm> が必要
		Vector3 closestPoint = base + ab * t;
		return (ballpos - closestPoint).Length();
	}

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
		float y = 1.0f - (2.0f * mouseY / screenHeight);

		Vector4 rayClip = Vector4(x, y, -1.0f, 1.0f); // ★ -1 にする（超重要）

		// ② Clip → View
		Matrix invProj = proj;
		invProj.Inverse();
		Vector4 rayView = InverseProjectionMatrix(rayClip, invProj);

		// ★ ここ修正
		rayView = Vector4(rayView.x, rayView.y, -1.0f, 0.0f);

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

	// レイと平面の交点を計算する関数
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

	void UpdateCursor3D()
	{
		SetCursorPosition();
		m_meetCursorWorldPos = CalcCursorWorldPos();
	}

	void RoundAndRoundBat();

	void GuruGuruCountUP(float currentAngle);

	int GetGuruGuruBatCount() const
	{
		return m_guruGuruBatCount;
	}

	void DebuffDepth();
	void SetRandomCursorTimeRadius();
	float SetRandom(const float min, const float max)
	{
		return min + (max - min) * (rand() / (float)RAND_MAX);
	}
	void SetPlaySpeed(float speed) {
		m_playSpeed = speed;
	}
	float GetPlaySpeed() const {
		return m_playSpeed;
	}
	void EffectUpdate();
	int GetGuruGuru() const {
		return m_guruGuruBatCount;
	}
	nsApp::CharacterModel* GetCharacterModel() const
	{
		return m_characterModel.get();
	}
	void PlaySwingAnimation();
	void ResetSwing();
	void SetCursorMode(bool flag);
	void ResetCursorPosition();
	bool m_isPaused;


private:
	// ★ ステートマシン関連の変数（追加）
	std::unique_ptr<BatterStateMachine> m_stateMachine;		// バッターステートマシンへのユニークポインタ

	// ★ ポインタ関連の変数（追加）	
	Game* m_game;											// ゲームクラスへのポインタ
	FontRender m_fontRender;								// フォントレンダラー
	CollisionObject* m_collisionObject;						// 当たり判定オブジェクトへのポインタ
	InGameUI* m_inGameUI;									// インゲームUIへのポインタ
	Ball* m_ball;											// ボールへのポインタ

	// ★ 記憶数値関連の変数（追加）
	Vector3 m_facingDir = Vector3(0.0f, 0.0f, -1.0f);		// 初期向き
	Vector3 newPosition;									// 新しい位置を保持する変数
	float m_prevAngle = 0.0f;								// 前回の角度を保持する変数
	Quaternion m_initialRotation;							// 初期の向きを保存する変数
	int m_UniformNumber = BatterNumber::UniformNumber_1;	// ユニフォームの番号

	// ★ アニメーションを管理する変数（追加）
	float m_playSpeed = 1.0f;								// アニメーションの再生速度を管理する変数
	AnimationClip m_animationClips[enAnimationClip_Num];	// アニメーションクリップの配列
	EnAnimationClip m_setAnimation = enAnimationClip_Idle;	// 現在再生中のアニメーションクリップ

	// ★ カーソル関連の変数（追加）
	Vector3 m_meetPosition;									// ミートカーソルの位置を保持する変数
	bool m_isCursorMode = true;								// カーソルモードの状態を保持するフラグ
	Vector3 m_meetCursorWorldPos;							// ミートカーソルのワールド座標を保持する変数
	bool m_randomCursorUpdate = false;						// ランダムな位置にカーソルを更新するフラグ
	Vector3 m_randomCursorTargetPos;						// ランダムな位置に更新するためのターゲット座標
	float m_randomCursorMoveTimer = 0.0f;					// ランダムな位置にカーソルを移動するためのタイマー
	float m_randomSpotRadius = 0.0f;						// ランダムな位置にカーソルを移動する際の半径
	float m_randomMoveDuration = 0.0f;						// ランダムな位置にカーソルを移動する際の移動時間
	Vector3 m_randomCursorMovePwer;							// ランダムな位置にカーソルを移動する際の移動の強さ

	// ★ グルグルバットの回転に関する変数（追加）
	Quaternion m_rotation;									// 回転を保持するクォータニオン
	Vector3 pivotOffset = { 0.0f, 0.0f, 10.0f };			// 例: 回転の軸となるオフセット座標（ローカル）
	bool m_isRotation = true;								// 回転状態を保持するフラグ
	float m_guruGuruBatTimer = 0.0f;						// グルグルバットのタイマー
	float m_totalRotation = 0.0f;							// グルグルバットの累積回転量
	int m_guruGuruBatCount = 0;								// グルグルバットの回数

	// ★ 遅延ヒット用の変数（追加）
	bool m_isHitReserved = false;							// ヒットが予約されているかどうかを管理するフラグ
	float m_hitDelayTimer = 0.0f;							// ヒットの遅延時間を管理するタイマー
	Vector3 m_reservedHitDir = Vector3::Zero;				// ヒットの遅延後に使用するヒットの方向を管理する変数
	float m_reservedHitPower = 0.0f;						// ヒットの遅延後に使用するヒットの強さを管理する変数
	Transform m_transform;									// Transformの型に合わせてください
	std::unique_ptr<nsApp::CharacterModel> m_characterModel;// CharacterModelへのユニークポインタ
};

