#pragma once
#include "Source/Actor/Character/Batter/Debuff/Pattern/IDebuffPattern.h"
#include <vector>

class Batter;

class DebuffStage 
{
protected:
    // デバフパターンのベクター
    std::vector<std::unique_ptr<IDebuffPattern>> m_patterns;

    // 回転数
    int m_rotationCount = 0;

	public:
	DebuffStage();
    virtual ~DebuffStage() { ClearPatterns(); };
    
	void Update(Batter* batter);

    void ClearPatterns()
    {
        m_patterns.clear();        
    }

	// デバフパターンを追加するためのテンプレート関数
	// この関数は、指定されたデバフパターンのインスタンスを作成し、m_patternsベクターに追加します。
    template<typename T>
    T* AddPattern()
    {
        auto pattern =
            std::make_unique<T>();

        T* ptr = pattern.get();

        m_patterns.push_back(
            std::move(pattern));

        return ptr;
    }
	 virtual void BuildStage(int level) = 0;

    void SetRotationCount(int count)
    {
        m_rotationCount = count;
	}

};

