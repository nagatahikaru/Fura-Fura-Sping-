#pragma once
#include "DebuffPatternBase.h"

class DebuffReversePattern : public DebuffPatternBase
{
public:

    enum ReverseType
    {
        Vertical,
        Horizontal,
        Random,
        Heavy
    };

    void SetType(ReverseType type);

    void Update(Batter* batter) override;

private:

    void VerticalShake(Batter* batter);
    void HorizontalShake(Batter* batter);
	void RandomShake(Batter* batter);

private:	
    ReverseType m_type;
};

