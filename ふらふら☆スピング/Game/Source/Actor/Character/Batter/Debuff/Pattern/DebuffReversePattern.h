#pragma once
#include "DebuffPatternBase.h"
#include "Source/Actor/Character/Batter/Batter.h"


/// DebuffReversePatternクラスの実装
/// 入力反転系のデバフパターンを実装するクラスです。
/// 例えば、Y軸に対して入力が反転するデバフや、X軸に対して入力が反転するデバフ、
/// または両方の軸に対して入力が反転するデバフなどがあります。
class DebuffReversePattern : public DebuffPatternBase
{
public:

    enum ReverseType
    {
        Reverse_Y,
        Reverse_X,
		Reverse_All
    };

    /// <summary>
    /// 入力に対する反転軸を設定する関数です。
    /// </summary>
    /// <param name="type"> 反転させる軸を指定します。
	/// <para>Reverse_Y はY軸に対して入力が反転するデバフ。</para>
	/// <para>Reverse_X はX軸に対して入力が反転するデバフ。</para>
	/// <para>Reverse_All はX軸とY軸の両方に対して入力が反転するデバフです。</para>
    /// </param>
    void SetType(ReverseType type);

    void Update(Batter* batter) override;

    void Reset(Batter* batter)
    {
        batter->SetDelayFrag(false);
        batter->SetInversionInputScale(1.0f, 1.0f);
	}

private:

    void SetReverseY(Batter* batter)
    {
        batter->SetDelayFrag(true);
        batter->SetInversionInputScale(m_reversePower,0.0f);
    }
    void SetReverseX(Batter* batter)
    {
        batter->SetDelayFrag(true);
        batter->SetInversionInputScale(0.0f, m_reversePower);
    }
    void SetReverseAll(Batter* batter)
    {
		batter->SetDelayFrag(true);
        batter->SetInversionInputScale(m_reversePower, m_reversePower);
    }
    
private:	
    ReverseType m_type;
	float m_reversePower = -1.0f;
};

