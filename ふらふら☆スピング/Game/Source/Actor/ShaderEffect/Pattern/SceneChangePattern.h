#pragma once
#include "ShaderEffectBase.h"

/// SceneChangePatternクラスの実装

class SceneChangePattern : public ShaderEffectBase
{
public:

    enum SceneChangeType
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
    void SetType(SceneChangeType type);

    
private:

  

private:
    SceneChangeType m_type; // デバフの種類を保持する変数
};

