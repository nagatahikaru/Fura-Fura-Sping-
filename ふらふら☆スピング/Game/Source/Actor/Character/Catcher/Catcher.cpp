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
		"Idle"
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
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(1.0f, 1.0f, 1.0f),
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

bool Catcher::Start()
{
	InitModelRender(
		&m_modelRender[m_UniformNumber],
		m_animationClips,
		enAnimationClip_Num,
		BasicSettings::INITIAL_COORDINATE,
		BasicSettings::INITIAL_SCALE,
		GetcatcherUniformNumberFilePath(0));
	return true;
}

void Catcher::Update()
{

}

void Catcher::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender[m_UniformNumber].Draw(rc);
}