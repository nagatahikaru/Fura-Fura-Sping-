#include "stdafx.h"
#include "Source/Actor/ActorStateMachine.h"
#include "BatterStateMachine.h"
#include "Batter.h" 

namespace {
	float LSTICK_MIN_THRESHOLD = 0.000001f;
	float RSTICK_MIN_THRESHOLD = 0.000001f;
	float LSTICK_MINUS_THRESHOLD = -0.000001f;
	float RSTICK_MINUS_THRESHOLD = -0.000001f;
}


BatterStateMachine::BatterStateMachine()
{
	RegisterState<BatterIdleState>();
	RegisterState<BatterRotationState>();	
	m_currentState = FindState(BatterIdleState::ID());

}

BatterStateMachine::~BatterStateMachine()
{
	
}

void BatterStateMachine::Update()
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

void BatterIdleState::Enter()
{ 

}

void BatterIdleState::Update()
{
	Batter* batter = GetBatter();
	batter->SetPlayAnimation(batter->GetEnAnimationClip());
}

void BatterIdleState::Exit()
{

}

//待機状態からの状態遷移判定。
//スティック入力で回転状態。
//戻り値：状態遷移が発生したらtrue、しなかったらfalseを返す。
bool BatterIdleState::RequestState(uint32_t& request)
{
	if (g_pad[0]->GetLStickXF() >= LSTICK_MIN_THRESHOLD ||
		g_pad[0]->GetLStickYF() >= RSTICK_MIN_THRESHOLD||
		g_pad[0]->GetLStickXF() <= LSTICK_MINUS_THRESHOLD||
		g_pad[0]->GetLStickYF() <= RSTICK_MINUS_THRESHOLD)
	{
		request = BatterRotationState::ID();
		return true;
	}
	return false;
}

void BatterRotationState::Enter()
{

}

void BatterRotationState::Update()
{
	Batter* batter = GetBatter();
	batter->Rotation();
	batter->RotationUpdate();
	batter->SetPlayAnimation(batter->GetEnAnimationClip());
}

void BatterRotationState::Exit()
{

}

bool BatterRotationState::RequestState(uint32_t&request)
{
	Batter* batter = GetBatter();

	if(!g_pad[0]->IsPressAnyKey() &&
		batter->GetIsOnGround())
	{
		request = BatterIdleState::ID();
		return true;
	}
	return false;

}