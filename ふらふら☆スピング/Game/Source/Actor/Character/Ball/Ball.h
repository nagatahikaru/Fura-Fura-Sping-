#pragma once
#include "Source/Actor/Character/Character.h"
#include "Source/Difficulty.h"

class Game;

class Ball : public Character
{
public:
	Ball();
	virtual ~Ball();
	virtual bool Start();
	virtual void Update();
	virtual void Render(RenderContext& rc);

	//==============================================================
	// 投球・打撃
	//==============================================================
	// ボールを投げる
	void Throw(const Vector3& targetPos);
	// ボールを打つ（打球の方向と強さを指定）
	void HitBall(const Vector3& hitDirection, float hitPower);
	// 打った瞬間の打球が着地するまでの水平距離を予測する
	float PredictLandingDistance();
	// ボールを初期位置・初期状態に戻す
	void ResetBall();
	// 投球タイマーのリセット（着地など外部から呼ぶ）
	void ResetThrowTimer();

	// スライダー（カーブ）の横移動
	void Slider(float dt);
	// 終盤の落ち込み（現在は未使用）
	void ApplyProSpiritsDrop(float dt);

	//==============================================================
	// 軌跡エフェクト
	//==============================================================
	// 打球の軌跡を出す。distance（予測した飛距離）に対応する判定のエフェクトを選ぶ
	void StartTrajectoryEffect(float distance = 0.0f);
	void StopTrajectoryEffect();

	//==============================================================
	// 位置・速度・判定
	//==============================================================
	void SetPosition(const Vector3& pos);
	Vector3 GetPosition() const { return m_position; }
	Vector3 GetPrevPosition() const { return m_prevPosition; }

	void SetVelocity(const Vector3& v) { m_velocity = v; }
	const Vector3& GetVelocity() const { return m_velocity; }

	void GetFlightRay(Vector3& startPos, Vector3& endPos) const;
	Vector3 GetFlightDirection() const;
	float GetFlightLength() const { return (m_throwEndPos - m_throwStartPos).Length(); }

	bool CheckCollision(const Vector3& pos, float radius);
	CollisionObject* GetCollisionObject() const { return m_collisionObject; }

	// 角度帯テーブルからフェードアウト距離のしきい値を取得
	float GetFadeThresholdByAngle(float angleDeg) const;

	//==============================================================
	// 状態の取得・設定
	//==============================================================
	bool IsHit() const { return m_hasHit; }
	bool GetHasHit() const { return m_hasHit; }
	void SetHasHit(bool hasHit) { m_hasHit = hasHit; }

	bool IsMoving() const { return m_isMove; }
	bool GetIsMove() const { return m_isMove; }
	void SetIsMove(bool isMove) { m_isMove = isMove; }

	bool GetIsPaused() const { return m_isPaused; }
	void SetIsPaused(bool isPaused) { m_isPaused = isPaused; }

	bool GetIsMagicBall() const { return m_isMagicBall; }
	void SetIsMagicBall(bool isMagicBall) { m_isMagicBall = isMagicBall; }

	//==============================================================
	// リプレイ記録
	//==============================================================
	bool GetIsRecording() const { return m_isRecording; }
	void SetIsRecording(bool isRecording) { m_isRecording = isRecording; }

	std::vector<Vector3> GetReplayPath() const { return m_replayPath; }
	void SetReplayPath(const std::vector<Vector3>& path) { m_replayPath = path; }

	// 互換用（現在は何もしない）
	void SelectBallType(Difficulty diff) {}

private:
	// 球種
	enum BallType
	{
		Straight,
		Curve,
		ShakeHorizontal,
		ShakeVertical,
		SlowBall
	};

	//==============================================================
	// Update の内訳
	//==============================================================
	// 通常プレイの更新。途中で処理を打ち切った場合は false を返す
	bool UpdatePlay(Game* game);
	// リプレイ中の描画位置・拡大・回転の更新
	void UpdateReplayVisual(Game* game);

	// 打撃前の重力
	void ApplyPreHitGravity(float dt);
	// 移動中の加速・重力
	void ApplyMovingForces(float dt);
	// 風（打球にだけ作用）
	void ApplyWind(Game* game, float dt);
	// 速度を位置に反映
	void MoveBall(float dt);
	// 投球コース(m_pitchTargetX)への位置補正と変化球
	void ApplyPitchCourse(float dt);
	// 飛距離の更新・予測距離の表示・100m超えイベント
	void UpdateHitDistance(Game* game);
	// 通過位置に応じたSE・ストライク演出
	void CheckPassEvents(Game* game);
	// 空振り判定。空振りなら true
	bool CheckWhiff(Game* game);
	// 着地・バウンド・転がり
	void HandleGroundContact(Game* game, float dt);
	// 通常プレイ時の拡大率
	void UpdateModelScale(float dt);

	// 難易度に応じて球種・魔球を決める
	void ChoosePitchType(Difficulty difficulty);
	// 魔球が消える演出を一度だけ再生
	void TryPlayDisappearEffect(float effectScale);

	// 軌跡エフェクトの位置・向きの更新
	void UpdateTrajectoryEffect();
	// 尾を進行方向の後ろへ向ける回転を求める
	Quaternion CalcTrailRotation(const Vector3& travelDir) const;

private:
	//==============================================================
	// 定数
	//==============================================================
	static constexpr float kRollingFriction = 50.0f;     // 転がり中の減速量（速度/秒）
	static constexpr float kRollingStopSpeed = 30.0f;    // これ未満の速度で完全停止
	static constexpr int   kMaxBounces = 2;              // 最大バウンド回数
	static constexpr float kBounceRestitution = 0.35f;   // 反発係数（跳ねるたびに弱くなる）
	static constexpr float kMinBounceSpeed = 10.0f;      // これ未満のY速度ならバウンドさせず転がりへ

	//==============================================================
	// 位置・速度
	//==============================================================
	Vector3 m_position = Vector3::Zero;       // ボールの位置
	Vector3 m_prevPosition = Vector3::Zero;
	Vector3 m_velocity = Vector3::Zero;       // ボールの速度
	Vector3 m_throwStartPos = Vector3::Zero;
	Vector3 m_throwEndPos = Vector3::Zero;
	Vector3 m_finalPos = Vector3::Zero;       // 投球の到達予定位置（UI用）
	Vector3 m_hitStartPos = Vector3::Zero;    // 打った瞬間の位置

	//==============================================================
	// 投球パラメータ
	//==============================================================
	BallType m_ballType = Straight;
	float m_curveDir = 0.0f;
	float m_pitchTargetX = 0.0f;
	float m_pitchStartY = 0.0f;
	float m_baseGravity = 0.0f;
	float m_initialSpeedZ = 0.0f;
	// 直球の落ち始め位置の割合（0〜1）。以前は未初期化だった。値は要確認
	float m_dropStartRatioOverride = 0.0f;
	float m_throwTimer = 0.0f;
	float m_radius = 10.0f;

	//==============================================================
	// 描画・演出
	//==============================================================
	ModelRender m_modelRender;
	float m_rotateSpeed = 7660.0f;
	float m_rotationAngle = 0.0f;
	float m_hitScaleTimer = 0.0f;
	float m_storedPredictedDistance = 0.0f;

	//==============================================================
	// 状態フラグ
	//==============================================================
	bool m_isMove = false;
	bool m_isPaused = true;
	bool m_isRolling = false;           // 着地後、転がっている最中
	bool m_hasHit = false;              // 打たれて飛行中（着地で false に戻る）
	bool m_wasHit = false;              // 一度でも打たれた
	bool m_isMagicBall = false;
	bool m_isRecording = false;
	bool m_hasShownPrediction = false;
	bool m_hasPlayedSE6 = false;
	bool m_hasStrike = false;
	bool m_hasPlayedDisappearEffect = false;
	int  m_bounceCount = 0;

	//==============================================================
	// 当たり判定
	//==============================================================
	CollisionObject* m_collisionObject = nullptr;

	//==============================================================
	// リプレイ
	//==============================================================
	std::vector<Vector3> m_replayPath;

	//==============================================================
	// 軌跡エフェクト
	//==============================================================
	Vector3 m_renderPos = Vector3::Zero;     // 実際に描画しているボールの位置
	Vector3 m_trailPrevPos = Vector3::Zero;
	Vector3 m_trailDir = Vector3(0.0f, 0.0f, -1.0f);  // 平滑化した進行方向
	Quaternion m_trailRot = Quaternion::Identity;
	uint32_t m_trailEffectId = 0;
	bool m_hasTrailEffect = false;       // 軌跡エフェクト再生中
	bool m_replayTrailStarted = false;       // リプレイ中に軌跡を開始済みか
};