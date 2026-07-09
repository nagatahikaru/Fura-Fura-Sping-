#pragma once
#include "Source/Source.h"
#include "Source/Actor/Character/Batter/Debuff/Pattern/IDebuffPattern.h"
#include "DebuffStage.h"
#include "DifficultyLevel/DebuffEasyStage.h"
#include "DifficultyLevel/DebuffNormalStage.h"
#include "DifficultyLevel/DebuffHardStage.h"
#include <vector>

class Batter;

class DebuffStageManager
{
public:
    DebuffStageManager();
	~DebuffStageManager();

    DebuffStageManager*GetDebuffStageManager()
    {
		return this;
    }

    void Initialize(Batter* batter);
    void ResetStage()
    {
        if(m_stage)
        {
            m_stage.reset();
        }
	}

    void Update(Batter* batter)
    {
        if (m_stage)
        {
            m_stage->Update(batter);
        }
	}

    void RebuildStage(int level,int rotationCount)
    {
        if (m_stage)
        {
            m_stage->ClearPatterns();
            m_stage->SetRotationCount(rotationCount);
            m_stage->BuildStage(level);
        }
	}

    void ClearPatterns()
    {
       if (m_stage)
        {
            m_stage->ClearPatterns();
		}
    }

private:
    std::unique_ptr<DebuffStage> m_stage;

    enum
    {
        Easy,
        Normal,
		Hard
    };
};

