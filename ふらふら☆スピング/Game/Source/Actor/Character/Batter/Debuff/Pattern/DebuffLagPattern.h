#pragma once
#include "DebuffPatternBase.h"

// DebuffLagPatternクラスの実装
// 入力遅延系のデバフパターンを実装するクラスです。
// 例えば、入力に対してカーソルが遅れて追従するデバフや、入力に対してカーソルがランダムに動くデバフなどがあります。
class DebuffLagPattern : public DebuffPatternBase
{
public:

    enum LagType
    {
        Vertical,
        Horizontal,
        Random
    };

    void SetType(LagType type);

    void Update(Batter* batter) override;

private:

private:	
    LagType m_type;
    Vector3 m_lagCursorPos;
};

