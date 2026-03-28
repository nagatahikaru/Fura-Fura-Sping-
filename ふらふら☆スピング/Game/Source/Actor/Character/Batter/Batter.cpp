#include "stdafx.h"
#include "Batter.h"
#include "BatterStateMachine.h"
#include"Source/Scene/InGame/Game.h"
#include "Source/Actor/Character/Ball/Ball.h"
#include <algorithm> // 追加


namespace {
	std::string FILE_PATH_BATTER = ("Assets/animData/batter/");
	//std::string FILE_PATH_BATTER_UNIFORMNUMBER = ("Assets/modelData/Batter/UniformNumber/");
	//std::string FILE_PATH_BAT = ("Assets/modelData/Bat");
	//std::string FILE_PATH_TKM = (".tkm");
	std::string FILE_PATH_DDS = (".tka");
	//std::string FILE_PATH_NUMBER[10] = {
	//	"0",
	//	"1",
	//	"2",
	//	"3",
	//	"4",
	//	"5",
	//	"6",
	//	"7",
	//	"8",
	//	"9"
	//};
	std::string FILE_PATH_ANIMATION[2] = {
		"idle",
		"swing"
	};

	//inline std::string GetBatterUniformNumberFilePath(int number)
	//{
	//	return FILE_PATH_BATTER_UNIFORMNUMBER + FILE_PATH_NUMBER[number] + FILE_PATH_TKM;
	//}

	inline std ::string GetAnimationFilePath(int number)
	{
		return FILE_PATH_BATTER + FILE_PATH_ANIMATION[number] + FILE_PATH_DDS;
	}

	//inline std::string GetBatFilePath()
	//{
	//	return FILE_PATH_BAT + FILE_PATH_TKM;
	//}
	///**
	//モデルの一括初期化処理
	//ModelRenderの初期化、位置、スケール設定、更新処理をまとめて行う関数
	//modelRender			 初期化するModelRenderのポインタ
	//m_animationClips	 アニメーションクリップの配列
	//enAnimationClip_Num	 アニメーションクリップの数
	//pos					 モデルの位置
	//scl					 モデルのスケール
	//filePath			 モデルデータのファイルパス
	//例:
	//InitModelRender(
	//	&m_modelRender[i],
	//	m_animationClips,
	//	enAnimationClip_Num,
	//	PlayerVariable::Transform::INITIAL_COORDINATE,
	//	PlayerVariable::Transform::INITIAL_SCALE,
	//	GetModelFilePath(i));
	//*/
	//void InitModelRender(
	//	ModelRender* modelRender
	//	, AnimationClip* m_animationClips
	//	, int enAnimationClip_Num
	//	, const Vector3& pos
	//	, const Vector3& scl
	//	, std::string filePath) {
	//	modelRender->Init(filePath.c_str(), m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
	//	modelRender->SetPosition(pos);
	//	modelRender->SetScale(scl);
	//	modelRender->Update();
	//}
	//
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
	if (m_collisionObject) {		
		delete m_collisionObject;
		m_collisionObject = nullptr;
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
	m_characterController.SetPosition(m_transform.m_position);

	

	m_collisionObject = new CollisionObject;
	m_collisionObject->CreateBox(
		m_characterModel->GetWeaponWorldPosition(),
		Quaternion::Identity,
		BatterBasicSettings::COLLISION_SCALE);

	m_characterModel->Update();
	m_inGameUI = FindGO<InGameUI>("inGameUI");
	m_ball = FindGO<Ball>("ball");
	return true;
}

void Batter::Update()
{
	if (m_isPaused) {
		return; // 完全停止
	}

	// ★ ポーズ中はキャッチャーのアニメーションを止める
	Game* game = FindGO<Game>("game");
	if (game && game->m_isPaused) {
		return;   // ← これでキャッチャーの動きが完全停止
	}

	if (GetRotationSeen())
	{
		m_meetCursorWorldPos = CalcCursorWorldPos();
	}

	m_stateMachine->Update();
}

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

	SetRotationSeen(true);
}

void Batter::RotationUpdate()
{
	//回転処理の更新
	m_transform.m_rotation.SetRotationYFromDirectionXZ(m_facingDir); // m_rotationAngle はメンバ変数などから取得

	// オフセットを考慮した位置の補正計算
	Vector3 pivot = m_transform.m_position - pivotOffset;
	newPosition = pivot + pivotOffset;

	m_characterModel->SettRotation(m_transform.m_rotation);
	m_characterModel->SetPosition(newPosition);
}

void Batter::SetPlayAnimation(int enAnimationClip)
{
	m_characterModel->PlayAnimation(enAnimationClip,0.2);
	
}

void Batter::SetBatSwingPosition()
{
	// バットの位置をミートカーソルの位置に合わせて調整する処理
	// ここでは例として、ミートカーソルの位置を取得してバットの位置を更新するコードを示します。
	// 実際のミートカーソルの位置はゲームのロジックに応じて取得してください。
	// 例: ミートカーソルの位置を取得
	Vector3 meetCursorPosition = m_inGameUI->GetMeetCursorPosition(); // この関数は実装されていると仮定
	// バットの位置をミートカーソルの位置に合わせて更新
	m_characterModel->SetWeaponOffset(meetCursorPosition - m_transform.m_position);
}

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

		m_meetPosition += move * speed * dt;

		m_meetPosition.x = std::clamp(m_meetPosition.x, -300.0f, 300.0f);
		m_meetPosition.y = std::clamp(m_meetPosition.y, -300.0f, 300.0f);
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

		m_transform.m_moveSpeed += right + forward;
	}

	m_inGameUI->SetMeetCursorPosition(m_meetPosition);
}

void Batter::HitBat()
{
	Vector3 base = m_characterModel->GetBatBase();
	Vector3 tip = m_characterModel->GetBatTip();
	Vector3 ballPos = m_ball->GetPosition();

	float dist = DistancePointToSegment(ballPos, base, tip);

	if (dist < 50.0f)
	{
		// ヒット
	}
}

float Batter::DistancePointToSegment(const Vector3& ballpos, const Vector3& base, const Vector3& tip)
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

Vector3 Batter::CalcCursorWorldPos()
{
	Vector3 camPos = g_camera3D->GetPosition();

	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	Vector3 up = g_camera3D->GetUp();

	float scale = 0.002f;

	Vector3 dir =
		forward +
		right * (m_meetPosition.x * scale) +
		up * (m_meetPosition.y * scale);

	dir.Normalize();

	// ★ 安全対策
	if (fabs(dir.y) < 0.0001f)
	{
		return camPos + dir * 1000.0f; // 適当に前方へ
	}

	float t = -camPos.y / dir.y;

	return camPos + dir * t;
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

void Batter::Render(RenderContext& rc)
{
	//モデルの描画
	m_characterModel->DrawCharacterModel(rc);

	//文字の描画
	wchar_t be[129];
	m_fontRender.SetPosition(-896.0f, 200.0f, 0.0f);
	m_fontRender.SetColor(g_vec4White);
	//Vector3 pos = m_transform.m_position;
	Vector3 pos = m_facingDir;
	swprintf(be, 129, L"pos:x=%.0f,y=%.0f,z=%.0f", pos.x, pos.y, pos.z);
	m_fontRender.SetText(be);
	m_fontRender.Draw(rc);
}