#pragma once
#include "DebuffPatternBase.h"

class DebuffLagPattern : public DebuffPatternBase
{
public:

    enum LagType
    {
        Vertical,
        Horizontal,
        Random,
        Heavy
    };

    void SetType(LagType type);

    void Update(Batter* batter) override;

private:

    void VerticalShake(Batter* batter);
    void HorizontalShake(Batter* batter);
	void RandomShake(Batter* batter);

private:	
    LagType m_type;
};

