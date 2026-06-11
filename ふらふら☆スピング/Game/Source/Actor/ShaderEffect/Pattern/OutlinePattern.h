#pragma once
#include "ShaderEffectBase.h"

/// OutlinePatternクラスの実装

class OutlinePattern : public ShaderEffectBase
{
public:

    enum OutlineType
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
    void SetType(OutlineType type);

    
private:

  

private:
    OutlineType m_type; // デバフの種類を保持する変数
};

