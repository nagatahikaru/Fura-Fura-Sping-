#include "stdafx.h"
#include "DebuffDriftPattern.h"
#include "Source/Actor/Character/Batter/Batter.h"

void DebuffDriftPattern::SetType(DriftType type)
{
	m_type = type;
}

// Update関数は、デバフの種類に応じて、流される処理を実装します。
void DebuffDriftPattern::Update(Batter* batter)
{
	switch (m_type)
	{
	case Random:
		RandomDrift(batter);
		break;
	default:
		RandomDrift(batter);
		break;
	}
}

void DebuffDriftPattern::RandomDrift(Batter* batter)
{
	UpdateTime();
	AngleUpdate();

	float rad = Math::DegToRad(m_angle);

	float c = cosf(rad);
	float s = sinf(rad);

	Vector2 out;

	float dynamicForce =
		m_force *
		(0.7f + sinf(m_waveTime * m_waveSpeed) * 0.3f);

	out.x = c * dynamicForce;
	out.y = s * dynamicForce;

	batter->SetDriftCursorOffset(Vector3(out.x, out.y, 0.0f));
}