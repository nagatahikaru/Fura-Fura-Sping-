#include "stdafx.h"
#include "Pitcher.h"
#include "Source/Actor/Character/Batter/Batter.h"

namespace PITCHER {
	std::string FILE_PATH_PITCHER_AIM = ("Assets/animData/pitcher/");
	std::string FILE_PATH_PITCHER = ("Assets/modelData/Pitcher/");
	std::string FILE_PATH_TKM = (".tkm");
	std::string FILE_PATH_DDS = (".tka");
	std::string FILE_PATH = {
		"Pitcher"
	};

	std::string FILE_PATH_ANIMATION[2] = {
		"Idle",
		"Throw"
	};

	inline std::string GetPitcherFilePath()
	{
		return FILE_PATH_PITCHER + FILE_PATH + FILE_PATH_TKM;
	}

	inline std::string GetAnimationFilePath(int number)
	{
		return FILE_PATH_PITCHER_AIM + FILE_PATH_ANIMATION[number] + FILE_PATH_DDS;
	}

	/**
	モデルの一括初期化処理
	ModelRenderの初期化、位置、スケール設定、更新処理をまとめて行う関数
	modelRender			 初期化するModelRenderのポインタ
	m_animationClips	 アニメーションクリップの配列
	enAnimationClip_Num	 アニメーションクリップの数
	pos					 モデルの位置
	scl					 モデルのスケール
	rot					 モデルの回転
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
		, const Quaternion& rot
		, std::string filePath) {
		modelRender->Init(filePath.c_str(), m_animationClips, enAnimationClip_Num, enModelUpAxisZ);
		modelRender->SetPosition(pos);
		modelRender->SetScale(scl);
		modelRender->SetRotation(rot);
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
};

Pitcher::Pitcher()
{
}

Pitcher::~Pitcher()
{
}

bool Pitcher::Start()
{
	// アニメーション初期化
	for (int j = 0; j < enAnimationClip_Num; j++)
	{
		if (j == enAnimationClip_Throw)
		{
			PITCHER::InitAnimation(m_animationClips, j, false);
		}
		else
		{
			PITCHER::InitAnimation(m_animationClips, j, true);
		}
	}

	Quaternion rot = m_transform.m_rotation;
	rot.AddRotationY(Math::DegToRad(-90));
	m_transform.m_rotation = rot;
	
	m_transform.m_position = Vector3(-0.0f, -24.0f, 50.0f);

	//初期位置の設定
	PITCHER::InitModelRender(
		&m_modelRender,
		m_animationClips,
		enAnimationClip_Num,
		m_transform.m_position,
		Vector3(10.0f, 10.0f, 10.0f),
		m_transform.m_rotation,
		PITCHER::GetPitcherFilePath());

	PITCHER::InitCharacterController(
		&m_collisionObject, 
		Vector3(10.0f, 10.0f, 10.0f), 
		m_transform.m_position);


	//アニメーション再生
	m_modelRender.PlayAnimation(enAnimationClip_Idle);

	m_timer = 0.0f;
	m_isThrowing = false;

	
	m_position = m_transform.m_position;
	m_game = FindGO<Game>("game");
	m_batter = FindGO<Batter>("batter");
	return true;
}

void Pitcher::Update()
{


	if (m_isPaused) {
		return; // 完全停止
	}
	if (m_game && m_game->GetShouldContinueTutorial()) {
		SetPlayAnimation(enAnimationClip_Idle);
		AnimationUpdate();
		return;
	}
	if (m_game && m_game->GetIsInputLocked() && !m_game->IsReplayPlaying()) {
		SetPlayAnimation(enAnimationClip_Idle);
		AnimationUpdate();
		return;
	}
	if(m_game==nullptr)
	{
		m_game = FindGO<Game>("game");
		return;
	}

	float dt = 1.0f / 60.0f;

	// ★ ポーズ中はアニメーションを止める

	if (m_game && m_game->GetIsPaused()) {
		return;   // ← これで投球アニメが途中で停止する
	}

	if (m_game && m_game->IsReplayPlaying()) {
		// アニメーションの更新（Idle）だけは行い、タイマー更新や投球判定には進ませない
		m_modelRender.Update();
		return;
	}
	// 🌟【ここも注意】ゲームがまだ始まっていない（ぐるぐる中など）もタイマーを進めない
	if (m_game && !m_game->IsGameStarted()) {
		m_modelRender.Update();
		return;
	}

	if(m_batter==nullptr)
	{
		m_batter = FindGO<Batter>("batter");
		return;
	}
	if (m_batter && m_batter->GetRotationSeen()) {
		SetPlayAnimation(enAnimationClip_Idle);
		AnimationUpdate();
		return;
	}

	m_timer += dt;

	if (!m_isThrowing && m_timer > 2.5f)
	{
		SetPlayAnimation(enAnimationClip_Throw);
		m_isThrowing = true;
		m_timer = 0.0f;
		// ★★★ ここで録画開始 ★★★
		if (m_game) {
			m_game->StartReplayRecording();
			m_game->SetGameStarted(true);   // ★ これを足す
			// ★ 投球開始フレームを記録
			int shotIndex = m_game->GetShots();
			m_game->SetPitchFrame(shotIndex,m_game->GetReplayFrameCount());
		}
	}

	if (m_isThrowing)
	{
		if (m_timer > 1.0f)
		{
			m_isThrowing = false;
		}
	}
	if(!m_characterController.IsOnGround())
	{
		m_position.y -= 0.1f;
		m_modelRender.SetPosition(m_position);
	}

	m_modelRender.Update();
}

void Pitcher::SetPlayAnimation(int enAnimationClip)
{
	m_modelRender.PlayAnimation(enAnimationClip);
}

void Pitcher::AnimationUpdate()
{	
	// Pitcher のアニメーション更新
	m_modelRender.Update();
}

void Pitcher::ResetThrow()
{
	m_timer = 0.0f;
	m_isThrowing = false;

	// Idle に戻す
	SetPlayAnimation(enAnimationClip_Idle);
}

void Pitcher::PlayPitchAnimation()
{
	// ★ 投球アニメーションを最初から再生	
	m_modelRender.PlayAnimation(enAnimationClip_Throw, 0.0f);
}

void Pitcher::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender.Draw(rc);
}