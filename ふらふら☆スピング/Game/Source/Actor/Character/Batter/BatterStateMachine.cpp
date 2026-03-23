#include "stdafx.h"
#include "Source/Actor/ActorStateMachine.h"
#include "BatterStateMachine.h"
#include "Batter.h" 

namespace {
	float LSTICK_MIN_THRESHOLD = 0.000001f;
	float RSTICK_MIN_THRESHOLD = 0.000001f;
}


BatterStateMachine::BatterStateMachine()
{
	RegisterState<BatterIdleState>();
	RegisterState<BatterRotationState>();
	RegisterState<BatterSwingState>();
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
	Batter* batter = GetBatter();
	batter->SetPlayAnimation(batter->GetEnAnimationClip());
}

void BatterIdleState::Update()
{
	Batter* batter = GetBatter();
	batter->AnimationUpdate();
}

void BatterIdleState::Exit()
{

}

//待機状態からの状態遷移判定。
//スティック入力で回転状態。
//戻り値：状態遷移が発生したらtrue、しなかったらfalseを返す。
bool BatterIdleState::RequestState(uint32_t& request)
{
	Batter* batter = GetBatter();

	if (fabs(g_pad[0]->GetLStickXF()) >= LSTICK_MIN_THRESHOLD ||
		fabs(g_pad[0]->GetLStickYF()) >= RSTICK_MIN_THRESHOLD)
	{
		request = BatterRotationState::ID();
		return true;
	}
	if (g_pad[0]->IsTrigger(enButtonA))
	{		
		request = BatterSwingState::ID();
		return true;
	}
	return false;
}

void BatterRotationState::Enter()
{
	Batter* batter = GetBatter();
	batter->SetPlayRotation();
	batter->SetPlayAnimation(batter->GetEnAnimationClip());
}

void BatterRotationState::Update()
{
	Batter* batter = GetBatter();
	batter->Rotation();
	batter->RotationUpdate();
	batter->AnimationUpdate();
}

void BatterRotationState::Exit()
{

}

bool BatterRotationState::RequestState(uint32_t& request)
{
	float lx = g_pad[0]->GetLStickXF();
	float ly = g_pad[0]->GetLStickYF();
	Batter* batter = GetBatter();

	if (lx< 0.0f && ly < 0.0f)
	{
		request = BatterIdleState::ID();
		return true;
	}
	if(g_pad[0]->IsTrigger(enButtonA))
	{
		request = BatterSwingState::ID();
		return true;
	}

	return false;
}

void BatterSwingState::Enter()
{
	Batter* batter = GetBatter();
	batter->Swing();
	batter->SetPlayAnimation(batter->GetEnAnimationClip());
}

void BatterSwingState::Update()
{
	Batter* batter = GetBatter();
	batter->AnimationUpdate();
}

void BatterSwingState::Exit()
{
}

bool BatterSwingState::RequestState(uint32_t& request)
{
	Batter* batter = GetBatter();
	if (batter->IsSwingAnimationPlaying())
	{
		return false;		
	}
	float lx = g_pad[0]->GetLStickXF();
	float ly = g_pad[0]->GetLStickYF();

	if (lx < 0.0f && ly < 0.0f)
	{
		request = BatterIdleState::ID();
		return true;
	}
	return false;
}