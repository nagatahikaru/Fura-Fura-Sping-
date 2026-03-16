#include "stdafx.h"
#include "Batter.h"
#include "BatterStateMachine.h"

namespace {
	std::string FILE_PATH_BATTER_UNIFORMNUMBER = ("Assets/modelData/Batter/UniformNumber/");
	std::string FILE_PATH_TKM = (".tkm");
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

	inline std::string GetBatterUniformNumberFilePath(int number)
	{
		return FILE_PATH_BATTER_UNIFORMNUMBER + FILE_PATH_NUMBER[number + 1] + FILE_PATH_TKM;
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
		modelRender->Init(filePath.c_str(), m_animationClips, enAnimationClip_Num, enModelUpAxisY);
		modelRender->SetPosition(pos);
		modelRender->SetScale(scl);
		modelRender->Update();
	}
}


Batter::Batter()
{
	m_stateMachine = std::make_unique<BatterStateMachine>();
}

Batter::~Batter()
{
	m_stateMachine->SetBatter(nullptr);
}

bool Batter::Start()
{
	//モデルレンダーの初期化

		InitModelRender(
			&m_modelRender[m_UniformNumber],
			m_animationClips,
			enAnimationClip_Num,
			BasicSettings::INITIAL_COORDINATE,
			BasicSettings::INITIAL_SCALE,
			GetBatterUniformNumberFilePath(m_UniformNumber));

		m_characterController.IsOnGround();
		m_stateMachine->SetBatter(this);
	
	return true;
}

void Batter::Update()
{
	m_stateMachine->Update();
}

void Batter::Rotation()
{
	//xzの移動速度を0.0fにする
	m_transform.m_moveSpeed.x = BasicSettings::NONE_SPEED;
	m_transform.m_moveSpeed.z = BasicSettings::NONE_SPEED;

	//左スティックの入力量を取得
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持って来る。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//ｙ方向には移動させない
	forward.y = BasicSettings::NONE_SPEED;
	right.y = BasicSettings::NONE_SPEED;

	//左スティックの入力量と200.0fを乗算
	right *= stickL.x * BasicSettings::BASICS_SPEED;
	forward *= stickL.y * BasicSettings::BASICS_SPEED;

	//移動速度にスティックの入力量を加算する。
	m_transform.m_moveSpeed += right + forward;


	//回転処理
	Vector3 forward = m_transform.m_moveSpeed;
	forward.y = 0.0f;

	const float kEps = 0.001f;
	if (forward.Length() > kEps) {
		// 移動があるときだけ向きを更新する
		forward.Normalize();
		m_facingDir = forward; // last non-zero direction を保持
	}
}

void Batter::RotationUpdate()
{
	//回転処理の更新
	m_transform.m_rotation.SetRotationYFromDirectionXZ(m_facingDir); // m_rotationAngle はメンバ変数などから取得

	// オフセットを考慮した位置の補正計算
	newPosition = m_transform.m_position - pivotOffset; // m_position は現在の座標	
	newPosition += pivotOffset;

	m_modelRender[m_UniformNumber].SetRotation(m_transform.m_rotation);
	m_modelRender[m_UniformNumber].SetPosition(newPosition);
}

void Batter::SetPlayAnimation(int enAnimationClip)
{
	m_modelRender[m_UniformNumber].PlayAnimation(enAnimationClip);
}

void Batter::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender[m_UniformNumber].Draw(rc);
}