#include "stdafx.h"
#include "Batter.h"
#include "BatterStateMachine.h"
#include"Source/Scene/InGame/Game.h"
#include "Source/Actor/Character/Ball/Ball.h"
#include"Source/Sound/SoundManager.h"
#include "Source/Effect/EffectManager.h"
#include "Source/Actor/GameCamera/GameCamera.h"
#include <algorithm> // 追加


// ファイル冒頭付近に追加（std::clampが使えない場合のため）
template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

namespace {
	float PI= 3.1415f / 180.0f;
	float ZERO_FLOAT = 0.0f;

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
}

Batter::~Batter()
{
	m_stateMachine->SetBatter(nullptr);
	//当たり判定オブジェクトの削除
	if (m_collisionObject)return;
	delete m_collisionObject;
	
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
	m_characterController.SetPosition(m_transform.m_position);
	m_initialRotation = m_transform.m_rotation;

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
	// ★ ポーズ中はキャッチャーのアニメーションを止める	
	if (m_game && m_game->m_isPaused) {
		g_effectManager->StopEffect(); // エフェクトも停止
		return;   // ← これでキャッチャーの動きが完全停止
	}

	m_stateMachine->Update();


	// ★★★ これを追加！ 毎フレーム UI に送る ★★★
	if (m_inGameUI) {
		m_inGameUI->SetGuruGuruCount(m_guruGuruBatCount);
	}
}


/** ぐるぐるバット関連コード */

/**
* 回転計算処理関数
*/
void Batter::Rotation()
{
	//キーボード操作
	//コントローラー操作
	if(ERROR_DEVICE_NOT_CONNECTED != ERROR_SUCCESS)
	{
		//xzの移動速度を0.0fにする
		m_transform.m_moveSpeed.x = BatterBasicSettings::NONE_SPEED;
		m_transform.m_moveSpeed.z = BatterBasicSettings::NONE_SPEED;

		//左スティックの入力量を取得
		Vector3 stickL = Vector3::Zero;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.y = g_pad[0]->GetLStickYF();

		//カメラの前方向と右方向のベクトルを持って来る。
		Vector3 forward = g_camera3D->GetForward();
		Vector3 right = g_camera3D->GetRight();
		//ｙ方向には移動させない
		forward.y = BatterBasicSettings::NONE_SPEED;
		right.y = BatterBasicSettings::NONE_SPEED;

		//左スティックの入力量と200.0fを乗算
		right *= stickL.x * BatterBasicSettings::BASICS_SPEED;
		forward *= stickL.y * BatterBasicSettings::BASICS_SPEED;

		//移動速度にスティックの入力量を加算する。
		m_transform.m_moveSpeed += right + forward;


		//回転処理
		Vector3 ford = m_transform.m_moveSpeed;
		ford.y = 0.0f;

		const float kEps = 0.001f;
		if (ford.Length() > kEps) {
			// 移動があるときだけ向きを更新する
			ford.Normalize();
			m_facingDir = ford; // last non-zero direction を保持
		}
	}
	else
	{
		float lx = g_pad[0]->GetLStickXF();
		float ly = g_pad[0]->GetLStickYF();

		Vector3 dir;
		dir.x = lx;
		dir.z = ly;
		dir.y = 0.0f;

		if (dir.Length() > 0.1f)
		{
			dir.Normalize();
			m_facingDir = dir;
		}
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
	if (m_guruGuruBatTimer <= 0.0f)
	{
		m_transform.m_rotation.SetRotationYFromDirectionXZ(m_facingDir);

		Quaternion offset;
		offset.SetRotationY(-90.0f); // ←ここ調整ポイント

		Quaternion finalRot = m_transform.m_rotation * offset;
		finalRot.Normalize();

		m_characterModel->SettRotation(finalRot);
		m_guruGuruBatTimer = 0.0f;
		SetRotationSeen(false);
		m_game->SetRotationSeen(false);
		m_game->SetGameStarted(true);
		m_characterModel->Update();
	}
}

/**
* ぐるぐるカウントup処理
*/
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

/**
* モデルの回転処理
*/
void Batter::RotationUpdate()
{
	//回転処理の更新
	m_transform.m_rotation.SetRotationYFromDirectionXZ(m_facingDir); // m_rotationAngle はメンバ変数などから取得

	// オフセットを考慮した位置の補正計算
	Vector3 pivot = m_transform.m_position - pivotOffset;
	newPosition = pivot + pivotOffset;


	m_characterModel->SettRotation(m_transform.m_rotation);

	if (!m_isRotation)
	{
		m_characterModel->SettRotation(m_initialRotation);
		Quaternion rot;
		rot.SetRotationDeg(Vector3(1.0f, 0.0f, 0.0f), 180.0f);
		m_characterModel->SetWeaponRotation(false);
		m_characterModel->SetWeaponRotation(rot);
		m_characterModel->SetWeaponPosition(Vector3(m_transform.m_position.x, m_transform.m_position.y+200.0f, m_transform.m_position.z));
	}else
		{
		m_characterModel->SetWeaponRotation(true);
	}

	m_characterModel->SetPosition(newPosition);
}


/** カーソル関連コード */

/**
* カーソル操作関数
*/
void Batter::SetCursorPosition()
{
	float dt = 1.0f / 60.0f;

	float lx = g_pad[0]->GetLStickXF();
	float ly = g_pad[0]->GetLStickYF();

	if (m_isCursorMode)
	{
		// カーソル操作
		Vector3 move;
		move.x = lx;
		move.y = ly;
		move.z = 0.0f;

		float speed = 500.0f;

		m_meetPosition += move * speed * dt+ m_randomCursorMovePwer;

		m_meetPosition.x = clamp(m_meetPosition.x, -300.0f, 300.0f);
		m_meetPosition.y = clamp(m_meetPosition.y, -300.0f, 300.0f);
	}
	else
	{
		// 従来の移動ロジック
		m_transform.m_moveSpeed.x = 0.0f;
		m_transform.m_moveSpeed.z = 0.0f;

		Vector3 forward = g_camera3D->GetForward();
		Vector3 right = g_camera3D->GetRight();

		forward.y = 0.0f;
		right.y = 0.0f;

		right *= lx * 400.0f;
		forward *= ly * 400.0f;

		m_meetPosition += right + forward+ m_randomCursorMovePwer;
	}

	m_inGameUI->SetMeetCursorPosition(m_meetPosition);
}

/**
* 3D空間に2Dカーソルを合わせる処理
*/
Vector3 Batter::CalcCursorWorldPos()
{
	float screenW = 1920.0f;
	float screenH = 1080.0f;

	// UI座標（中心基準なら変換必要）
	float mouseX = m_meetPosition.x + screenW * 0.5f;
	float mouseY = m_meetPosition.y + screenH * 0.5f;

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

/**
* カーソルデバフの段階を分けるための計算処理
*/
void Batter::SetRandomCursorTimeRadius()
{
	// ★ 回転回数に応じて時間と半径を増加させる
	int count = m_guruGuruBatCount / 5;
	if (count >= 10) count = 10; // 上限を設ける（必要に応じて調整）
	// ★ ランダムな時間と半径を設定
	m_randomMoveDuration = count * -0.5f + 5.0f;// 例: 回転5回ごとに時間0.5秒減少
	m_randomSpotRadius = count * 50.0f + 50.0f; // 例: 回転5回ごとに半径50増加
	m_randomCursorUpdate = true;
}


/** デバフ関連コード */
void Batter::DebuffDepth() {
	// ★ 回転回数が0以下ならデバフ無し
	if (m_guruGuruBatCount <= 4)
	{
		m_randomCursorMovePwer = Vector3::Zero;
		return;
	}

	// ★ デバフの強さを回転回数で決める
	//ランダムなベクトルを取得
	if (m_randomCursorUpdate)
	{
		//そのベクトルに向かって移動
		// 基準点から半径250の円範囲内でランダムに出現位置を決定
		//ランダムな角度と距離を生成
		float angle = (rand() % 360) * PI;
		float radius = SetRandom(0, m_randomSpotRadius);

		// 円の中のランダム位置を生成
		m_randomCursorTargetPos.x = cosf(angle) * radius;
		m_randomCursorTargetPos.y = sinf(angle) * radius;
		m_randomCursorMoveTimer = SetRandom(0, m_randomMoveDuration);
		m_randomCursorUpdate = false;
	}
	Vector3 toTarget = m_randomCursorTargetPos - m_meetPosition;

	// 少しずつ寄せる（ここがデバフの強さ）
	m_randomCursorMovePwer = toTarget * 0.05f;
	//一定時間経過後、再度ランダムなベクトルを取得
	m_randomCursorMoveTimer -= g_gameTime->GetFrameDeltaTime();
	EffectUpdate();
	if (m_randomCursorMoveTimer <= ZERO_FLOAT)
	{
		m_randomCursorUpdate = true;
	}
}


/** Hit計算関連コード */
void Batter::HitBat()
{
	// ★ ポーズ中は絶対に打撃処理しない
	if (m_game && m_game->m_isPaused) {
		return;
	}

	if (!IsSwingAnimationPlaying()) return;

	Vector3 ballPos = m_ball->GetPosition();

	// ① Z制限（打撃ゾーン）
	if (ballPos.z < 6000.0f || ballPos.z > 6100.0f) return;
	//if (ballPos.z < 500.0f || ballPos.z>5600.0f)return;
	// ② カーソル位置（Zはボールに合わせる）
	Vector3 cursor = m_meetCursorWorldPos;
	cursor.z = ballPos.z;

	// ③ 距離判定
	float dist = (ballPos - cursor).Length();

	if (dist < 100.0f)
	{
		Vector3 hitDir = ballPos - cursor;

		// ★ 前方向の力を必ず入れる（ここを追加）
		if (fabs(hitDir.z) >= 0.0f) {
			hitDir.z = -50.0f;
		}

		hitDir.Normalize();

		m_ball->HitBall(hitDir, 1000.0f);

		if (m_inGameUI) {
			m_inGameUI->m_shuchusenTimer = 0.5f;  // ← 集中線を0.2秒表示
		}

		// ★ 打った瞬間に倍速許可を ON
		if (m_game) {
			m_game->m_canFastForward = true;
		}

		// ★ 打撃 SE 再生
	// ★ 打撃 SE 再生（ポーズ中は絶対に鳴らさない）
		if (!m_game->m_isPaused && g_soundManager) {
			g_soundManager->PlaySE(Sound::enSound_SE, 100.0f);
			auto se2 = g_soundManager->PlaySE(Sound::enSound_SE2, 100.0f);
			se2->SetName("SE2");
		}

		// ★ ここでカメラ切り替え
		if (m_game) {
			m_game->SetCameraMode(Camera_BackBall);

			// ★ 打った瞬間カメラ開始
			GameCamera* cam = m_game->GetGameCamera();
			if (cam) cam->StartHitMomentCamera();
		}

		if (m_inGameUI) {
			m_inGameUI->SetBaisokuVisible(true);
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
	// �����̌����ɖ߂�
	m_transform.m_rotation = m_initialRotation;
	m_characterModel->SettRotation(m_initialRotation);

	// �J�[�\���ʒu�𒆉��ɖ߂��i�C�Ӂj
	m_meetPosition = Vector3::Zero;

	// ��]�t���O����Z�b�g�i�K�v�Ȃ�j
	m_isRotation = false;

	// �J�[�\�����[�h����i�K�v�Ȃ�j
	m_isCursorMode = false;
}

void Batter::SetCursorMode(bool flag)
{
	m_isCursorMode = flag;
}

void Batter::ResetCursorPosition()
{
	m_meetPosition = Vector3::Zero;
}



/** 演出関連コード */
void Batter::EffectUpdate()
{
	if (m_guruGuruBatCount < 5) return;

	if (g_effectManager->GetIsPlayeEffect()) {
		return; // すでにエフェクトが再生中なら新たに出さない
	}
	
	Vector3 pos = Vector3(m_transform.m_position.x, m_transform.m_position.y + 100.0f, m_transform.m_position.z);

	g_effectManager->SetEffect(
		enEffect_DownArrow,
		pos,
		Quaternion::Identity,
		Vector3(6.0f, 10.0f, 6.0f)
	);
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