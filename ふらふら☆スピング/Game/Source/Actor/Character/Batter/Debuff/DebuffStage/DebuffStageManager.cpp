#include "stdafx.h"
#include "DebuffStageManager.h"
#include "Source/Actor/Character/Batter/Batter.h"
#include "DifficultyLevel/DebuffEasyStage.h"
#include "DifficultyLevel/DebuffNormalStage.h"
#include "DifficultyLevel/DebuffHardStage.h"


DebuffStageManager::DebuffStageManager()
{

}

DebuffStageManager::~DebuffStageManager()
{
	
}

void DebuffStageManager::Initialize(Batter* batter)
{
    
    switch (batter->GetGame()->GetDifficulty())
    {
    case Easy:
        m_stage = std::make_unique<DebuffEasyStage>();
        break;

    case Normal:
        m_stage = std::make_unique<DebuffNormalStage>();
        break;

    case Hard:
        m_stage = std::make_unique<DebuffHardStage>();
        break;
    }
}