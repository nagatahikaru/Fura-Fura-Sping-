#pragma once
#include "Source/Actor/Character/Batter/Debuff/DebuffStage/DebuffStage.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/IDebuffPattern.h"
#include <vector>

class Batter;

// Normal難易度のデバフステージ
// 使用パターンは Easyの2種（Shake, Magnet）に加えて Noise（ノイズ）と Drift（流され）を追加した4種類。
// Lv1?12にかけて、Easyと同じ導入の流れ→新パターン追加→全パターン組み合わせ、と段階的に難しくなるように構成しています。
class DebuffNormalStage :public DebuffStage
{
public:
    DebuffNormalStage();
    ~DebuffNormalStage();
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

};