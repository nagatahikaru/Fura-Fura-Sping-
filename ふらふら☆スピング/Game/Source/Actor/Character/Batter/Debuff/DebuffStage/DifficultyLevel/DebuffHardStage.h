#pragma once
#include "Source/Actor/Character/Batter/Debuff/DebuffStage/DebuffStage.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/IDebuffPattern.h"
#include <vector>

class Batter;

// Hard難易度のデバフステージ
// Easy/Normalの4種（Shake, Magnet, Noise, Drift）に加え、
// Aim（照準）, Lag（遅延）, Reverse（反転）まで含めた全7種類を使用。
// Lv1?15にかけて、新パターンを少しずつ解禁しながら、最終的に全パターンを組み合わせた
// 最も過酷なステージへと段階的に難しくなるように構成しています。
class DebuffHardStage :public DebuffStage
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
    void DebuffStageEleven();
    void DebuffStageTwelve();
    void DebuffStageThirteen();
    void DebuffStageFourteen();
    void DebuffStageFifteen();
};