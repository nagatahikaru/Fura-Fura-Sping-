#pragma once
#include "sound/SoundSource.h"
//ファイルパスを追加する際は、
//cppのnamespaceと下記のenumを追加してください。
enum Sound{
	enSound_TitleBGM,
	enSound_GameBGM1,
	enSound_GameBGM2,
	enSound_GameBGM3,
	enSound_GameBGM4,
	enSound_GameBGM5,
	enSound_GameBGM6,
	enSound_GameBGM7,
	enSound_GameBGM8,
	enSound_GameBGM9,
	enSound_ResultBGM,
	
	enSound_SE,
	enSound_SE2,
	enSound_SE3,
	enSound_SE4,
	enSound_SE5,
	enSound_SE6,
	enSound_SE7,
	enSound_SE8,
	enSound_SE9,
	enSound_SE10,
	enSound_SE11,
	enSound_SE12,
	enSound_SE13,
	enSound_SE14,
	enSound_Num //このステータスは、サウンドの総数を表しているため、この下には追加しないでください。
};
class SoundManager :public IGameObject
{
public:
	SoundManager();
	~SoundManager() {};
	void Update();
	/// <summary>
	/// 指定したサウンドを再生し、再生中のサウンドソースを返します。
	/// </summary>
	/// <param name="number">再生するサウンドを指定します。</param>
	/// <param name="isLoop">サウンドをループ再生するかどうかを指定します。デフォルトは true です。</param>
	/// <param name="volume">再生音量を指定します。デフォルトは 1.0f です。</param>
	/// <returns>再生中のサウンドソースへのポインタ。</returns>
	SoundSource* PlayingSound(Sound number, bool isLoop = true, float volume = 1.0f);
	Sound m_nowPlayingBGM = enSound_Num; // 今流れているBGM
	SoundSource* PlaySE(Sound number, float volume = 1.0f);
	// ★ 追加
	SoundSource* GetSE2() const { return m_se2; }
	void ClearSE2() { m_se2 = nullptr; }
	void SetBGMVolume(float vol);
	void SetSEVolume(float vol);
	void StopBGM();
	float m_bgmVolume = 100.0f;   // 0〜100
	float m_seVolume = 100.0f;   // 0〜100
	float m_se2Volume = 100.0f;   // SE2 専用音量
	SoundSource* m_se2 = nullptr;
	void StopSE2();
	float m_se2SavedVolume = 1.0f;
	float m_se2BaseVolume = 1.0f;
	void MuteSE2();
	void UnmuteSE2();
	void FadeOutSE2(float durationSec);
	bool m_isFadingSE2 = false;
	float m_se2FadeDuration = 0.0f;
	float m_se2FadeTimer = 0.0f;
	float m_se2StartVolume = 0.0f;
};

extern SoundManager* g_soundManager;
extern SoundSource* g_bgm;
