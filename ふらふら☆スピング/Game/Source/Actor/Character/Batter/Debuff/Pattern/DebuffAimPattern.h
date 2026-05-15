#pragma once
#include "DebuffPatternBase.h"

class DebuffAimPattern : public DebuffPatternBase
{
public:

    enum AimType
    {
        SmallCursor,
        TinySweetSpot,
        WeakHitAssist,
        HeavySwing,
    };

    void SetType(AimType type);

    void Update(Batter* batter) override;
    void SetMeatRange(float range)
    {
		m_meatRange = range;
    }

private:

    void UpdateSmallCursor(Batter* batter);
    void UpdateTinySweetSpot(Batter* batter);
	void UpdateWeakHitAssist(Batter* batter);
	void UpdateHeavySwing(Batter* batter);

private:	
    AimType m_type;
	float m_meatRange = 0.0f; // “–‚½‚è”»’è‚Ì”ÍˆÍ‚ð’Ç‰Á
};

