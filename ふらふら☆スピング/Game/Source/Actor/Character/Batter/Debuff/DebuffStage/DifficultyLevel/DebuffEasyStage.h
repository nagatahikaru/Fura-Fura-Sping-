#pragma once
#include "Source/Actor/Character/Batter/Debuff/Pattern/IDebuffPattern.h"
#include "Source/Actor/Character/Batter/Debuff/DebuffStage/DebuffStage.h"
#include <vector>

class Batter;

// Easy難易度のデバフステージ
// 使用パターンは DebuffShakePattern（揺れ）と DebuffMagnetPattern（誘導）の2種類のみ。
// Lv1?10にかけて、単体→組み合わせ→強化、と段階的に難しくなるように構成しています。
class DebuffEasyStage :public DebuffStage
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