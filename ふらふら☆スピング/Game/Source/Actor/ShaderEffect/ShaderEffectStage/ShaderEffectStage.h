#pragma once
#include "Source/Actor/ShaderEffect/Pattern/IShaderEffectPattern.h"
#include <vector>

class Batter;

class ShaderEffectStage
{
	public:
        ShaderEffectStage();
	~ShaderEffectStage();

	void Update();
    ShaderEffectStage*GetShaderEffectStage()
    {
        return this;
    }
	
    void ClearPatterns()
    {
        m_patterns.clear();        
    }

    

	// シェーダー効果パターンを追加するためのテンプレート関数
	// この関数は、指定されたシェーダー効果パターンのインスタンスを作成し、m_patternsベクターに追加します。
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

    void BuildStage(int level);

private:
	// 演出パターンのベクター
    std::vector<
        std::unique_ptr<IShaderEffectPattern>
    > m_patterns;
};

