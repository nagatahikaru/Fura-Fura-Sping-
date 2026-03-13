#include "stdafx.h"
#include "Source/Actor/ActorStateMachine.h"
#include "BatterStateMachine.h"
#include "Batter.h" 

namespace {
	float LSTICK_ZERO_THRESHOLD = 0.000001f;	
}


BatterStateMachine::BatterStateMachine()
{
	RegisterState<BatterIdleState>();
	RegisterState<BatterMoveState>();	
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
	Batter* player = GetBatter();
	player->SetPlayAnimation(player->GetEnAnimationClip());
}

void BatterIdleState::Exit()
{

}

//待機状態からの状態遷移判定。
//スティック入力で移動状態。
//戻り値：状態遷移が発生したらtrue、しなかったらfalseを返す。
bool BatterIdleState::RequestState(uint32_t& request)
{
	if (g_pad[0]->GetLStickXF() >= LSTICK_ZERO_THRESHOLD ||
		g_pad[0]->GetLStickYF() >= LSTICK_ZERO_THRESHOLD)
	{
		request = BatterMoveState::ID();
		return true;
	}
	return false;
}

void BatterMoveState::Enter()
{

}

void BatterMoveState::Update()
{
	Batter* batter = GetBatter();
	batter->Move();
	batter->MoveUpdate();
	batter->SetPlayAnimation(batter->GetEnAnimationClip());
}

void BatterMoveState::Exit()
{

}

bool BatterMoveState::RequestState(uint32_t&request)
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