#pragma once
#include "sound/SoundSource.h"

//ファイルパスを追加する際は、
//cppのnamespaceと下記のenumを追加してください。
enum Sound{
	enSound_1,
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
};


