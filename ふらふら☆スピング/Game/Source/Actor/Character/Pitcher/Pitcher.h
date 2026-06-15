#pragma once
#include "Source/Actor/Character/Character.h"

namespace {
	namespace PitcherNumber {
		enum {
			UniformNumber_1,
			Num
		};
	}
	namespace PITCHER{
		std::string FILE_PATH_PITCHER = ("Assets/animData/pitcher/");
		std::string FILE_PATH_BATTER_UNIFORMNUMBER = ("Assets/modelData/Pitcher/UniformNumber/");
		std::string FILE_PATH_TKM = (".tkm");
		std::string FILE_PATH_DDS = (".tka");
		std::string FILE_PATH_NUMBER[1] = {
			"0"
		};

		std::string FILE_PATH_ANIMATION[2] = {
			"Idle",
			"Throw"
		};

		inline std::string GetBatterUniformNumberFilePath(int number)
		{
			return FILE_PATH_BATTER_UNIFORMNUMBER + FILE_PATH_NUMBER[number] + FILE_PATH_TKM;
		}

		inline std::string GetAnimationFilePath(int number)
		{
			return FILE_PATH_PITCHER + FILE_PATH_ANIMATION[number] + FILE_PATH_DDS;
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


	}
};

class Pitcher :public Character
{
public:

	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Throw,
		enAnimationClip_Num
	};

	Pitcher();
	virtual ~Pitcher();
	virtual bool Start();
	virtual void Update();
	virtual void Render(RenderContext& rc);
	void SetPlayAnimation(int enAnimationClip);
	void RotationUpdate();
	bool m_isPaused = true;
	void AnimationUpdate();
	void ResetThrow();
	void PlayPitchAnimation();
	ModelRender m_modelRender[PitcherNumber::Num];
	int m_UniformNumber = PitcherNumber::UniformNumber_1;
private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_targetPosition = Vector3::Zero;
	
	AnimationClip m_animationClips[enAnimationClip_Num];

	float m_timer = 0.0f;
	bool m_isThrowing = false;
	float m_throwReleaseTime = 0.35f; // 投球アニメのどのタイミングでボールを離すか
	bool m_hasReleasedBall = false;
	bool m_hasNotifiedThrow = false;
};

