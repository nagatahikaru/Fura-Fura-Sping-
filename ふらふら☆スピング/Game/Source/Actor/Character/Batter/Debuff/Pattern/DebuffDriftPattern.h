#pragma once
#include "DebuffPatternBase.h"

class DebuffDriftPattern : public DebuffPatternBase
{
public:

    enum DriftType
    {
        Vertical,
        Horizontal,
        Random,
        Heavy
    };

    void SetType(DriftType type);

    void Update(Batter* batter) override;

private:

    void VerticalShake(Batter* batter);
    void HorizontalShake(Batter* batter);
	void RandomShake(Batter* batter);

private:	
    DriftType m_type;
};

