#pragma once
#include "Source/Actor/Character/Batter/Debuff/Pattern/IDebuffPattern.h"
#include "Source/Actor/Character/Batter/Debuff/DebuffStage/DebuffStage.h"
#include <vector>

class Batter;

class DebuffEasyStage:public DebuffStage
{
private:

public:
     DebuffEasyStage();
	~DebuffEasyStage();
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

