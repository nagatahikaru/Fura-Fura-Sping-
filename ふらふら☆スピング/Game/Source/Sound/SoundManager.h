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

	enSound_Num //このステータスは、サウンドの総数を表しているため、この下には追加しないでください。
};
class SoundManager :public IGameObject
{
public:
	SoundManager();
	~SoundManager() {};
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
	void SetBGMVolume(float vol);
	void SetSEVolume(float vol);
	float m_bgmVolume = 100.0f;   // 0〜100
	float m_seVolume = 100.0f;   // 0〜100
};

extern SoundManager* g_soundManager;
extern SoundSource* g_bgm;
