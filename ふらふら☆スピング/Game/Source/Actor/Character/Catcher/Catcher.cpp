#include "stdafx.h"
#include "Catcher.h"

namespace {
	std::string FILE_PATH_CATCHER = ("Assets/animData/catcher/");
	std::string FILE_PATH_CATCHER_UNIFORMNUMBER = ("Assets/modelData/Catcher/UniformNumber/");
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
	std::string FILE_PATH_ANIMATION[1] = {
		"idle"
	};

	inline std::string GetcatcherUniformNumberFilePath(int number)
	{
		return FILE_PATH_CATCHER_UNIFORMNUMBER + FILE_PATH_NUMBER[number] + FILE_PATH_TKM;
	}

	inline std::string GetAnimationFilePath(int number)
	{
		return FILE_PATH_CATCHER + FILE_PATH_ANIMATION[number] + FILE_PATH_DDS;
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
		, std::string filePath) {
		modelRender->Init(filePath.c_str(), m_animationClips, enAnimationClip_Num, enModelUpAxisY);
		modelRender->SetPosition(pos);
		modelRender->SetScale(scl);
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
}

bool Catcher::Start()
{
	LoadAnimationClips(m_animationClips,enAnimationClip_Idle, enAnimationClip_Num);
	
	InitModelRender(
		&m_modelRender,
		m_animationClips,
		enAnimationClip_Num,
		CatcherBasicSettings::INITIAL_COORDINATE,
		CatcherBasicSettings::INITIAL_SCALE,
		GetcatcherUniformNumberFilePath(0));
	
	InitCharacterController(&m_characterController, 
		CatcherBasicSettings::COLLISION_SCALE,
		CatcherBasicSettings::INITIAL_COORDINATE);

	return true;
}

void Catcher::Update()
{

}

void Catcher::CatchJudgement()
{
	
}

void Catcher::PlayeAnimation(int animationNo)
{
	m_modelRender.PlayAnimation(animationNo);
}

void Catcher::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender.Draw(rc);
}