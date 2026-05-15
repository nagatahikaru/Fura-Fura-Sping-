#pragma once
#include "DebuffPatternBase.h"

class DebuffMagnetPattern : public DebuffPatternBase
{
public:

    enum MagnetType
    {
        Vertical,
        Horizontal, 
        Heavy
    };

    void SetType(MagnetType type);
    void SetRandomSpotRadius(float radius)
    {
        m_randomSpotRadius = radius;
	}
    void SetRandomMoveDuration(float duration)
    {
        m_randomMoveDuration = duration;
	}

    void Update(Batter* batter) override;

private:

    void ApplyRandomMagnet(Batter* batter);
    void ApplyHorizontalMagnet(Batter* batter);
	void ApplyVerticalMagnet(Batter* batter);

private:	
    MagnetType m_type;

	// ランダムなカーソル移動のための変数
	bool m_randomCursorUpdate = true;        // ランダムなカーソル移動を更新するかどうかのフラグ
	float m_randomCursorMoveTimer = 0.0f;    // ランダムなカーソル移動のタイマー
	float m_randomMoveDuration = 0.5f;       // ランダムなカーソル移動の持続時間
	float m_randomSpotRadius = 50.0f;        // ランダムなカーソル移動の半径    
	Vector3 m_randomCursorMovePwer;          // ランダムなカーソル移動の強さ
	Vector3 m_randomCursorTargetPos;         // ランダムなカーソルの目標位置
};

