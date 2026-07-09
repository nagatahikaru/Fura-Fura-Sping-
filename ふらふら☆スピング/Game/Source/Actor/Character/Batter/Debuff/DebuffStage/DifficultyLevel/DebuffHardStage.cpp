#include "stdafx.h"
#include "DebuffHardStage.h"
#include "Source/Actor/Character/Batter/Debuff/DebuffStage/DebuffStage.h"
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


DebuffHardStage::DebuffHardStage()
{

}

DebuffHardStage::~DebuffHardStage()
{
	ClearPatterns();
}

void DebuffHardStage::Update(Batter* batter)
{

	for (auto& pattern : m_patterns)
	{
		pattern->Update(batter);
	}
}

void DebuffHardStage::BuildStage(int level)
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
	case 13:
		DebuffStageThirteen();
		break;
	case 14:
		DebuffStageFourteen();
		break;
	case 15:
		DebuffStageFifteen();
		break;
	default:
		return;
		break;
	}
}

// Hard難易度：全7種（Shake, Noise, Drift, Magnet, Aim, Lag, Reverse）を使い切る15段階
// 回転数：3～5回転
// Easy/Normalと同じ導入。揺れデバフのみ・弱め。
void DebuffHardStage::DebuffStageOne()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Vertical);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 0.6f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 0.6f);

	m_inGameUI->SetDebuffComment(L"カーソルが少し震え始めた…");
}

// 回転数：6～8回転
// ノイズデバフを初登場。揺れと組み合わせる。
void DebuffHardStage::DebuffStageTwo()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 0.8f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 0.8f);

	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Vertical);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 0.6f);
	noise->SetNoiseTimer(0.05f);

	m_inGameUI->SetDebuffComment(L"揺れに加えてノイズも出始めた!");
}

// 回転数：9～11回転
// 誘導デバフを初登場させ、揺れ・ノイズと3種同時に。
void DebuffHardStage::DebuffStageThree()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 0.9f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 0.9f);

	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Vertical);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 0.7f);
	noise->SetNoiseTimer(0.05f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Vertical);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 0.5f);
	magnet->SetRandomMoveDuration(m_rotationCount * 0.5f);

	m_inGameUI->SetDebuffComment(L"揺れ・ノイズ・誘導が同時に来た!");
}

// 回転数：12～14回転
// 流されデバフを初登場させ、誘導(横)と組み合わせる。
void DebuffHardStage::DebuffStageFour()
{
	auto drift = AddPattern<DebuffDriftPattern>();
	drift->SetType(DebuffDriftPattern::Random);
	drift->Reset();
	drift->SetSpeed(drift->GetRotationRate(m_rotationCount) * 0.8f);
	drift->SetForce(drift->GetRotationRate(m_rotationCount) * 0.8f);
	drift->SetWaveSpeed(drift->GetRotationRate(m_rotationCount) * 0.8f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Horizontal);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 0.7f);
	magnet->SetRandomMoveDuration(m_rotationCount * 0.7f);

	m_inGameUI->SetDebuffComment(L"入力が流されるようになってきた!");
}

// 回転数：15～17回転
// 照準デバフ（カーソル縮小）を初登場させ、揺れ・誘導と組み合わせる。
void DebuffHardStage::DebuffStageFive()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.0f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.0f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 0.8f);
	magnet->SetRandomMoveDuration(m_rotationCount * 0.8f);

	auto aim = AddPattern<DebuffAimPattern>();
	aim->Reset();
	aim->SetType(DebuffAimPattern::SmallCursor);
	aim->SetMeatRange(m_rotationCount * 0.6f);

	m_inGameUI->SetDebuffComment(L"カーソルまで小さくなって当てづらい!");
}

// 回転数：18～20回転
// 流されデバフを強化し、揺れ・誘導と組み合わせる。
void DebuffHardStage::DebuffStageSix()
{
	auto drift = AddPattern<DebuffDriftPattern>();
	drift->SetType(DebuffDriftPattern::Random);
	drift->Reset();
	drift->SetSpeed(drift->GetRotationRate(m_rotationCount) * 1.1f);
	drift->SetForce(drift->GetRotationRate(m_rotationCount) * 1.1f);
	drift->SetWaveSpeed(drift->GetRotationRate(m_rotationCount) * 1.1f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount);
	magnet->SetRandomMoveDuration(m_rotationCount);

	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.1f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.1f);

	m_inGameUI->SetDebuffComment(L"流され・誘導・揺れのトリプルパンチ!");
}

// 回転数：21～23回転
// 誘導・ノイズ・揺れ(強)の組み合わせ。
void DebuffHardStage::DebuffStageSeven()
{
	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount);
	magnet->SetRandomMoveDuration(m_rotationCount);

	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 0.9f);
	noise->SetNoiseTimer(0.05f);

	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.2f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.2f);

	m_inGameUI->SetDebuffComment(L"誘導・ノイズ・揺れが一気に押し寄せる!");
}

// 回転数：24～26回転
// 遅延デバフを初登場させる。単体で見せることで違和感を強く感じさせる。
void DebuffHardStage::DebuffStageEight()
{
	auto lag = AddPattern<DebuffLagPattern>();
	lag->SetType(DebuffLagPattern::Delay);
	lag->Reset();

	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Vertical);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 0.5f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 0.5f);

	m_inGameUI->SetDebuffComment(L"入力への反応が遅れるようになった…!");
}

// 回転数：27～29回転
// 反転デバフを初登場させる。単体発動でインパクトを重視。
// ※ DebuffReversePattern::Reset は Batter* を要求するため、BuildStage内では呼び出さない。
void DebuffHardStage::DebuffStageNine()
{
	auto reverse = AddPattern<DebuffReversePattern>();
	reverse->SetType(DebuffReversePattern::Reverse_All);

	m_inGameUI->SetDebuffComment(L"まさかの入力反転!右と左が逆になった!");
}

// 回転数：30～32回転
// ノイズ・誘導・揺れの強力な組み合わせ（Normalの最終形と同等）。
void DebuffHardStage::DebuffStageTen()
{
	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 1.0f);
	noise->SetNoiseTimer(0.05f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount);
	magnet->SetRandomMoveDuration(m_rotationCount);

	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.2f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.2f);

	m_inGameUI->SetDebuffComment(L"ノイズ・誘導・揺れの強力コンボ!");
}

// 回転数：33～35回転
// 遅延デバフを誘導・ノイズと組み合わせ、じわじわ追い込む。
void DebuffHardStage::DebuffStageEleven()
{
	auto lag = AddPattern<DebuffLagPattern>();
	lag->SetType(DebuffLagPattern::Delay);
	lag->Reset();

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 1.1f);
	magnet->SetRandomMoveDuration(m_rotationCount * 1.1f);

	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 0.9f);
	noise->SetNoiseTimer(0.045f);

	m_inGameUI->SetDebuffComment(L"遅延・誘導・ノイズでどんどん狂わされる!");
}

// 回転数：36～38回転
// 反転デバフを揺れ・誘導と組み合わせる。
void DebuffHardStage::DebuffStageTwelve()
{
	auto reverse = AddPattern<DebuffReversePattern>();
	reverse->SetType(DebuffReversePattern::Reverse_All);

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

	m_inGameUI->SetDebuffComment(L"反転しながら揺れて誘導までされる!");
}

// 回転数：39～41回転
// 照準・流され・ノイズの組み合わせで、当てにくさを最大限に。
void DebuffHardStage::DebuffStageThirteen()
{
	auto aim = AddPattern<DebuffAimPattern>();
	aim->Reset();
	aim->SetType(DebuffAimPattern::SmallCursor);
	aim->SetMeatRange(m_rotationCount * 0.8f);

	auto drift = AddPattern<DebuffDriftPattern>();
	drift->SetType(DebuffDriftPattern::Random);
	drift->Reset();
	drift->SetSpeed(drift->GetRotationRate(m_rotationCount) * 1.2f);
	drift->SetForce(drift->GetRotationRate(m_rotationCount) * 1.2f);
	drift->SetWaveSpeed(drift->GetRotationRate(m_rotationCount) * 1.2f);

	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 1.1f);
	noise->SetNoiseTimer(0.04f);

	m_inGameUI->SetDebuffComment(L"カーソルは小さく、当たり判定も流されまくる!");
}

// 回転数：42～44回転
// 遅延・反転・揺れの組み合わせ。入力そのものが信用できなくなる領域。
void DebuffHardStage::DebuffStageFourteen()
{
	auto lag = AddPattern<DebuffLagPattern>();
	lag->SetType(DebuffLagPattern::Delay);
	lag->Reset();

	auto reverse = AddPattern<DebuffReversePattern>();
	reverse->SetType(DebuffReversePattern::Reverse_All);

	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.3f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.3f);

	m_inGameUI->SetDebuffComment(L"遅延・反転・揺れ…もう自分の入力が信じられない!");
}

// 回転数：45回転以上
// Hard最終形態。全7種のデバフを一斉に組み合わせた最終局面。
void DebuffHardStage::DebuffStageFifteen()
{
	auto shake = AddPattern<DebuffShakePattern>();
	shake->SetType(DebuffShakePattern::Shake_Random);
	shake->Reset();
	shake->SetPower(shake->GetRotationRate(m_rotationCount) * 1.4f);
	shake->SetSeismicIntensity(shake->GetRotationRate(m_rotationCount) * 1.4f);

	auto noise = AddPattern<DebuffNoisePattern>();
	noise->SetType(DebuffNoisePattern::Noise_Random);
	noise->Reset();
	noise->SetPower(noise->GetRotationRate(m_rotationCount) * 1.2f);
	noise->SetNoiseTimer(0.035f);

	auto drift = AddPattern<DebuffDriftPattern>();
	drift->SetType(DebuffDriftPattern::Random);
	drift->Reset();
	drift->SetSpeed(drift->GetRotationRate(m_rotationCount) * 1.2f);
	drift->SetForce(drift->GetRotationRate(m_rotationCount) * 1.2f);
	drift->SetWaveSpeed(drift->GetRotationRate(m_rotationCount) * 1.2f);

	auto magnet = AddPattern<DebuffMagnetPattern>();
	magnet->SetType(DebuffMagnetPattern::Random);
	magnet->Reset();
	magnet->SetRandomSpotRadius(m_rotationCount * 1.3f);
	magnet->SetRandomMoveDuration(m_rotationCount * 1.3f);

	auto aim = AddPattern<DebuffAimPattern>();
	aim->Reset();
	aim->SetType(DebuffAimPattern::SmallCursor);
	aim->SetMeatRange(m_rotationCount * 0.9f);

	auto lag = AddPattern<DebuffLagPattern>();
	lag->SetType(DebuffLagPattern::Delay);
	lag->Reset();

	auto reverse = AddPattern<DebuffReversePattern>();
	reverse->SetType(DebuffReversePattern::Reverse_All);

	m_inGameUI->SetDebuffComment(L"全てのデバフが同時に襲いかかる…伝説のデバフここに極まれり!");
}