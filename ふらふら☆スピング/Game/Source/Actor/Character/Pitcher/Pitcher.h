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
	Pitcher();
	virtual ~Pitcher();
	virtual bool Start();
	virtual void Update();
	virtual void Render(RenderContext& rc);
	void SetPlayAnimation(int enAnimationClip);

	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Num
	};


private:
	Vector3 m_position = Vector3::Zero;
	Vector3 m_targetPosition = Vector3::Zero;

	ModelRender m_modelRender[PitcherNumber::Num];
	AnimationClip m_animationClips[enAnimationClip_Num];
	EnAnimationClip m_setAnimation = enAnimationClip_Idle;
	int m_UniformNumber = PitcherNumber::UniformNumber_1;
};

