#pragma once
#include "ShaderEffectBase.h"

/// FocusPatternクラスの実装

class FocusPattern : public ShaderEffectBase
{
public:

    enum FocusType
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
    void SetType(FocusType type);

    
private:

  

private:
    FocusType m_type; // デバフの種類を保持する変数
};

