#pragma once
#include "ShaderEffectBase.h"

/// MotionBlurPatternクラスの実装

class MotionBlurPattern : public ShaderEffectBase
{
public:

    enum MotionBlurType
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
    void SetType(MotionBlurType type);

    
private:

  

private:
    MotionBlurType m_type; // デバフの種類を保持する変数
};

