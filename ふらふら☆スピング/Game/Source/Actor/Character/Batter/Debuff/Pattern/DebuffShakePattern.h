#pragma once
#include "DebuffPatternBase.h"

class DebuffShakePattern : public DebuffPatternBase
{
public:

    enum ShakeType
    {
        Vertical,
        Horizontal,        
        Heavy
    };

    void SetType(ShakeType type);

    void Update(Batter* batter) override;

private:

    void VerticalShake(Batter* batter);
    void HorizontalShake(Batter* batter);
	void RandomShake(Batter* batter);

private:	
    ShakeType m_type;
};

