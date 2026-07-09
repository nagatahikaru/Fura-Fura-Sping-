#include "stdafx.h"
#include "DebuffStage.h"
#include "Source/Actor/Character/Batter/Batter.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/IDebuffPattern.h"


DebuffStage::DebuffStage()
{

}

void DebuffStage::Update(Batter* batter)
{
	for (auto& pattern : m_patterns)
	{
		pattern->Update(batter);
	}
}
