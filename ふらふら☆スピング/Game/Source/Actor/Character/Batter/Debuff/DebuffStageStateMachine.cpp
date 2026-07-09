#include "stdafx.h"
#include "Source/Actor/ActorStateMachine.h"
#include "DebuffStageStateMachine.h"
#include "Source/Actor/Character/Batter/Batter.h" 
#include "DebuffStage/DebuffStage.h"
#include "Source/Scene/InGame/Game.h"

struct DebuffMasterData {
	int rotationPerLevel; // 1ステージ上昇に必要なぐるぐるバットの回転数
	int maxLevel;         // その難易度におけるデバフの最大レベル上限
};

// 難易度 [Easy, Normal, Hard] の enum 順に対応したマスタデータテーブル
static const DebuffMasterData g_DebuffMasterTable[] = {
	{ 7, 10 }, // Easy:   3回転で1レベル上昇、最大レベル10
	{ 5, 12 }, // Normal: 3回転で1レベル上昇、最大レベル12
	{ 3, 15 }  // Hard:   3回転で1レベル上昇、最大レベル15
};

// ぐるぐるバットの段階を管理するステートマシンの実装
// ぐるぐるバットの段階は、バッターがぐるぐるバットを振った回数に応じて変化します。
// 例えば、バッターがぐるぐるバットを3回振ったら、デバフステージ1に移行し、6回振ったらデバフステージ2に移行する、といった具合です。
// デバフステージが上がるごとに、バッターのカーソルがより大きく揺れるようになり、打撃が難しくなります。
// デバフステージの段階を管理するステートマシンの実装
DebuffStageStateMachine::DebuffStageStateMachine()
{
	RegisterState<DebuffStageState>();
	m_currentState = FindState(DebuffStageState::ID());

}

DebuffStageStateMachine::~DebuffStageStateMachine()
{
	
}

// Update関数は、現在のステートのRequestState関数を呼び出して、
// 次のステートへの移行をリクエストします。もしRequestStateがtrueを返した場合、
// 現在のステートのExit関数を呼び出し、新しいステートに移行してEnter関数を呼び出します。
// その後、現在のステートのUpdate関数を呼び出します。
void DebuffStageStateMachine::Update()
{
	_ASSERT(m_currentState != nullptr);
	if (m_currentState) {
		uint32_t request;
		if (m_currentState->RequestState(request)) {
			m_currentState->Exit();
			m_currentState = FindState(request);
			m_currentState->Enter();
		}
		m_currentState->Update();
	}
}

// DebuffStageStateクラスの実装
void DebuffStageState::Enter()
{
	auto debuffStageManager = GetDebuffStageManager();
	auto batter = GetBatter();


	_ASSERT(debuffStageManager != nullptr);

	if (!debuffStageManager)
		return;
	batter->ResetCursorOffset();
	debuffStageManager->ClearPatterns();
	debuffStageManager->RebuildStage(m_stageLevel, batter->GetGuruGuru(),batter);

}

// Update関数は、バッターのぐるぐるバットの回数に応じて、デバフステージの段階を更新します。
// もし段階が変化した場合は、デバフステージを再構築します。
void DebuffStageState::Update()
{
	Batter* batter = GetBatter();
	auto debuffStageManager = GetDebuffStageManager();

	_ASSERT(batter != nullptr);
	_ASSERT(debuffStageManager != nullptr);

	if (!batter || !debuffStageManager)
		return;

	Difficulty currentDiff = Difficulty::Normal; // ヌルチェック落ちした際の安全用デフォルト
	if (batter->GetGame()) {
		currentDiff = batter->GetGame()->GetDifficulty();
	}

	// 難易度の enum 値をテーブルのインデックスとして使用
	const auto& master = g_DebuffMasterTable[static_cast<int>(currentDiff)];

	// 難易度に応じた回転数の基準で現在のレベルを算出
	int level = batter->GetGuruGuru() / master.rotationPerLevel;

	// 難易度ごとの最大レベルでクランプ
	level = min(level, master.maxLevel);

	// 算出されたデバフステージの段階に変更があった場合のみ、ステージを再構築します。
	if (level != m_stageLevel)
	{
		m_stageLevel = level;
		debuffStageManager->Initialize(batter);
		debuffStageManager->ClearPatterns();
		debuffStageManager->RebuildStage(m_stageLevel, batter->GetGuruGuru(), batter);
	}

	debuffStageManager->Update(batter);
}

void DebuffStageState::Exit()
{
	auto debuffStageManager = GetDebuffStageManager();
	debuffStageManager->ClearPatterns();
}


// この関数は、現在のぐるぐるバットの回数に応じて、次のステージへの移行をリクエストする役割を果たします。
bool DebuffStageState::RequestState(uint32_t& request)
{

	return false; // ステートの移行はない
}
