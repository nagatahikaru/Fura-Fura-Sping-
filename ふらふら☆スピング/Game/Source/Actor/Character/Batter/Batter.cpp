#include "stdafx.h"
#include "Batter.h"

namespace {
	std::string FILE_PATH_BATTER_UNIFORMNUMBER = ("Assets/modelData/Batter/UniformNumber/");
	std::string FILE_PATH_TKM = (".tkm");
	std::string FILE_PATH_NUMBER[9] = {
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8"	
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
		(i == PlayerVariable::FormState::Normal) ? Vector3(1.0f, 1.0f, 1.0f) : PlayerVariable::Transform::INITIAL_SCALE,
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
}

Batter::~Batter()
{
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
	
	return true;
}

void Batter::Update()
{

}

void Batter::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender[m_UniformNumber].Draw(rc);
}