#include "stdafx.h"
#include "Source/Actor/ActorStateMachine.h"
#include "DebuffStageStateMachine.h"
#include "Source/Actor/Character/Batter/Batter.h" 
#include "DebuffStage/DebuffStage.h"


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
	auto debuffStage = GetDebuffStage();

	_ASSERT(debuffStage != nullptr);

	if (!debuffStage)
		return;

	debuffStage->ClearPatterns();
	debuffStage->BuildStage(m_stageLevel);
}

// Update関数は、バッターのぐるぐるバットの回数に応じて、デバフステージの段階を更新します。
// もし段階が変化した場合は、デバフステージを再構築します。
void DebuffStageState::Update()
{
	Batter* batter = GetBatter();
	auto debuffStage = GetDebuffStage();

	_ASSERT(batter != nullptr);
	_ASSERT(debuffStage != nullptr);

	if (!batter || !debuffStage)
		return;

	int level = batter->GetGuruGuruBatCount() / 3;

	level = min(level, 10);

	if (level != m_stageLevel)
	{
		m_stageLevel = level;

		debuffStage->ClearPatterns();
		debuffStage->BuildStage(m_stageLevel);
	}

	debuffStage->Update(batter);
}

void DebuffStageState::Exit()
{
	GetDebuffStage()->ClearPatterns();
}


// この関数は、現在のぐるぐるバットの回数に応じて、次のステージへの移行をリクエストする役割を果たします。
bool DebuffStageState::RequestState(uint32_t& request)
{

	return false; // ステートの移行はない
}
