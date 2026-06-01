#pragma once
#include "Source/Actor/Character/Character.h"

namespace {
	namespace PitcherNumber {
		enum {
			UniformNumber_1,
			Num
		};
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

