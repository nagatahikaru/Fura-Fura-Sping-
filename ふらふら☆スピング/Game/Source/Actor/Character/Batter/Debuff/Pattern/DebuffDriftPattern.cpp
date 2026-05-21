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
	Vector2 v;
	v.x = batter->GetInputScale().x * m_force;
	v.y = batter->GetInputScale().y * m_force;
	UpdateTime();
	AngleUpdate();

	Vector2 vSrc = v;

	float rad = Math::DegToRad(m_angle);

	float c = cosf(rad);
	float s = sinf(rad);

	Vector2 out;

	out.x = vSrc.x * c - vSrc.y * s;
	out.y = vSrc.x * s + vSrc.y * c;

	batter->SetInputOffset(out.x, out.y);
}