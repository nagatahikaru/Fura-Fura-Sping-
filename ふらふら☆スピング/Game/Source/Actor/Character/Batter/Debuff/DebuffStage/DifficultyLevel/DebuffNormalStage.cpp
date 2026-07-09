#include "stdafx.h"
#include "DebuffNormalStage.h"
#include "Source/Actor/Character/Batter/Batter.h"
#include "Source/Actor/Character/Batter/Debuff/DebuffStage/DebuffStage.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/IDebuffPattern.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffShakePattern.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffDriftPattern.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffNoisePattern.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffMagnetPattern.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/DebuffPatternBase.h"


DebuffNormalStage::DebuffNormalStage()
{

}

DebuffNormalStage::~DebuffNormalStage()
{
	ClearPatterns();
}

void DebuffNormalStage::Update(Batter* batter)
{
	for (auto& pattern : m_patterns)
	{
		pattern->Update(batter);
	}
}

void DebuffNormalStage::BuildStage(int level)
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
	case 11:
		DebuffStageEleven();
		break;
	case 12:
		DebuffStageTwelve();
		break;
	default:
		return;
		break;
	}
}

// Normal“ïˆÕ“xFShake / Magnet / Noise / Drift ‚Ì4í‚Å\¬‚µ‚½12’iŠK
// ‰ñ“]”F3`5‰ñ“]
// Easy‚Æ“¯‚¶“±“üB—h‚êƒfƒoƒt‚Ì‚İEã‚ßB
void DebuffNormalStage::DebuffStageOne()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Vertical);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 0.6f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 0.6f);

	m_inGameUI->SetDebuffComment(L"c—h‚ê(ã)");
}

// ‰ñ“]”F6`8‰ñ“]
// —h‚êƒfƒoƒt‚ğ‹­‰»B
void DebuffNormalStage::DebuffStageTwo()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 0.8f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 0.8f);

	m_inGameUI->SetDebuffComment(L"ƒ‰ƒ“ƒ_ƒ€—h‚ê(ã)");
}

// ‰ñ“]”F9`11‰ñ“]
// ‚±‚±‚ÅƒmƒCƒYƒfƒoƒt‚ğ‰“oêB—h‚ê‚Æ‘g‚İ‡‚í‚¹‚éB
void DebuffNormalStage::DebuffStageThree()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 0.8f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 0.8f);

	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Vertical);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 0.7f);
	noise->SetNoiseTimer(0.05f);

	m_inGameUI->SetDebuffComment(L"—h‚êƒmƒCƒY(ã)");
}

// ‰ñ“]”F12`14‰ñ“]
// ƒmƒCƒY‚ğ‰¡•ûŒü‚ÉØ‚è‘Ö‚¦‚Â‚ÂA—U“±ƒfƒoƒt‚ğ‰“oê‚³‚¹‚éB
void DebuffNormalStage::DebuffStageFour()
{
	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Horizontal);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 0.8f);
	noise->SetNoiseTimer(0.05f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Vertical);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 0.5f);
	magnet->SetRandomMoveDuration(m_rotationCount * 0.5f);

	m_inGameUI->SetDebuffComment(L"ƒmƒCƒY—U“±(ã)");
}

// ‰ñ“]”F15`17‰ñ“]
// —h‚ê + —U“±(‰¡)‚Ì‘g‚İ‡‚í‚¹B
void DebuffNormalStage::DebuffStageFive()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.0f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.0f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Horizontal);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 0.8f);
	magnet->SetRandomMoveDuration(m_rotationCount * 0.8f);

	m_inGameUI->SetDebuffComment(L"—h‚ê—U“±(’†)");
}

// ‰ñ“]”F18`20‰ñ“]
// ‚±‚±‚Å—¬‚³‚êƒfƒoƒt‚ğ‰“oê‚³‚¹A—U“±‚Æ‘g‚İ‡‚í‚¹‚éB
void DebuffNormalStage::DebuffStageSix()
{
	auto drift = AddPattern<DebuffDriftPattern>();
	drift->SetType(DebuffDriftPattern::Random);
	drift->Reset();
	drift->SetSpeed(drift->GetRotationRate(m_rotationCount) * 0.8f);
	drift->SetForce(drift->GetRotationRate(m_rotationCount) * 0.8f);
	drift->SetWaveSpeed(drift->GetRotationRate(m_rotationCount) * 0.8f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount);
	magnet->SetRandomMoveDuration(m_rotationCount);

	m_inGameUI->SetDebuffComment(L"—¬‚³‚ê—U“±(ã)");
}

// ‰ñ“]”F21`23‰ñ“]
// —h‚ê + ƒmƒCƒY + —U“±‚Ì3í“¯”­“®B
void DebuffNormalStage::DebuffStageSeven()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.1f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.1f);

	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 0.9f);
	noise->SetNoiseTimer(0.05f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount);
	magnet->SetRandomMoveDuration(m_rotationCount);

	m_inGameUI->SetDebuffComment(L"—h‚êƒmƒCƒY—U“±(’†)");
}

// ‰ñ“]”F24`26‰ñ“]
// —¬‚³‚êƒfƒoƒt‚ğ‹­‰»‚µAƒmƒCƒY‚Æ‘g‚İ‡‚í‚¹‚éB
void DebuffNormalStage::DebuffStageEight()
{
	auto drift = AddPattern<DebuffDriftPattern>();
	drift->SetType(DebuffDriftPattern::Random);
	drift->Reset();
	drift->SetSpeed(drift->GetRotationRate(m_rotationCount) * 1.1f);
	drift->SetForce(drift->GetRotationRate(m_rotationCount) * 1.1f);
	drift->SetWaveSpeed(drift->GetRotationRate(m_rotationCount) * 1.1f);

	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 1.0f);
	noise->SetNoiseTimer(0.05f);

	m_inGameUI->SetDebuffComment(L"—¬‚³‚êƒmƒCƒY(‹­)");
}

// ‰ñ“]”F27`29‰ñ“]
// —h‚ê(‹­) + —¬‚³‚ê + —U“±‚Ì‘g‚İ‡‚í‚¹B
void DebuffNormalStage::DebuffStageNine()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.2f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.2f);

	auto drift = AddPattern<DebuffDriftPattern>();
	drift->SetType(DebuffDriftPattern::Random);
	drift->Reset();
	drift->SetSpeed(drift->GetRotationRate(m_rotationCount) * 1.0f);
	drift->SetForce(drift->GetRotationRate(m_rotationCount) * 1.0f);
	drift->SetWaveSpeed(drift->GetRotationRate(m_rotationCount) * 1.0f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 1.1f);
	magnet->SetRandomMoveDuration(m_rotationCount * 1.1f);

	m_inGameUI->SetDebuffComment(L"—h‚ê—¬‚³‚ê—U“±(‹­)");
}

// ‰ñ“]”F30`32‰ñ“]
// Shake / Noise / Drift / Magnet ‚Ì4í‚ğ‰‚ß‚Ä“¯‚É”­“®B
void DebuffNormalStage::DebuffStageTen()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.1f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.1f);

	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 0.9f);
	noise->SetNoiseTimer(0.05f);

	auto drift = AddPattern<DebuffDriftPattern>();
	drift->SetType(DebuffDriftPattern::Random);
	drift->Reset();
	drift->SetSpeed(drift->GetRotationRate(m_rotationCount) * 0.9f);
	drift->SetForce(drift->GetRotationRate(m_rotationCount) * 0.9f);
	drift->SetWaveSpeed(drift->GetRotationRate(m_rotationCount) * 0.9f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount);
	magnet->SetRandomMoveDuration(m_rotationCount);

	m_inGameUI->SetDebuffComment(L"—h‚êƒmƒCƒY—¬‚³‚ê—U“±(‹­)");
}

// ‰ñ“]”F33`35‰ñ“]
// 4í‘S•”‚ğ‚³‚ç‚É‹­‰»B
void DebuffNormalStage::DebuffStageEleven()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.3f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.3f);

	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 1.1f);
	noise->SetNoiseTimer(0.04f);

	auto drift = AddPattern<DebuffDriftPattern>();
	drift->SetType(DebuffDriftPattern::Random);
	drift->Reset();
	drift->SetSpeed(drift->GetRotationRate(m_rotationCount) * 1.1f);
	drift->SetForce(drift->GetRotationRate(m_rotationCount) * 1.1f);
	drift->SetWaveSpeed(drift->GetRotationRate(m_rotationCount) * 1.1f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 1.2f);
	magnet->SetRandomMoveDuration(m_rotationCount * 1.2f);

	m_inGameUI->SetDebuffComment(L"—h‚êƒmƒCƒY—¬‚³‚ê—U“±(Å‘å)");
}

// ‰ñ“]”F36‰ñ“]ˆÈã
// NormalÅIŒ`‘ÔB4í‚·‚×‚Ä‚ğÅ‘å’l•t‹ß‚Å’@‚«‚ŞB
void DebuffNormalStage::DebuffStageTwelve()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.5f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.5f);

	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 1.3f);
	noise->SetNoiseTimer(0.03f);

	auto drift = AddPattern<DebuffDriftPattern>();
	drift->SetType(DebuffDriftPattern::Random);
	drift->Reset();
	drift->SetSpeed(drift->GetRotationRate(m_rotationCount) * 1.3f);
	drift->SetForce(drift->GetRotationRate(m_rotationCount) * 1.3f);
	drift->SetWaveSpeed(drift->GetRotationRate(m_rotationCount) * 1.3f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 1.4f);
	magnet->SetRandomMoveDuration(m_rotationCount * 1.4f);

	m_inGameUI->SetDebuffComment(L"—h‚êƒmƒCƒY—¬‚³‚ê—U“±(‹É)");
}