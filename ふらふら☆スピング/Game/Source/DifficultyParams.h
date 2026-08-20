// DifficultyParams.h
#pragma once
#include "Source/Scene/InGame/Game.h" // Difficulty enum を使うため

struct DifficultyParams
{
    // Ball
    float ballBaseSpeed;// 投球の基本速度
    float ballGravity;// 投球の重力
    int   magicBallChancePercent;// 魔球の出現確率（％）
    float controlAccuracy; // コントロール精度（ランダムターゲット生成用）
    int   pitchCount; // 投球数
    // Game
    float replayDuration;// リプレイの再生時間（秒）
    int   guruguruSEInterval;// ぐるぐるSEを鳴らす間隔（ぐるぐるの回数）
    float replayZoomMaxSpeed; // ★ 追加：リプレイズームカメラの1フレーム最大移動量
    // Batter
    float meatRangeMultiplier; // ミート範囲の倍率
    int   adjacentFrames;      // 前後判定フレーム
};

/*
* 難易度に応じたパラメータを返す関数
* 呼び出し方
* cppファイルの先頭に#include "Source/DifficultyParams.h"を追加してから
* 下記のように呼び出す
* const DifficultyParams& params = GetDifficultyParams(Difficulty::Easy);
* 　　　　　　　 (↓矢印使いたい変数)
* 使い方はparams.ballBaseSpeedなどでアクセス可能
*/
inline const DifficultyParams& GetDifficultyParams(Difficulty diff)
{
    static const DifficultyParams table[4] = {
        /* Easy    */ { 1300.0f, 4.0f,  0, 0.3f,3, 5.5f, 7, 13.0f,  3.0f, 8 },
        /* Normal  */ { 1500.0f, 9.5f,  0, 0.5f,3, 4.5f, 5, 16.0f, 2.0f, 5 },
        /* Hard    */ { 2000.0f, 25.0f, 10,1.0f,3, 3.5f, 3, 19.0f, 1.0f, 2 },
        /* Tutorial*/ { 1300.0f, 4.0f,  0, 0.1f,5, 5.0f, 7, 8.0f,  3.0f, 8 },
    };
    return table[diff];
}