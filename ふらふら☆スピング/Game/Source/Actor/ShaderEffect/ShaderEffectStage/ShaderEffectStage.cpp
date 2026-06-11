#include "stdafx.h"
#include "ShaderEffectStage.h"
#include "Source/Actor/ShaderEffect/Pattern/Pattern.h"



ShaderEffectStage::ShaderEffectStage()
{

}

ShaderEffectStage::~ShaderEffectStage()
{
	ClearPatterns();
}

void ShaderEffectStage::Update()
{
	for (auto& pattern : m_patterns)
	{
		//pattern->Update();
	}
}

void ShaderEffectStage::BuildStage(int level)
{
	switch (level)
	{
	case 1:
		break;
		return;
		break;
	}
}