#pragma once
#include "Source/Actor/Character/Character.h"
#include "Source/Actor/Character/Batter/Batter.h"
#include "Source/Scene/InGame/Game.h"

namespace PITCHER {
	extern std::string FILE_PATH_PITCHER_AIM;
	extern std::string FILE_PATH_PITCHER;
	extern std::string FILE_PATH_TKM;
	extern std::string FILE_PATH_DDS;
	extern std::string FILE_PATH;
	extern std::string FILE_PATH_ANIMATION[2];
};

class Pitcher :public Character
{
private:
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Throw,
		enAnimationClip_Num
	};

	Game* m_game = nullptr;
	Batter* m_batter = nullptr;

	Vector3 m_position = Vector3::Zero;
	Vector3 m_targetPosition = Vector3::Zero;

	ModelRender m_modelRender;

	AnimationClip m_animationClips[enAnimationClip_Num];

	float m_timer = 0.0f;
	float m_throwReleaseTime = 0.35f; // 投球アニメのどのタイミングでボールを離すか

	bool m_isThrowing = false;
	bool m_hasReleasedBall = false;
	bool m_hasNotifiedThrow = false;


public:
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
	ModelRender* GetModelRender()
	{
		return &m_modelRender;
	};
};

