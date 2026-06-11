#pragma once
#include "ShaderEffectBase.h"

/// Patternクラスの実装

class Pattern : public ShaderEffectBase
{
public:

    enum Type
    {
		
    };

    /// <summary>
    /// を設定する関数です。
    /// </summary>
    /// <param name="type"> 種類を指定します。
    /// <para> </para>
    /// <para> </para>
    /// <para> </para>
    /// </param>
    void SetType(Type type);

    
private:

  

private:
	Type m_type; // デバフの種類を保持する変数
};

