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
	case Vertical:
		VerticalDrift(batter);
		break;
	case Horizontal:
		HorizontalDrift(batter);
		break;
	case Random:
		RandomDrift(batter);
		break;
	default:
		break;
	}
}

void DebuffDriftPattern::VerticalDrift(Batter* batter)
{

}

void DebuffDriftPattern::HorizontalDrift(Batter* batter)
{

}

void DebuffDriftPattern::RandomDrift(Batter* batter)
{

}