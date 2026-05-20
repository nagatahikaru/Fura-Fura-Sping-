#include "stdafx.h"
#include "DebuffReversePattern.h"
#include "Source/Actor/Character/Batter/Batter.h"

void DebuffReversePattern::SetType(ReverseType type)
{
	m_type = type;
}

void DebuffReversePattern::Update(Batter* batter)
{
	switch (m_type)
	{
	case Reverse_Y:
		SetReverseY(batter);
		break;

	case Reverse_X:
		SetReverseX(batter);
		break;

	case Reverse_All:
		SetReverseAll(batter);
		break;
	}
}


