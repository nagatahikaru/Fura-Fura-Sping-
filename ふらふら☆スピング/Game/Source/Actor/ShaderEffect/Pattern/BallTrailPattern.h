#pragma once
#include "ShaderEffectBase.h"

/// BallTrailPatternクラスの実装

class BallTrailPattern : public ShaderEffectBase
{
public:

    enum BallTrailType
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
    void SetType(BallTrailType type);

    
private:

  

private:
    BallTrailType m_type; // デバフの種類を保持する変数
};

