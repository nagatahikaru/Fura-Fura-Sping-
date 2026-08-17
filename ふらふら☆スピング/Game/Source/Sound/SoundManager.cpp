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
		"SE3",
		"SE4",
		"SE5",
		"SE6",
		"SE7",
		"SE8",
		"SE9",
		"SE10",
		"SE11",
		"SE12",
		"SE13",
		"SE14",
		"SE15",
		"SE16",
		"SE17"
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

void SoundManager::Update()
{
	if (m_isFadingSE2 && m_se2) {

		m_se2FadeTimer += g_gameTime->GetFrameDeltaTime();

		float t = m_se2FadeTimer / m_se2FadeDuration;
		if (t > 1.0f) t = 1.0f;

		float newVolume = m_se2StartVolume * (1.0f - t);
		m_se2->SetVolume(newVolume);

		if (t >= 1.0f) {
			m_se2->Stop();
			m_isFadingSE2 = false;
		}
	}
}

SoundSource* SoundManager::PlayingSound(Sound number, bool isLoop, float volume)
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

	float masterScale = m_masterVolume / 100.0f;
	float finalVolume = volume * masterScale;   // ★ マスター音量を掛ける

	SoundSource* sound = NewGO<SoundSource>(0);
	sound->Init(number);
	sound->SetVolume(finalVolume);              // ★ 修正
	sound->Play(isLoop);

	g_bgm = sound;
	m_nowPlayingBGM = number;

	return sound;
}

// SoundManager.cpp

void SoundManager::SetMasterVolume(float vol) {
	m_masterVolume = vol;

	// ▼ BGM の再計算
	SetBGMVolume(m_bgmVolume);

	float v = m_seVolume / 100.0f;
	float curved = powf(v, 1.3f);
	float masterScale = m_masterVolume / 100.0f;

	// ▼ SE2 の再計算
	if (m_se2) {
		if (m_masterVolume <= 0.0f) {
			m_se2->SetVolume(0.0f);   // ★ 完全ミュート
		}
		else {
			m_se2BaseVolume = curved * 3.0f * masterScale;
			m_se2->SetVolume(m_se2BaseVolume);
		}
	}

	// ▼ ★ 追加：再生中の BGM を完全ミュート
	if (g_bgm) {
		if (m_masterVolume <= 0.0f) {
			g_bgm->SetVolume(0.0f);   // ★ 完全ミュート
		}
		else {
			float v2 = m_bgmVolume / 100.0f;
			float curved2 = powf(v2, 1.5f);
			g_bgm->SetVolume(curved2 * masterScale);
		}
	}
}

void SoundManager::SetBGMVolume(float vol) {
	m_bgmVolume = vol;

	if (g_bgm) {
		float v = vol / 100.0f;
		float curved = powf(v, 1.5f);

		// ★ マスターボリュームの倍率（0.0〜1.0）を掛け合わせる
		float masterScale = m_masterVolume / 100.0f;
		g_bgm->SetVolume(curved * masterScale);
	}
}

// ▼▼ 修正：SE再生（マスター音量を掛け合わせる） ▼▼
SoundSource* SoundManager::PlaySE(Sound number, float volume)
{
	float v = m_seVolume / 100.0f;
	float curved = powf(v, 1.3f);

	// ★ マスターボリュームの倍率（0.0〜1.0）
	float masterScale = m_masterVolume / 100.0f;

	// ★ SE2 は 1 個だけにする
	if (number == enSound_SE2) {
		if (!m_se2) {
			m_se2 = NewGO<SoundSource>(0);
			m_se2->Init(number);
		}

		m_se2BaseVolume = curved * 3.0f * masterScale;  // ★ マスターを掛ける
		m_se2->SetVolume(m_se2BaseVolume);
		m_se2->Play(false);

		return m_se2;
	}

	if (number == enSound_SE4) {
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(number);
		se->SetVolume(curved * 1.5f * masterScale);    // ★ マスターを掛ける
		se->Play(false);
		return se;
	}

	if (number == enSound_SE14) {
		SoundSource* se = NewGO<SoundSource>(0);
		se->Init(number);

		float raw = (m_seVolume / 100.0f) * 3.0f * masterScale; // ★ マスターを掛ける
		se->SetVolume(raw);

		se->Play(false);
		return se;
	}

	// ★ 通常 SE
	SoundSource* se = NewGO<SoundSource>(0);
	se->Init(number);
	se->SetVolume(curved * masterScale);                // ★ マスターを掛ける
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

void SoundManager::StopBGM()
{
	if (g_bgm) {
		g_bgm->Stop();
		DeleteGO(g_bgm);
		g_bgm = nullptr;
		m_nowPlayingBGM = enSound_Num; // ★ 何も再生していない状態に
	}
}

void SoundManager::FadeOutSE2(float durationSec)
{
	if (!m_se2) return;

	m_isFadingSE2 = true;
	m_se2FadeDuration = durationSec;
	m_se2FadeTimer = 0.0f;

	m_se2StartVolume = m_se2->GetVolume();
}

// SoundManager.cpp に追加
float SoundManager::GetCalculatedBGMVolume(float volumePercent) {
	float v = m_bgmVolume / 100.0f;
	float curved = powf(v, 1.5f);
	float masterScale = m_masterVolume / 100.0f;
	return curved * masterScale;
}

