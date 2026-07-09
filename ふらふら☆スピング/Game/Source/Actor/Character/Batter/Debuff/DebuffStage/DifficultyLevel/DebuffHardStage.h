#pragma once
#include "Source/Actor/Character/Batter/Debuff/DebuffStage/DebuffStage.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/IDebuffPattern.h"
#include <vector>

class Batter;

class DebuffHardStage:public DebuffStage
{
	public:
		DebuffHardStage();
		~DebuffHardStage();
	void Update(Batter* batter);
    void BuildStage(int level)override;

    // 各デバフ段階
    void DebuffStageOne();
    void DebuffStageTwo();
    void DebuffStageThree();
    void DebuffStageFour();
    void DebuffStageFive();
    void DebuffStageSix();
    void DebuffStageSeven();
    void DebuffStageEight();
    void DebuffStageNine();
    void DebuffStageTen();
};

