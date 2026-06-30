#include "stdafx.h"
#include "DebuffShakePattern.h"
#include "Source/Actor/Character/Batter/Batter.h"
#include "DebuffPatternBase.h"

void DebuffShakePattern::SetType(ShakeType type)
{
	m_type = type;
}

// UpdateŠÖ”‚ÍAƒfƒoƒt‚ÌŽí—Þ‚É‰ž‚¶‚ÄAƒoƒbƒ^[‚ÌƒJ[ƒ\ƒ‹‚ðã‰º‚â¶‰E‚Ék‚í‚¹‚éˆ—‚ðŽÀ‘•‚µ‚Ü‚·B
void DebuffShakePattern::Update(Batter* batter)
{
	switch (m_type)
	{
	case Shake_Vertical:
		VerticalShake(batter);
		break;
	case Shake_Horizontal:
		HorizontalShake(batter);
		break;
	case Shake_Random:
		RandomShake(batter);
		break;
	default:
		break;
	}
}

void DebuffShakePattern::VerticalShake(Batter* batter)
{
	// ã‰º‚Ìk‚¦‚ðŽÀ‘•
	float shakeAmount = GetPower(); // k‚¦‚Ì‹­‚³
	float shakeFrequency = GetSeismicIntensity(); // k‚¦‚Ì•p“x
	UpdateTime();
	float offsetY = SinWave(m_timer * shakeFrequency) * shakeAmount;
	batter->SetShakeCursorOffset(Vector3(0.0f, offsetY, 0.0f));
}

void DebuffShakePattern::HorizontalShake(Batter* batter)
{
	// ¶‰E‚Ìk‚¦‚ðŽÀ‘•
	float shakeAmount = GetPower(); // k‚¦‚Ì‹­‚³
	float shakeFrequency = GetSeismicIntensity(); // k‚¦‚Ì•p“x
	UpdateTime();
	float offsetX = CosWave(shakeFrequency) * shakeAmount;
	batter->SetShakeCursorOffset(Vector3(offsetX, 0.0f, 0.0f));
}

void DebuffShakePattern::RandomShake(Batter* batter)
{
	// ƒ‰ƒ“ƒ_ƒ€‚Èk‚¦‚ðŽÀ‘•
	float shakeAmount = GetPower(); // k‚¦‚Ì‹­‚³
	float shakeFrequency = GetSeismicIntensity(); // k‚¦‚Ì•p“x
	UpdateTime();
	float offsetX = CosWave(shakeFrequency) * shakeAmount;
	float offsetY = SinWave(shakeFrequency) * shakeAmount;
	batter->SetShakeCursorOffset(Vector3(offsetX, offsetY, 0.0f));
}


