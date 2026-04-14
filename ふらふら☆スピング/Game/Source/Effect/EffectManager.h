#pragma once
#include "graphics/effect/EffectEmitter.h"
#include <map>
#include <string>

enum EffectType {
	enEffect_DownArrow,
	enEffect_Num
};

class EffectManager : public IGameObject
{
public:
	EffectManager();
	~EffectManager() = default;

	void SetEffect(
		EffectType type,
		const Vector3& pos,
		const Quaternion& rot,
		const Vector3& scale);

	//// Ä¶‚¾‚¯
	//void PlayEffect(
	//	EffectType type,
	//	const Vector3& pos,
	//	const Quaternion& rot,
	//	const Vector3& scale);

	// ’â~
	void StopEffect();

	bool GetIsPlayeEffect()
	{
		if (!GetEffectEmitter())return false;
		return m_effectEmitter->IsPlay(); 
	}

	EffectEmitter* GetEffectEmitter()
	{
		return m_effectEmitter;
	}



private:
	const char16_t* m_filePath = u"Assets/effect/";
	const char16_t* m_ext = u".efk";

	const char16_t* m_files[enEffect_Num] = {
		u"DownArrow"
	};

	EffectEmitter* m_effectEmitter = nullptr;					//effect‚Ö‚ÌQÆB
};

extern EffectManager* g_effectManager;