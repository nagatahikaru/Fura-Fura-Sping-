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

// デバフ段階ごとの処理を実装

//回転数：3～5回転
void DebuffStage::DebuffStageOne()
{
	//揺れデバフ
	auto shake =AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Vertical);
	shake->SetPower(shake->GetRotationRate(m_rotationCount));
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount));
}

//回転数：6～8回転
void DebuffStage::DebuffStageTwo()
{
	//揺れデバフ
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->SetPower(shake->GetRotationRate(m_rotationCount));
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount));
	
}

//回転数：9～11回転
void DebuffStage::DebuffStageThree()
{
	//揺れデバフ
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->SetPower(shake->GetRotationRate(m_rotationCount));
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount));

	//ノイズデバフ
	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Vertical);
	noise->SetPower(noise->GetRotationRate(m_rotationCount));
	noise->SetNoiseTimer(0.05f);
}

//回転数：12～14回転
void DebuffStage::DebuffStageFour()
{
	//誘導デバフ
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->SetRandomSpotRadius(magnet->GetRotationRate(m_rotationCount));
	magnet->SetRandomMoveDuration(magnet->GetRotationRate(m_rotationCount));
	
}

//回転数：15～17回転
void DebuffStage::DebuffStageFive()
{
	//誘導デバフ
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->SetRandomSpotRadius(magnet->GetRotationRate(m_rotationCount));
	magnet->SetRandomMoveDuration(magnet->GetRotationRate(m_rotationCount));

	//判定デバフ
	auto aim = AddPattern<DebuffAimPattern>();
	aim->SetType(DebuffAimPattern::HeavySwing);
	aim->SetMeatRange(aim->GetRotationRate(m_rotationCount));
}

//回転数：18～20回転
void DebuffStage::DebuffStageSix()
{
	//流されデバフ
	auto drift = AddPattern<DebuffDriftPattern>();
	drift->SetType(DebuffDriftPattern::Random);
	drift->SetSpeed(drift->GetRotationRate(m_rotationCount));
	drift->SetForce(drift->GetRotationRate(m_rotationCount));

	//ノイズデバフ
	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->SetPower(noise->GetRotationRate(m_rotationCount));
	noise->SetNoiseTimer(0.05f);
}

//回転数：21～23回転
void DebuffStage::DebuffStageSeven()
{
	//誘導デバフ
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->SetRandomSpotRadius(magnet->GetRotationRate(m_rotationCount));
	magnet->SetRandomMoveDuration(magnet->GetRotationRate(m_rotationCount));

	//ノイズデバフ
	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->SetPower(noise->GetRotationRate(m_rotationCount));
	noise->SetNoiseTimer(0.05f);
}

//回転数：24～26回転
void DebuffStage::DebuffStageEight()
{
	//遅れデバフ
	auto lag = AddPattern<DebuffLagPattern>();
	lag->SetType(DebuffLagPattern::Delay);
}

//回転数：27～29回転
void DebuffStage::DebuffStageNine()
{
	//反転デバフ
	auto reverse = AddPattern<DebuffReversePattern>();
	reverse->SetType(DebuffReversePattern::Reverse_All);
}

//	回転数：30回転以上
void DebuffStage::DebuffStageTen()
{
	//ノイズデバフ
	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->SetPower(noise->GetRotationRate(m_rotationCount));
	noise->SetNoiseTimer(0.05f);

	//誘導デバフ
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->SetRandomSpotRadius(magnet->GetRotationRate(m_rotationCount));
	magnet->SetRandomMoveDuration(magnet->GetRotationRate(m_rotationCount));

	//揺れデバフ
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->SetPower(shake->GetRotationRate(m_rotationCount));
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount));

}
