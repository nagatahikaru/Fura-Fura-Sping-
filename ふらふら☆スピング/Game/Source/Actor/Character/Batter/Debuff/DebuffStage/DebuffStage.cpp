#include "stdafx.h"
#include "DebuffStage.h"
#include "Source/Actor/Character/Batter/Batter.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/IDebuffPattern.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffShakePattern.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffDriftPattern.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffNoisePattern.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffReversePattern.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffLagPattern.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffAimPattern.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffPatternBase.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffMagnetPattern.h"


DebuffStage::DebuffStage()
{

}

DebuffStage::~DebuffStage()
{
	ClearPatterns();
}

void DebuffStage::Update(Batter* batter)
{
	for (auto& pattern : m_patterns)
	{
		pattern->Update(batter);
	}
}

void DebuffStage::BuildStage(int level)
{
	switch (level)
	{
	case 1:
		DebuffStageOne();
		break;

	case 2:
		DebuffStageTwo();
		break;

	case 3:
		DebuffStageThree();
		break;
	case 4:
		DebuffStageFour();
		break;
	case 5:
		DebuffStageFive();
		break;
	case 6:
		DebuffStageSix();
		break;
	case 7:
		DebuffStageSeven();
		break;
	case 8:
		DebuffStageEight();
		break;
	case 9:
		DebuffStageNine();
		break;
	case 10:
		DebuffStageTen();
		break;
	default:
		return;
		break;
	}
}

void DebuffStage::DebuffStageOne()
{
	auto shake =AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Vertical);
	shake->SetPower(5.0f);
	shake->SetSeismicIntensity(10.0f);
}

void DebuffStage::DebuffStageTwo()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Vertical);
	shake->SetPower(5.0f);
	shake->SetSeismicIntensity(10.0f);
	
}

void DebuffStage::DebuffStageThree()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Vertical);
	shake->SetPower(5.0f);
	shake->SetSeismicIntensity(10.0f);
}

void DebuffStage::DebuffStageFour()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Vertical);
	shake->SetPower(5.0f);
	shake->SetSeismicIntensity(10.0f);
	
}

void DebuffStage::DebuffStageFive()
{
	// デバフ段階5の処理を実装
}

void DebuffStage::DebuffStageSix()
{
	// デバフ段階6の処理を実装
}

void DebuffStage::DebuffStageSeven()
{
	// デバフ段階7の処理を実装
}

void DebuffStage::DebuffStageEight()
{
	// デバフ段階8の処理を実装
}

void DebuffStage::DebuffStageNine()
{
	// デバフ段階9の処理を実装
}

void DebuffStage::DebuffStageTen()
{
	// デバフ段階10の処理を実装
}
