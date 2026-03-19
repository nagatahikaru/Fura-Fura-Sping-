#include "stdafx.h"
#include "Batter.h"
#include "BatterStateMachine.h"

namespace {
	std::string FILE_PATH_BATTER = ("Assets/animData/batter/");
	std::string FILE_PATH_BATTER_UNIFORMNUMBER = ("Assets/modelData/Batter/UniformNumber/");
	std::string FILE_PATH_BAT = ("Assets/modelData/bat");
	std::string FILE_PATH_TKM = (".tkm");
	std::string FILE_PATH_DDS = (".tka");
	std::string FILE_PATH_NUMBER[10] = {
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9"
	};
	std::string FILE_PATH_ANIMATION[2] = {
		"idle",
		"swing"
	};

	inline std::string GetBatterUniformNumberFilePath(int number)
	{
		return FILE_PATH_BATTER_UNIFORMNUMBER + FILE_PATH_NUMBER[number] + FILE_PATH_TKM;
	}

	inline std ::string GetAnimationFilePath(int number)
	{
		return FILE_PATH_BATTER + FILE_PATH_ANIMATION[number] + FILE_PATH_DDS;
	}

	inline std::string GetBatteFilePath()
	{
		return FILE_PATH_BAT + FILE_PATH_TKM;
	}

	/**
	モデルの一括初期化処理
	ModelRenderの初期化、位置、スケール設定、更新処理をまとめて行う関数
	modelRender			 初期化するModelRenderのポインタ
	m_animationClips	 アニメーションクリップの配列
	enAnimationClip_Num	 アニメーションクリップの数
	pos					 モデルの位置
	scl					 モデルのスケール
	filePath			 モデルデータのファイルパス
	例:
	InitModelRender(
		&m_modelRender[i],
		m_animationClips,
		enAnimationClip_Num,
		PlayerVariable::Transform::INITIAL_COORDINATE,
		PlayerVariable::Transform::INITIAL_SCALE,
		GetModelFilePath(i));
	*/
	void InitModelRender(
		ModelRender* modelRender
		, AnimationClip* m_animationClips
		, int enAnimationClip_Num
		, const Vector3& pos
		, const Vector3& scl
		, std::string filePath) {
		modelRender->Init(filePath.c_str(), m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
		modelRender->SetPosition(pos);
		modelRender->SetScale(scl);
		modelRender->Update();
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
	if (m_collisionObject) {		
		delete m_collisionObject;
		m_collisionObject = nullptr;
	}
}

bool Batter::Start()
{
	//forループでまとめる
//アニメーションクリップの読み込み
	for (int j = enAnimationClip_Idle; j < enAnimationClip_Num; j++)
	{
		InitAnimation(m_animationClips, j, true);
	}
	//モデルレンダーの初期化
	InitModelRender(
		m_modelRender,
		m_animationClips,
		enAnimationClip_Num,
		BatterBasicSettings::INITIAL_COORDINATE,
		BatterBasicSettings::INITIAL_SCALE,
		GetBatterUniformNumberFilePath(m_UniformNumber));

	InitModelRender(
		m_batModelRender,
		nullptr,
		0,
		BatBasicSettings::INITIAL_COORDINATE,
		BatBasicSettings::INITIAL_SCALE,
		GetBatteFilePath());

	m_transform.m_position = BatterBasicSettings::INITIAL_COORDINATE;
	m_transform.m_rotation.SetRotationYFromDirectionXZ(m_facingDir);	
	

	InitCharacterController(&m_characterController,
		BatterBasicSettings::COLLISION_SCALE,
		BatterBasicSettings::INITIAL_COORDINATE);

	m_stateMachine->SetBatter(this);
	m_modelRender->SetRotation(m_transform.m_rotation);
	m_characterController.SetPosition(m_transform.m_position);

	m_modelRender->Update();

	m_collisionObject = new CollisionObject;
	m_collisionObject->CreateBox(
		m_transform.m_position,
		Quaternion::Identity,
		BatterBasicSettings::COLLISION_SCALE);

	m_batCollisionObject = new CollisionObject;
	m_batCollisionObject->CreateBox(
		m_offsetPosition,
		Quaternion::Identity,
		BatBasicSettings::INITIAL_SCALE);


	
	return true;
}

void Batter::Update()
{
	m_stateMachine->Update();

	/* 武器を装備させる。*/
	if (m_modelRender != nullptr && m_batModelRender != nullptr)
	{
		/* 右手のボーンの位置と傾きのデータを取得。*/
		m_handMatrix = GetWorldMatrix(L"mixamorig:RightHand");

		/* 行列から座標を抽出。*/
		m_matrixPosition.x = m_handMatrix.m[3][0];
		m_matrixPosition.y = m_handMatrix.m[3][1];
		m_matrixPosition.z = m_handMatrix.m[3][2];

		m_xAxis = Vector3(m_handMatrix.m[0][0], m_handMatrix.m[0][1], m_handMatrix.m[0][2]);
		m_yAxis = Vector3(m_handMatrix.m[1][0], m_handMatrix.m[1][1], m_handMatrix.m[1][2]);
		m_zAxis = Vector3(m_handMatrix.m[2][0], m_handMatrix.m[2][1], m_handMatrix.m[2][2]);

		/* 正規化。*/
		m_xAxis.Normalize();
		m_yAxis.Normalize();
		m_zAxis.Normalize();

		m_offsetPosition = m_matrixPosition + (m_xAxis * m_weaponOffset.x), (m_yAxis * m_weaponOffset.y), (m_zAxis * m_weaponOffset.z);
		m_batModelRender->SetPosition(m_offsetPosition);


		m_rotationMatrix = m_handMatrix;
		m_rotationMatrix.m[0][0] = m_xAxis.x; m_rotationMatrix.m[0][1] = m_xAxis.y; m_rotationMatrix.m[0][2] = m_xAxis.z;
		m_rotationMatrix.m[1][0] = m_yAxis.x; m_rotationMatrix.m[1][1] = m_yAxis.y; m_rotationMatrix.m[1][2] = m_yAxis.z;
		m_rotationMatrix.m[2][0] = m_zAxis.x; m_rotationMatrix.m[2][1] = m_zAxis.y; m_rotationMatrix.m[2][2] = m_zAxis.z;

		/* 正規化した行列から回転を抽出。*/
		m_matrixRotation.SetRotation(m_rotationMatrix);
		m_batModelRender->SetRotation(m_matrixRotation);

		/* 武器を更新。*/
		m_batModelRender->Update();
	}
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


}

void Batter::RotationUpdate()
{
	//回転処理の更新
	m_transform.m_rotation.SetRotationYFromDirectionXZ(m_facingDir); // m_rotationAngle はメンバ変数などから取得

	// オフセットを考慮した位置の補正計算
	Vector3 pivot = m_transform.m_position - pivotOffset;
	newPosition = pivot + pivotOffset;
	m_setAnimation = enAnimationClip_Idle;

	m_modelRender->SetRotation(m_transform.m_rotation);
	m_modelRender->SetPosition(newPosition);	
}

void Batter::SetPlayAnimation(int enAnimationClip)
{
	m_modelRender->PlayAnimation(enAnimationClip);
	m_modelRender->Update();
}

void Batter::Swing()
{
	m_setAnimation = enAnimationClip_Swing;
}

void Batter::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender->Draw(rc);

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