#pragma once
#include "ShaderEffectBase.h"

/// HomeRunPatternクラスの実装

class HomeRunPattern : public ShaderEffectBase
{
public:

    enum HomeRunType
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
    void SetType(HomeRunType type);

    
private:

  

private:
    HomeRunType m_type; // デバフの種類を保持する変数
};

