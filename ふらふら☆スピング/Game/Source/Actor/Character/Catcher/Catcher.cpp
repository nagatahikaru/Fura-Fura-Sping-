#include "stdafx.h"
#include "Catcher.h"
#include "Source/Actor/Character/Ball/Ball.h"

namespace CATCHER {
	std::string FILE_PATH_CATCHER_AIM = ("Assets/animData/catcher/");
	std::string FILE_PATH_CATCHER = ("Assets/modelData/Catcher/");
	std::string FILE_PATH_TKM = (".tkm");
	std::string FILE_PATH_DDS = (".tka");
	std::string FILE_PATH = {
		"Catcher"
	};
	std::string FILE_PATH_ANIMATION[1] = {
		"idle"
	};
	inline std::string GetcatcherFilePath()
	{
		return FILE_PATH_CATCHER + FILE_PATH + FILE_PATH_TKM;
	}

	inline std::string GetAnimationFilePath(int number)
	{
		return FILE_PATH_CATCHER_AIM + FILE_PATH_ANIMATION[number] + FILE_PATH_DDS;
	}

	/**
	* モデルの一括初期化処理
	* ModelRenderの初期化、位置、スケール設定、更新処理をまとめて行う関数
	* modelRender			初期化するModelRenderのポインタ
	* m_animationClips		アニメーションクリップの配列
	* enAnimationClip_Num	アニメーションクリップの数
	* pos					モデルの位置
	* scl					モデルのスケール
	* filePath				モデルデータのファイルパス
	* 例:
	* InitModelRender(
	* 	&m_modelRender[i],
	* 	m_animationClips,
	* 	enAnimationClip_Num,
	* 	Vector3(0.0f, 0.0f, 0.0f),
	* 	Vector3(1.0f, 1.0f, 1.0f),
	* 	GetModelFilePath(i));
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

	void LoadAnimationClips(AnimationClip* m_animationClips, int enAnimation, int enAnimationClip_Num)
	{
		for (int j = enAnimation; j < enAnimationClip_Num; j++)
		{
			m_animationClips[j].Load(GetAnimationFilePath(j).c_str());
			m_animationClips[j].SetLoopFlag(true);
		}
	}

	/**
	*  CharacterControllerの初期化処理
	*  characterController	初期化するCharacterControllerのポインタ
	*  scale				体の大きさ
	*  pos					初期位置
	*  使い方
	*	InitCharacterController(&m_characterController,
	*	Vector3(1.0f, 2.0f, 1.0f),
	*	Vector3(0.0f, 0.0f, 0.0f));
	*/
	void InitCharacterController(CharacterController* characterController, const Vector3& scale, const Vector3& pos)
	{
		characterController->Init(scale.x, scale.y, pos);
		characterController->SetCollisionActive(true);
		characterController->IsOnGround();
	}
};


Catcher::~Catcher()
{
	delete m_collisionObject;
}

bool Catcher::Start()
{
	CATCHER::LoadAnimationClips(m_animationClips,enAnimationClip_Idle, enAnimationClip_Num);
	
	Quaternion rot = m_transform.m_rotation;
	rot.AddRotationY(Math::DegToRad(180.0f));
	m_transform.m_rotation = rot;	

	CATCHER::InitModelRender(
		&m_modelRender,
		m_animationClips,
		enAnimationClip_Num,
		CATCHER::CatcherBasicSettings::INITIAL_COORDINATE,
		CATCHER::CatcherBasicSettings::INITIAL_SCALE,
		m_transform.m_rotation,
		CATCHER::GetcatcherFilePath());
	
	CATCHER::InitCharacterController(&m_characterController,
		CATCHER::CatcherBasicSettings::COLLISION_SCALE,
		CATCHER::CatcherBasicSettings::INITIAL_COORDINATE);

	m_collisionObject = new CollisionObject;
	m_collisionObject->CreateBox(
		CATCHER::CatcherBasicSettings::INITIAL_COORDINATE,
		Quaternion::Identity,
		CATCHER::CatcherBasicSettings::COLLISION_SCALE);

	m_ball = FindGO<Ball>("ball");
	m_game = FindGO<Game>("game");

	return true;
}

void Catcher::Update()
{
	if (m_isPaused) return;

	if(m_game==nullptr)
	{
		m_game = FindGO<Game>("game");
		return;
	}

	if (m_game && m_game->m_isPaused) return;
	if (!m_game->IsGameStarted())return;

	Catch();
}

void Catcher::Catch()
{
	if(m_collisionObject->IsHit(m_ball->GetCollisionObject()))
	{
		m_game->OnBallLanded();
	}
}

void Catcher::PlayeAnimation(int animationNo)
{
	m_modelRender.PlayAnimation(animationNo);
}

void Catcher::Render(RenderContext& rc)
{
	if (m_game == nullptr)
	{
		m_game= FindGO<Game>("game");
		return;
	}
	if (m_game->GetCameraType() == 0)
	{
		return;
	}
	//モデルの描画
	m_modelRender.Draw(rc);
}