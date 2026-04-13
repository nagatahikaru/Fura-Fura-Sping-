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
	void StopEffect(int handle);

	bool GetIsPlayeEffect()
	{
		return m_effectEmitter->IsPlay();
	}

private:
	std::u16string ToU16(const std::string& str);

	const char* m_filePath = "Game/Assets/effect/";
	const char* m_ext = ".efk";

	const char* m_files[enEffect_Num] = {
		"DownArrow"
	};

	EffectEmitter* m_effectEmitter = nullptr;					//effect‚Ö‚ÌQÆB
};

extern EffectManager* g_effectManager;