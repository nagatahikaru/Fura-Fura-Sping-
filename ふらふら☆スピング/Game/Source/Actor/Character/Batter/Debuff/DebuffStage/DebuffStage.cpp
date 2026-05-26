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

// ƒfƒoƒt’iŠK‚²‚Æ‚Ìˆ—‚ğÀ‘•

//‰ñ“]”F3`5‰ñ“]
void DebuffStage::DebuffStageOne()
{
	//—h‚êƒfƒoƒt
	auto shake =AddPattern<DebuffShakePattern>();
	DebuffShakePattern::ShakeType type = static_cast<DebuffShakePattern::ShakeType>(rand() % 2);
	shake->SetType(type);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount));
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount));
}

//‰ñ“]”F6`8‰ñ“]
void DebuffStage::DebuffStageTwo()
{
	//—h‚êƒfƒoƒt
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount));
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount));
	
}

//‰ñ“]”F9`11‰ñ“]
void DebuffStage::DebuffStageThree()
{
	//—h‚êƒfƒoƒt
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount));
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount));

	//ƒmƒCƒYƒfƒoƒt
	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Vertical);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount));
	noise->SetNoiseTimer(0.05f);
}

//‰ñ“]”F12`14‰ñ“]
void DebuffStage::DebuffStageFour()
{
	//—U“±ƒfƒoƒt
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount);
	magnet->SetRandomMoveDuration(m_rotationCount);
	
}

//‰ñ“]”F15`17‰ñ“]
void DebuffStage::DebuffStageFive()
{
	//—U“±ƒfƒoƒt
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount);
	magnet->SetRandomMoveDuration(m_rotationCount);

	//”»’èƒfƒoƒt
	auto aim = AddPattern<DebuffAimPattern>();
	aim->SetType(DebuffAimPattern::HeavySwing);
	aim->Reset();
	aim->SetMeatRange(aim->GetRotationRate(m_rotationCount));
}

//‰ñ“]”F18`20‰ñ“]
void DebuffStage::DebuffStageSix()
{
	//—¬‚³‚êƒfƒoƒt
	auto drift = AddPattern<DebuffDriftPattern>();
	drift->SetType(DebuffDriftPattern::Random);
	drift->Reset();
	drift->SetSpeed(drift->GetRotationRate(m_rotationCount));
	drift->SetForce(drift->GetRotationRate(m_rotationCount));
	drift->SetWaveSpeed(drift->GetRotationRate(m_rotationCount));

	//—U“±ƒfƒoƒt
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount);
	magnet->SetRandomMoveDuration(m_rotationCount);
}

//‰ñ“]”F21`23‰ñ“]
void DebuffStage::DebuffStageSeven()
{
	//—U“±ƒfƒoƒt
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount);
	magnet->SetRandomMoveDuration(m_rotationCount);

	//ƒmƒCƒYƒfƒoƒt
	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount));
	noise->SetNoiseTimer(0.05f);
}

//‰ñ“]”F24`26‰ñ“]
void DebuffStage::DebuffStageEight()
{
	//’x‚êƒfƒoƒt
	auto lag = AddPattern<DebuffLagPattern>();
	lag->SetType(DebuffLagPattern::Delay);
	lag->Reset();
}

//‰ñ“]”F27`29‰ñ“]
void DebuffStage::DebuffStageNine()
{
	//”½“]ƒfƒoƒt
	auto reverse = AddPattern<DebuffReversePattern>();
	reverse->SetType(DebuffReversePattern::Reverse_All);
}

//	‰ñ“]”F30‰ñ“]ˆÈã
void DebuffStage::DebuffStageTen()
{
	//ƒmƒCƒYƒfƒoƒt
	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount));
	noise->SetNoiseTimer(0.05f);

	//—U“±ƒfƒoƒt
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount);
	magnet->SetRandomMoveDuration(m_rotationCount);

	//—h‚êƒfƒoƒt
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount));
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount));

}
