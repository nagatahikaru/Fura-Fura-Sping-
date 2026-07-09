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

// Easy“ïˆÕ“xFShakei—h‚êj‚ÆMagneti—U“±j‚Ì‚İ‚Å\¬‚µ‚½10’iŠK
// ‰ñ“]”F3`5‰ñ“]
// —h‚êƒfƒoƒt‚Ì‚İEã‚ßB‚Ü‚¸‚Íˆá˜aŠ´‚ğŠ´‚¶‚³‚¹‚é’ö“xB
void DebuffNormalStage::DebuffStageOne()
{
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Horizontal);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 0.8f);
	magnet->SetRandomMoveDuration(m_rotationCount * 0.8f);

	m_inGameUI->SetDebuffComment(L"‰¡—U“±(ã)");
}

// ‰ñ“]”F6`8‰ñ“]
// —h‚êƒfƒoƒt‚ğ‹­‰»‚µA•ûŒü‚àƒ‰ƒ“ƒ_ƒ€‚ÉB
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
// ‚±‚±‚Å—U“±ƒfƒoƒt‚ğ‰“oê‚³‚¹A—h‚ê‚Æ‘g‚İ‡‚í‚¹‚éB
void DebuffNormalStage::DebuffStageThree()
{
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Horizontal);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 2.0f);
	magnet->SetRandomMoveDuration(m_rotationCount * 2.0f);

	m_inGameUI->SetDebuffComment(L"‰¡—U“±(’†)");
}

// ‰ñ“]”F12`14‰ñ“]
// —U“±ƒfƒoƒt’P‘Ì‚ğ¶‰E•ûŒü‚ÉØ‚è‘Ö‚¦A”ÍˆÍ‚ğL‚°‚éB
void DebuffNormalStage::DebuffStageFour()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.0f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.0f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Vertical);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 1.0f);
	magnet->SetRandomMoveDuration(m_rotationCount * 1.0f);

	m_inGameUI->SetDebuffComment(L"—h‚ê—U“±(’†)");


}

// ‰ñ“]”F15`17‰ñ“]
// —h‚ê + —U“±(‰¡)‚Ì‘g‚İ‡‚í‚¹‚Å•‰‰×‚ğã‚°‚éB
void DebuffNormalStage::DebuffStageFive()
{
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 1.2f);
	magnet->SetRandomMoveDuration(m_rotationCount * 1.2f);

	m_inGameUI->SetDebuffComment(L"ƒ‰ƒ“ƒ_ƒ€—U“±(’†)");
}

// ‰ñ“]”F18`20‰ñ“]
// —U“±‚ğƒ‰ƒ“ƒ_ƒ€•ûŒü‚É‚µA—\‘ª‚µ‚Ã‚ç‚­‚·‚éB
void DebuffNormalStage::DebuffStageSix()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.0f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.0f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Horizontal);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 1.2f);
	magnet->SetRandomMoveDuration(m_rotationCount * 1.2f);

	m_inGameUI->SetDebuffComment(L"—h‚ê‰¡—U“±(’†)");
}

// ‰ñ“]”F21`23‰ñ“]
// —h‚ê(‹­) + —U“±(ƒ‰ƒ“ƒ_ƒ€)‚Ì“¯”­“®B
void DebuffNormalStage::DebuffStageSeven()
{
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 10.0f);
	magnet->SetRandomMoveDuration(m_rotationCount * 10.0f);

	m_inGameUI->SetDebuffComment(L"ƒ‰ƒ“ƒ_ƒ€—U“±(‹­)");
}

// ‰ñ“]”F24`26‰ñ“]
// —U“±ƒfƒoƒt‚Ì”ÍˆÍE•p“x‚ğÅ‘å•t‹ß‚Ü‚Åˆø‚«ã‚°‚éB
void DebuffNormalStage::DebuffStageEight()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.0f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.0f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount);
	magnet->SetRandomMoveDuration(m_rotationCount);

	m_inGameUI->SetDebuffComment(L"—h‚êƒ‰ƒ“ƒ_ƒ€—U“±(‹­)");


}

// ‰ñ“]”F27`29‰ñ“]
// —h‚êE—U“±‚Æ‚à‚É‚Ù‚ÚÅ‘å’l‚É‹ß‚Ã‚¯‚éB
void DebuffNormalStage::DebuffStageNine()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.1f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.1f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 1.1f);
	magnet->SetRandomMoveDuration(m_rotationCount * 1.1f);

	m_inGameUI->SetDebuffComment(L"—h‚êƒ‰ƒ“ƒ_ƒ€—U“±(Å‘å)");
}

// ‰ñ“]”F30‰ñ“]ˆÈã
// EasyÅIŒ`‘ÔB—h‚êE—U“±‚Æ‚à‚ÉÅ‘å’l‚ÅP‚¢‚©‚©‚éB
void DebuffNormalStage::DebuffStageTen()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.2f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.2f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 1.2f);
	magnet->SetRandomMoveDuration(m_rotationCount * 1.2f);

	m_inGameUI->SetDebuffComment(L"—h‚êƒ‰ƒ“ƒ_ƒ€—U“±(‹É)");
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