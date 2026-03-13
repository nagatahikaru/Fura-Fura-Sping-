/**
* Actor.h
* 見た目が存在するオブジェクトの基底クラス
*/
#pragma once
#include <map>
#include<vector>
#include <array>
#include "Source/Util/CRC32.h"

#define appState(name)	\
public:\
	static constexpr uint32_t ID() { return Hash32(#name); }

class StatePatternBase;

class IState
{
public:
	IState() {};
	virtual ~IState() {};

	void SetOwner(StatePatternBase* owner)
	{
		m_owner = owner;
	}
protected:
	StatePatternBase* m_owner=nullptr;


public:
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;

	virtual bool RequestState(uint32_t& request) = 0;
};

class StatePatternBase
{
	
protected:
	std::map<uint32_t,IState*> m_stateMap;
	IState* m_currentState;

public:
	StatePatternBase() :m_currentState(nullptr)
	{
		m_stateMap.clear();
	}
	virtual ~StatePatternBase()
	{
		for (auto it : m_stateMap)
		{
			delete it.second;
			it.second = nullptr;
		}
		m_stateMap.clear();
	}
	virtual void Update() = 0;

protected:
	inline IState* FindState(uint32_t id)
	{
		const auto& it = m_stateMap.find(id);
		if(it==m_stateMap.end())
		{
			return nullptr;
		}
		return it->second;
	}

public:
	template<typename T>
	inline void RegisterState()
	{
		T* state = new T();
		state->SetOwner(this);
		m_stateMap.emplace(T::ID(),state);
	}

	template<typename T>
	inline void InitializeState()
	{
		m_currentState = FindState(T::ID());
	}
};