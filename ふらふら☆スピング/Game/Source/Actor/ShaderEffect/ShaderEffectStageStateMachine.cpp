#include "stdafx.h"
#include "Source/Actor/ActorStateMachine.h"
#include "ShaderEffectStageStateMachine.h"
#include "Source/Actor/Character/Batter/Batter.h" 
#include "ShaderEffectStage/ShaderEffectStage.h"



ShaderEffectStageStateMachine::ShaderEffectStageStateMachine()
{
	//RegisterState<ShaderEffectStageState>();
	//m_currentState = FindState(ShaderEffectStageState::ID());

}

ShaderEffectStageStateMachine::~ShaderEffectStageStateMachine()
{
	
}

// Update関数は、現在のステートのRequestState関数を呼び出して、
// 次のステートへの移行をリクエストします。もしRequestStateがtrueを返した場合、
// 現在のステートのExit関数を呼び出し、新しいステートに移行してEnter関数を呼び出します。
// その後、現在のステートのUpdate関数を呼び出します。
void ShaderEffectStageStateMachine::Update()
{
	//_ASSERT(m_currentState != nullptr);
	//if (m_currentState) {
	//	uint32_t request;
	//	if (m_currentState->RequestState(request)) {
	//		m_currentState->Exit();
	//		m_currentState = FindState(request);
	//		m_currentState->Enter();
	//	}
	//	m_currentState->Update();
	//}
}

// ShaderEffectStateクラスの実装
void ShaderEffectStageState::Enter()
{
	//auto ShaderEffectStage = GetShaderEffectStage();


	//_ASSERT(debuffStage != nullptr);

	//if (!ShaderEffectStage)
	//	return;
	//ShaderEffectStage->ClearPatterns();
	//ShaderEffectStage->BuildStage(m_stageLevel);
}

// Update関数は、
void ShaderEffectStageState::Update()
{
	//auto ShaderEffectStage = GetShaderEffectStage();

	//_ASSERT(ShaderEffectStage != nullptr);

	//int level = 0;

	//if (level != m_stageLevel)
	//{
	//	m_stageLevel = level;
	//	
	//	ShaderEffectStage->ClearPatterns();
	//	
	//	ShaderEffectStage->BuildStage(m_stageLevel);
	//}

	//ShaderEffectStage->Update();
}

void ShaderEffectStageState::Exit()
{
	//GetShaderEffectStage()->ClearPatterns();
}


// この関数は、
bool ShaderEffectStageState::RequestState(uint32_t& request)
{

	return false; // ステートの移行はない
}
