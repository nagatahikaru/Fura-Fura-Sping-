#pragma once
#include "DebuffPatternBase.h"

class DebuffNoisePattern : public DebuffPatternBase
{
public:

    enum NoiseType
    {
        Vertical,
        Horizontal,        
        Heavy
    };

    void SetType(NoiseType type);

    void Update(Batter* batter) override;

private:

	void Noise(Batter* batter);

private:
    NoiseType m_type;
};

