#include "stdafx.h"
#include "SoundManager.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"
#include <iostream>
SoundManager* g_soundManager = nullptr;
SoundSource* g_bgm = nullptr;
namespace {
	//ファイル名のみ追加すれば、
	//コンストラクタにて自動でg_soundEngineに登録されます。
	//例："Assets/sound/"+"Title"+".wav"
	const char* SOUND_FILEPATH = "Assets/sound/";
	const char* SOUND_EXTENSION = ".wav";
	const char* soundFileNameList[enSound_Num] =
	{
		//ここにファイル名を追加してください。
		//追加する際は、ヘッダのenum Soundにも追加してください。
		"TitleBGM",
		"GameBGM1",
		"GameBGM2",
	    "GameBGM3",
     	"GameBGM4",
	    "GameBGM5",
	    "GameBGM6",
	    "GameBGM7",
	    "GameBGM8",
	    "GameBGM9",
		"ResultBGM",
		"SE",
		"SE2",
		"SE3"
	};	
}

SoundManager::SoundManager()
{
	//サウンドのファイルパスの登録。
	//サウンドの個数分（enSound_Numの数）、登録されます。
	for(int i = 0; i < enSound_Num; i++)
	{
		//std::stringを使用し、文字列を結合。
		std::string file = std::string(SOUND_FILEPATH) + soundFileNameList[i] + (SOUND_EXTENSION);
		//引数がconst char*なので、std::stringからconst char*に変換。
		const char* filepath = file.c_str();
		//g_soundEngineへ登録。
		g_soundEngine->ResistWaveFileBank(i, filepath);
	}
}

SoundSource* SoundManager::PlayingSound(Sound number,bool isLoop,float volume)
{
	// すでに同じBGMが流れているなら再生しない（継続）
	if (m_nowPlayingBGM == number && g_bgm != nullptr) {
		return g_bgm;
	}

	// 別のBGMが流れているなら止める
	if (g_bgm) {
		DeleteGO(g_bgm);
		g_bgm = nullptr;
	}


	SoundSource* sound = NewGO<SoundSource>(0);
	//引数で受け取ったnumberはsoundFileNameListの要素番号に対応しています。
	sound->Init(number);     //サウンドの初期化。
	sound->SetVolume(volume);//音量の設定。
	sound->Play(isLoop);     //サウンドの再生。
	//戻り値にSoundSourceのインスタンスを設定。
	//呼び出し元で、
	//SoundSource* sound = sound->PlayingSound(Sound::enSound_TitleBGM);
	//と書くとインスタンスのアドレスを受け取ることができます。

	   g_bgm = sound;
    m_nowPlayingBGM = number;

	return sound;
}

SoundSource* SoundManager::PlaySE(Sound number, float volume)
{
	float v = m_seVolume / 100.0f;
	float curved = powf(v, 1.3f);

	// ★ SE2 は 1 個だけにする
	if (number == enSound_SE2) {
		if (!m_se2) {
			m_se2 = NewGO<SoundSource>(0);
			m_se2->Init(number);
		}

		float v = m_seVolume / 100.0f;
		float curved = powf(v, 1.3f);

		m_se2BaseVolume = curved;        // ★ 本来の音量を保存
		m_se2->SetVolume(curved);        // ★ 実際の音量をセット
		m_se2->Play(false);

		return m_se2;
	}


	// ★ 通常 SE は今まで通り
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(number);
	se->SetVolume(curved);
	se->Play(false);
	return se;
}

void SoundManager::StopSE2()
{
	if (m_se2) {
		m_se2->Stop();
		DeleteGO(m_se2);
		m_se2 = nullptr;
	}
}

void SoundManager::SetBGMVolume(float vol) {
	m_bgmVolume = vol;

	if (g_bgm) {
		float v = vol / 100.0f;

		// ★ 音量カーブを強調（2乗）
		float curved = powf(v, 1.5f);

		g_bgm->SetVolume(curved);
	}
}

void SoundManager::SetSEVolume(float vol) {
	m_seVolume = vol;
}

void SoundManager::MuteSE2()
{
	if (m_se2) {
		m_se2->SetVolume(0.0f); // ミュート
	}
}

void SoundManager::UnmuteSE2()
{
	if (m_se2) {
		m_se2->SetVolume(m_se2BaseVolume); // ★ 本来の音量に戻す
	}
}

void SoundManager::FadeOutSE2(float delta)
{
	if (!m_se2) return;

	float v = m_se2->GetVolume();
	v -= delta;

	if (v <= 0.0f) {
		v = 0.0f;
		m_se2->Stop();
	}

	m_se2->SetVolume(v);
}

