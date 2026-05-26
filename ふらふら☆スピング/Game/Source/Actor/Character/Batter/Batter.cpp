#include "stdafx.h"
#include "Batter.h"
#include "BatterStateMachine.h"
#include "Debuff/DebuffStageStateMachine.h"
#include"Source/Scene/InGame/Game.h"
#include "Source/Actor/Character/Ball/Ball.h"
#include"Source/Sound/SoundManager.h"
#include "Source/Effect/EffectManager.h"
#include "Source/Actor/GameCamera/GameCamera.h"
#include "Debuff/DebuffStage/DebuffStage.h"
#include <algorithm> // 追加
#include <deque>


// ファイル冒頭付近に追加（std::clampが使えない場合のため）
template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

namespace {
	std::string FILE_PATH_BATTER = ("Assets/animData/batter/");
	std::string FILE_PATH_DDS = (".tka");
	std::string FILE_PATH_ANIMATION[3] = {
		"idle",
		"guruguru",
		"swing"
		
	};

	inline std ::string GetAnimationFilePath(int number)
	{
		return FILE_PATH_BATTER + FILE_PATH_ANIMATION[number] + FILE_PATH_DDS;
	}

	void InitAnimation(AnimationClip animation[], int number, bool loop)
	{
		animation[number].Load(GetAnimationFilePath(number).c_str());
		animation[number].SetLoopFlag(loop);		
	}

	void InitCharacterController(CharacterController* characterController, const Vector3& scale, const Vector3& pos)
	{
		characterController->Init(scale.x, scale.y, pos);
		characterController->SetCollisionActive(true);
		characterController->IsOnGround();
	}
}

Batter::Batter()
{
	m_stateMachine = std::make_unique<BatterStateMachine>();
	m_debuffStageStateMachine = std::make_unique<DebuffStageStateMachine>();
}

Batter::~Batter()
{
	
	m_stateMachine->SetBatter(nullptr);
	m_debuffStageStateMachine->SetBatter(nullptr);
	if (g_effectManager) {
		g_effectManager->AllStopEffect();
	}

	if (m_collisionObject) {
		delete m_collisionObject;
	}
}

bool Batter::Start()
{
	//forループでまとめる
	//アニメーションクリップの読み込み
	for (int j = enAnimationClip_Idle; j < enAnimationClip_Swing; j++)
	{
		InitAnimation(m_animationClips, j, true);
	}
	for(int j = enAnimationClip_Swing; j < enAnimationClip_Num; j++)
	{
		InitAnimation(m_animationClips, j, false);
	}
	// インスタンスの生成
	m_characterModel = std::make_unique<nsApp::CharacterModel>();

	// ※ animationClip と numClips は環境に合わせて適切な値を渡してください
	m_characterModel->LoadCharacterModel(
		nsApp::CharacterModelType::BatterUniformNumber_0, 
		m_animationClips, 
		enAnimationClip_Num);

	// 2. 武器（バット）の読み込み
	m_characterModel->LoadWeaponModel(nsApp::CharacterModelType::Bat);

	// 3. アタッチするボーンの設定とオフセットの調整
	m_characterModel->SetWeaponAttackBone(L"mixamorig:RightHand"); // 実際のボーン名に合わせる
	m_characterModel->SetWeaponOffset(Vector3(100.0f, 150.0f, 0.0f));

	m_transform.m_position = BatterBasicSettings::INITIAL_COORDINATE;
	m_transform.m_rotation.SetRotationYFromDirectionXZ(m_facingDir);	

	// 4. 初期位置やスケールの設定
	m_characterModel->SetPosition(m_transform.m_position);
	m_characterModel->SetCharacterScale(BatterBasicSettings::INITIAL_SCALE);
	m_characterModel->SettRotation(m_transform.m_rotation);
	m_characterModel->SetWeaponScale(BatterBasicSettings::INITIAL_SCALE);
	

	InitCharacterController(&m_characterController,
		BatterBasicSettings::COLLISION_SCALE,
		BatterBasicSettings::INITIAL_COORDINATE);

	m_stateMachine->SetBatter(this);
	m_debuffStageStateMachine->SetBatter(this);
	m_debuffStageStateMachine->SetDebuffStage(m_debuffStage.GetDebuffStage());
	m_characterController.SetPosition(m_transform.m_position);
	m_initialRotation = m_transform.m_rotation;

	m_bodyCenter = BatterBasicSettings::INITIAL_COORDINATE;
	m_orbitAngle = 0.0f;

	m_guruGuruBatTimer = 5.0f;

	m_collisionObject = new CollisionObject;
	m_collisionObject->CreateBox(
		m_meetCursorWorldPos,
		Quaternion::Identity,
		BatBasicSettings::COLLISION_SCALE_BAT);

	m_characterModel->Update();	
	m_inGameUI = FindGO<InGameUI>("inGameUI");
	m_game = FindGO<Game>("game");
	m_ball = FindGO<Ball>("ball");
	m_collisionObject->Update();
	return true;
}

void Batter::Update()
{
	if(m_isPaused)
	{		
		return; // ゲームがポーズ中なら更新処理をスキップ
	}
	if (!m_inGameUI)
	{
		m_inGameUI = FindGO<InGameUI>("inGameUI");
		return;
	}
	m_game = FindGO<Game>("game");
	EffectInfo info;
	// ★ リプレイ中はバッターの通常処理を完全停止
	if (m_game && m_game->IsReplayPlaying()) {
		// ★ ポーズ中ならアニメーションも止める
		if (m_game->m_isPaused) {
			return;   // ← これでスイングアニメも完全停止
		}
		// ★ リプレイ中にスイングアニメが再生されているなら速度を4.0に固定
		if (IsSwingAnimationPlaying()) {
			m_characterModel->GetModelRender()->SetAnimationSpeed(4.0f);
		}
		// アニメーションだけ進めたい場合はこれを残す
		m_characterModel->Update();

		return; // ← 入力・ぐるぐる・カーソル・移動など全部止める
	}
	// ★ ポーズ中はキャッチャーのアニメーションを止める	
	if (m_game && m_game->m_isPaused) {
		g_effectManager->AllStopEffect(); // エフェクトも停止
		return;   // ← これでキャッチャーの動きが完全停止
	}
	
	// ★ 遅延ヒット処理
	m_stateMachine->Update();
	// ★ フラグが切り替わったら、ぐるぐるバットの処理を行う
	if(!m_isRotation)
	{
		m_debuffStageStateMachine->Update();
	}

	// ★★★ これを追加！ 毎フレーム UI に送る ★★★
	if (m_inGameUI) {
		m_inGameUI->SetGuruGuruCount(m_guruGuruBatCount);
	}

}

/** ぐるぐるバット関連コード */

/** 回転計算処理関数 */
void Batter::Rotation()
{	
	//コントローラー操作
	//左スティックの入力量を取得
	Vector3 stickL = Vector3::Zero;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	float inputAngle = atan2f(stickL.y, stickL.x); // 入力角度を計算

	float radius = 230.0f; // 回転半径
	// 入力角度に基づいてバッターの位置を更新
	m_modelPos.x = m_bodyCenter.x + radius * cosf(inputAngle);
	m_modelPos.y = m_bodyCenter.y;
	m_modelPos.z = m_bodyCenter.z + radius * sinf(inputAngle);

	// バッターの位置を更新
	Vector3 toCenter = m_bodyCenter - m_modelPos;
	toCenter.y = 0.0f; // 水平方向のみに制限
	toCenter.Normalize();

	//回転処理
	const float kEps = 0.001f;

	if (toCenter.Length() > kEps)
	{
		toCenter.Normalize();

		// 向きは「中心を見る」
		m_facingDir = toCenter;
	}

	float currentAngle = atan2f(m_facingDir.x, m_facingDir.z) * 180.0f / 3.14159265f;
	GuruGuruCountUP(currentAngle);
}

/**
* この関数はぐるぐるバットの処理を行う関数です。
* 時間を計測し、一定時間が経過したら方向をresetし
* カーソル操作が可能になるようにフラグを切り替える処理を行います。
*/
void Batter::RoundAndRoundBat()
{
	m_guruGuruBatTimer -= g_gameTime->GetFrameDeltaTime();
	// ★ UI に残り時間を送る
	if (m_inGameUI) {
		m_inGameUI->SetGuruGuruTimer(m_guruGuruBatTimer);
	}
	// ★ タイマーが0以下になったら、回転をリセットしてカーソル操作を可能にする
	if (m_guruGuruBatTimer <= 0.0f)
	{
		m_guruGuruBatTimer = 0.0f;
		SetRotationSeen(false);
		m_game->SetRotationSeen(false);
		m_game->SetGameStarted(true);
		m_characterModel->Update();
	}
}

/** ぐるぐるカウントup処理 */
void Batter::GuruGuruCountUP(float currentAngle)
{
	float delta = currentAngle - m_prevAngle;

	// ★ 角度のラップ補正（重要！）
	if (delta > 180.0f) {
		delta -= 360.0f;
	}
	else if (delta < -180.0f) {
		delta += 360.0f;
	}

	// 累積
	m_totalRotation += delta;

	// ★ 1回転判定
	if (m_totalRotation >= 360.0f) {
		m_guruGuruBatCount++;
		m_totalRotation -= 360.0f;
	}
	else if (m_totalRotation <= -360.0f) {
		m_guruGuruBatCount++;
		m_totalRotation += 360.0f;
	}
	m_game->SetGuruGuru(m_guruGuruBatCount);
	m_prevAngle = currentAngle;

	if (m_inGameUI) {
		m_inGameUI->SetGuruGuruCount(m_guruGuruBatCount);
	}
}

/** モデルの回転処理 */
void Batter::RotationUpdate()
{
	//　フラグが立っているときは回転を適用、そうでないときは初期回転に戻す
	if (m_isRotation)
	{	
		float yaw = atan2f(m_facingDir.x, m_facingDir.z);

		// 90度補正
		yaw += Math::DegToRad(90.0f);

		Quaternion rot;
		rot.SetRotationY(yaw);
		m_characterModel->SetPosition(m_modelPos);
		m_characterModel->SettRotation(rot);

		Quaternion weaponrot;
		weaponrot.SetRotationDeg(Vector3(0.0f, 0.0f, 1.0f), 230.0f);
		m_characterModel->SetWeaponRotation(true);
		m_characterModel->SetWeaponRotation(weaponrot);
		m_characterModel->SetWeaponPosition(Vector3(m_transform.m_position.x, m_transform.m_position.y + 200.0f, m_transform.m_position.z));
	
	}
	else
	{
		m_characterModel->SettRotation(m_initialRotation);
		m_characterModel->SetPosition(m_transform.m_position);
		m_characterModel->SetWeaponRotation(false);
	}
	
	m_characterModel->Update();
}


/** カーソル関連コード */

/** カーソル操作関数 */
void Batter::SetCursorPosition()
{
	float dt = 1.0f / 60.0f;

	m_inputScale.x = g_pad[0]->GetLStickXF();
	m_inputScale.y = g_pad[0]->GetLStickYF();

	// 毎フレームリセット
	m_cursorOffset = Vector3::Zero;

	if (m_isCursorMode)
	{
		Vector3 move;
		if (m_isDelayFrag)
		{
			move.x = m_inputdelayScale.x * m_inversioninputScale.x;
			move.y = m_inputdelayScale.y * m_inversioninputScale.y;
			move.z = 0.0f;
		}
		else
		{
			move.x = m_inputScale.x;
			move.y = m_inputScale.y;
			move.z = 0.0f;
		}

		float speed =
			500.0f * m_cursorMoveScale;

		Vector3 driftmove = move + m_driftCursorOffset;

		// プレイヤー入力だけ
		m_meetPosition += driftmove * speed * dt;

		m_meetPosition.x =
			clamp(m_meetPosition.x, -300.0f, 300.0f);

		m_meetPosition.y =
			clamp(m_meetPosition.y, -300.0f, 300.0f);
	}

	// デバフ適用後の最終座標
	Vector3 finalPos = GetFinalCursorPosition();
		

	m_inGameUI->SetMeetCursorPosition(finalPos);
}

/** 3D空間に2Dカーソルを合わせる処理 */
Vector3 Batter::CalcCursorWorldPos()
{
	float screenW = 1920.0f;
	float screenH = 1080.0f;
	Vector3 finalPos = GetFinalCursorPosition();

	// UI座標（中心基準なら変換必要）
	float mouseX =
		finalPos.x + screenW * 0.5f;

	float mouseY =
		finalPos.y + screenH * 0.5f;

	Vector3 camPos = g_camera3D->GetPosition();

	Vector3 rayDir = ScreenToRay(
		mouseX, mouseY,
		screenW, screenH,
		g_camera3D->GetViewMatrix(),
		g_camera3D->GetProjectionMatrix(),
		camPos
	);

	// ★ Z=5500の平面と交差
	Vector3 planePoint = Vector3(0, 0, m_ball->GetPosition().z);
	Vector3 planeNormal = Vector3(0, 0, 1);

	return RayToPlane(camPos, rayDir, planePoint, planeNormal);
}


/** Hit計算関連コード */
void Batter::HitBat()
{
	if (m_ball->m_hasHit) return;
	// ★ リプレイ中は絶対に打撃処理しない
	if (m_game && m_game->IsReplayPlaying()) {
		return;
	}
	// ★ ポーズ中は絶対に打撃処理しない
	if (m_game && m_game->m_isPaused) {
		return;
	}

	if (!IsSwingAnimationPlaying()) return;

	Vector3 ballPos = m_ball->GetPosition();

	// ① Z制限（打撃ゾーン）
	if (ballPos.z < 6060.0f || ballPos.z > 6080.0f) return;
	//if (ballPos.z < 500.0f || ballPos.z>5600.0f)return;
	// ② カーソル位置（Zはボールに合わせる）
	Vector3 cursor = m_meetCursorWorldPos;
	cursor.z = ballPos.z;

	// ③ 距離判定
	float dist = (ballPos - cursor).Length();

	if (dist < m_meatRange)
	{
		Vector3 hitDir = ballPos - cursor;

		// 前方向の力
		if (fabs(hitDir.z) >= 0.0f) {
			hitDir.z = -100.0f;
		}

		if (m_inGameUI) {
			HitEffect();
			m_inGameUI->m_shuchusenTimer = 0.5f;  // ← 集中線を0.2秒表示
		}

		if (m_game && !m_game->m_isHitStop) {
			m_game->m_hitStopTimer = 0.08f;
		}

		hitDir.y += 21.0f;
		hitDir.Normalize();
		// 角度（打ち上げ角）を計算
		// 角度（打ち上げ角）を計算
		float angleDeg = atan2f(hitDir.y, -hitDir.z) * 180.0f / 3.14159265f;

		// ★ 角度に応じてパワー補正（真ん中は補正なし）
		float powerScale = 1.0f;

		// 高いフライほどパワーを弱くする
		if (angleDeg > 60.0f) {
			powerScale = 0.35f;   // 高フライ → 40%減衰
			// ★ Y軸の上昇力を追加（強いフライにする）
			hitDir.y += 50.0f;    // ← 好きな値に調整（50〜80が自然）
		}
		else if (angleDeg > 30.0f) {
			powerScale = 0.55f;   // 中フライ → 20%減衰
		}
		// ★ 真ん中（10〜30度）→ パワー増加
		else if (angleDeg >= 10.0f && angleDeg <= 30.0f) {
			powerScale = 1.0f;   // ← 好きな倍率に調整
		}
		// ゴロ（角度が低すぎる）は少し弱くしてもOK
		else if (angleDeg < 0.0f) {
			powerScale = 0.8f;   // ゴロ → 少し弱く
		}

		// 最終パワー
		float finalPower = 935.0f * powerScale;

			// ★ 通常ヒット（即飛ぶ）
			m_ball->HitBall(hitDir,+finalPower);
			// ★ カメラ切り替え
			if (m_game) {
				m_game->SetCameraMode(Camera_BackBall);

				GameCamera* cam = m_game->GetGameCamera();
				if (cam) cam->StartHitMomentCamera();
			}

		// UI・SE・カメラなどは共通でOK
		if (m_inGameUI) {
			m_inGameUI->m_shuchusenTimer = 0.5f;
		}
		if (m_game) {
			m_game->m_hitStopTimer = 0.02f;
			m_game->m_canFastForward = true;
		}
		if (!m_game->m_isPaused && g_soundManager) {
			g_soundManager->PlaySE(Sound::enSound_SE, 100.0f);
			auto se2 = g_soundManager->PlaySE(Sound::enSound_SE2, 100.0f);
			se2->SetName("SE2");
		}
	}
}

void Batter::UpdateBatAim()
{
	if (!IsSwingAnimationPlaying()) return;
	// カーソルのワールド座標
	Vector3 target = m_meetCursorWorldPos;

	// ★ 右手をカーソルに向ける
	m_characterModel->AimRightHand(target);

	// ★ キャラ本体は正面(Z-)維持
	m_transform.m_rotation.SetRotationYFromDirectionXZ(m_facingDir);
	m_characterModel->SettRotation(m_transform.m_rotation);
}

void Batter::BatHitBoxPosition()
{
	// ★ カーソル位置を使う
	Vector3 pos = m_meetCursorWorldPos;

	// ★ 向き（任意）
	Vector3 dir = m_ball->GetPosition() - pos;
	dir.Normalize();

	Quaternion rot;
	rot.SetRotationYFromDirectionXZ(dir);

	m_collisionObject->SetPosition(pos);
	m_collisionObject->SetRotation(rot);
	m_collisionObject->Update();
}

void Batter::ResetSwing()
{
	// ★ スイングアニメーションをリセット
	m_transform.m_rotation = m_initialRotation;
	m_characterModel->SettRotation(m_initialRotation);

	// ★ カーソル位置もリセット
	m_meetPosition = Vector3::Zero;

	// ★ ぐるぐるバット関連もリセット
	m_isRotation = false;

	// ★ カーソルモードもリセット
	m_isCursorMode = false;
}

/** 演出関連コード */
void Batter::EffectUpdate()
{	
	if (m_guruGuruBatCount < 5) return;

	if (g_effectManager->GetIsPlayeEffect(m_inro.m_effectDawnID)){		
		return; // すでにエフェクトが再生中なら新たに出さない
	}
	
	Vector3 pos = Vector3(m_transform.m_position.x, m_transform.m_position.y + 100.0f, m_transform.m_position.z);

	m_inro.m_effectDawnID = g_effectManager->PlayEffect(
		enEffect_DownArrow,
		pos,
		Vector3(15.0f, 40.0f, 15.0f));
}

void Batter::HitEffect()
{
	Vector3 pos = m_ball->GetPosition();
	if (g_effectManager->GetIsPlayeEffect(m_inro.m_effectHitID)) {
		return; // すでにエフェクトが再生中なら新たに出さない
	}
	
	m_inro.m_effectHitID = g_effectManager->PlayEffect(
		enEffect_HitBat,
		pos,
		Vector3(20.0f, 20.0f, 20.0f));
}

/** 計算関連コード */

// スクリーン座標 → レイ方向変換
Vector3 Batter::ScreenToRay(
	float mouseX,
	float mouseY,
	float screenWidth,
	float screenHeight,
	const Matrix& view,
	const Matrix& proj,
	const Vector3& cameraPos)
{
	//=====================================================
	// ① スクリーン座標 → NDC座標変換
	//=====================================================

	float x =
		(2.0f * mouseX / screenWidth) - 1.0f;

	float y =
		1.0f - (2.0f * mouseY / screenHeight);

	// Clip空間座標
	Vector4 rayClip =
	{
		x,
		y,
		-1.0f,
		1.0f
	};

	//=====================================================
	// ② Clip空間 → View空間
	//=====================================================

	Matrix invProj = proj;
	invProj.Inverse();

	Vector4 rayView =
		InverseProjectionMatrix(
			rayClip,
			invProj);

	// 方向ベクトルとして扱う
	rayView =
	{
		rayView.x,
		rayView.y,
		-1.0f,
		0.0f
	};

	//=====================================================
	// ③ View空間 → World空間
	//=====================================================

	Matrix invView = view;
	invView.Inverse();

	Vector4 rayWorld4 =
		InverseProjectionMatrix(
			rayView,
			invView);

	//=====================================================
	// ④ 正規化して返す
	//=====================================================

	Vector3 rayDir =
	{
		rayWorld4.x,
		rayWorld4.y,
		rayWorld4.z
	};

	rayDir.Normalize();

	return rayDir;
}

// 行列逆変換
Vector4 Batter::InverseProjectionMatrix(
	const Vector4& v,
	const Matrix& m)
{
	Vector4 result;

	result.x =
		v.x * m._11 +
		v.y * m._21 +
		v.z * m._31 +
		v.w * m._41;

	result.y =
		v.x * m._12 +
		v.y * m._22 +
		v.z * m._32 +
		v.w * m._42;

	result.z =
		v.x * m._13 +
		v.y * m._23 +
		v.z * m._33 +
		v.w * m._43;

	result.w =
		v.x * m._14 +
		v.y * m._24 +
		v.z * m._34 +
		v.w * m._44;

	return result;
}

// レイと平面の交点取得
Vector3 Batter::RayToPlane(
	const Vector3& rayOrigin,
	const Vector3& rayDir,
	const Vector3& planePoint,
	const Vector3& planeNormal)
{
	//=====================================================
	// レイと平面が平行か判定
	//=====================================================

	float denom =
		planeNormal.Dot(rayDir);

	// 平行なら始点返す
	if (fabs(denom) < 0.0001f)
	{
		return rayOrigin;
	}

	//=====================================================
	// 交点距離計算
	//=====================================================

	float t =
		(planePoint - rayOrigin)
		.Dot(planeNormal) / denom;

	//=====================================================
	// 交点座標返却
	//=====================================================

	return rayOrigin + rayDir * t;
}

void Batter::Render(RenderContext& rc)
{
	//モデルの描画
	m_characterModel->DrawCharacterModel(rc);

	//文字の描画
	//wchar_t be[129];
	//m_fontRender.SetPosition(-896.0f, 200.0f, 0.0f);
	//m_fontRender.SetColor(g_vec4White);
	//Vector3 pos = m_ball->GetPosition();
	////Quaternion pos = m_transform.m_rotation;
	//swprintf(be, 129, L"pos:x=%.0f,y=%.0f,z=%.0f", pos.x, pos.y, pos.z);
	//m_fontRender.SetText(be);
	//m_fontRender.Draw(rc);
}