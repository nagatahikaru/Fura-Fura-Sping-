#include "stdafx.h"
#include "Background.h"

namespace {
	std::string FILE_PATH = ("Assets/stage/");
	std::string FILE_PATH_TKM = (".tkm");
	std::string MAGUMA_FILE_PATH[1] = {
		"baseballField"
	};	

	inline std::string GetMagumaFilePath(const int number)
	{
		return FILE_PATH + MAGUMA_FILE_PATH[number] + FILE_PATH_TKM;
	}

	/**
		スプライト情報更新
		spriteRender:スプライトレンダー
		pos:座標
		scl:スケール
		基礎値:X=1.0f,Y=1.0f
		最大値:X1920.0f,Y1080.0f
		中心を0として
		x:-960.0f~960.0f
		y:-540.0f~540.0f
		filePath:ファイルパス
		戻り値:なし
		例:
		UpdateSpriteInfo(&spriteRender,
		Vector3(0.0f,0.0f,0.0f),
		Vector3(1.0f,1.0f,1.0f),
		"Assets/sprite/Number/0.DDS");
	*/
	void InitData(
		ModelRender* modelRender
		, PhysicsStaticObject* physicsStaticObject
		, std::string filePath) {
		modelRender->Init(filePath.c_str());
		physicsStaticObject->CreateFromModel(
			modelRender->GetModel()
			, modelRender->GetModel().GetWorldMatrix());//モデルから静的な物理オブジェクトを生成
		return;
	}
}

bool Background::Start()
{
	InitData(&m_modelRender, &m_physicsStaticObject, GetMagumaFilePath(0));
	return true;
}

void Background::Update()
{
}

void Background::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
