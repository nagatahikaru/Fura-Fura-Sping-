#include "stdafx.h"
#include "DebuffDriftPattern.h"
#include "Source/Actor/Character/Batter/Batter.h"

void DebuffDriftPattern::SetType(DriftType type)
{
	m_type = type;
}

// UpdateŠÖ”‚ÍAƒfƒoƒt‚ÌŽí—Þ‚É‰ž‚¶‚ÄAƒoƒbƒ^[‚ÌƒJ[ƒ\ƒ‹‚ðã‰º‚â¶‰E‚Ék‚í‚¹‚éˆ—‚ðŽÀ‘•‚µ‚Ü‚·B
void DebuffDriftPattern::Update(Batter* batter)
{
	switch (m_type)
	{
	case Vertical:
		VerticalShake(batter);
		break;
	case Horizontal:
		HorizontalShake(batter);
		break;
	case Random:
		if (rand() % 2 == 0) {
			VerticalShake(batter);
		}
		else {
			HorizontalShake(batter);
		}
		break;
	case Heavy:
		RandomShake(batter);
		break;
	default:
		break;
	}
}

void DebuffDriftPattern::VerticalShake(Batter* batter)
{
	// ã‰º‚Ìk‚¦‚ðŽÀ‘•
	float shakeAmount = 5.0f; // k‚¦‚Ì‹­‚³
	float shakeFrequency = 10.0f; // k‚¦‚Ì•p“x
	m_time += g_gameTime->GetFrameDeltaTime();
	float offsetY = sinf(m_time * shakeFrequency) * shakeAmount;
	batter->AddCursorOffset(Vector3(0.0f, offsetY, 0.0f));
}

void DebuffDriftPattern::HorizontalShake(Batter* batter)
{
	// ¶‰E‚Ìk‚¦‚ðŽÀ‘•
	float shakeAmount = 5.0f; // k‚¦‚Ì‹­‚³
	float shakeFrequency = 10.0f; // k‚¦‚Ì•p“x
	m_time += g_gameTime->GetFrameDeltaTime();
	float offsetX = cosf(m_time * shakeFrequency) * shakeAmount;
	batter->AddCursorOffset(Vector3(offsetX, 0.0f, 0.0f));
}

void DebuffDriftPattern::RandomShake(Batter* batter)
{
	// ƒ‰ƒ“ƒ_ƒ€‚Èk‚¦‚ðŽÀ‘•
	float shakeAmount = 5.0f; // k‚¦‚Ì‹­‚³
	float shakeFrequency = 10.0f; // k‚¦‚Ì•p“x
	m_time += g_gameTime->GetFrameDeltaTime();
	float offsetX = cosf(m_time * shakeFrequency) * shakeAmount;
	float offsetY = sinf(m_time * shakeFrequency) * shakeAmount;
	batter->AddCursorOffset(Vector3(offsetX, offsetY, 0.0f));
}


