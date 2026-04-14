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

	//// 再生だけ
	//void PlayEffect(
	//	EffectType type,
	//	const Vector3& pos,
	//	const Quaternion& rot,
	//	const Vector3& scale);

	// 停止
	void StopEffect(int handle);

	bool GetIsPlayeEffect()
	{
		return m_effectEmitter->IsPlay(); 
	}



private:
	std::string m_filePath = "Assets/effect/";
	std::string m_ext = ".efkpkg";
	std::u16string m_paths[enEffect_Num];
	//char16_t型を使っているので""の前にuをつける事
	std::string m_files[enEffect_Num] = {
		"DownArrow"
	};

	EffectEmitter* m_effectEmitter = nullptr;					//effectへの参照。
};

extern EffectManager* g_effectManager;