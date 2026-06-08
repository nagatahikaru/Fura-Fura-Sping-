#pragma once
#include "DebuffPatternBase.h"

// DebuffLagPatternクラスの実装
// 入力遅延系のデバフパターンを実装するクラスです。
// 例えば、入力に対してカーソルが遅れて追従するデバフや、入力に対してカーソルがランダムに動くデバフなどがあります。
class DebuffLagPattern : public DebuffPatternBase
{
public:

    enum LagType
    {
		Delay,      // 入力に対してカーソルが遅れて追従するデバフ
    };

    /// <summary>
    /// デバフの詳細な効果を設定する関数です。
    /// </summary>
    /// <param name="type"> デバフの種類を指定します。
    /// <para>Delay は入力に対してカーソルが遅れて追従するデバフ。</para>
    /// </param>
    void SetType(LagType type);

    void Update(Batter* batter) override;

    void Reset()
    {
        if (m_inputHistory.size() > 0)
        {
			m_inputHistory.clear();
        }        
	}

private:
	void LagCursor(Batter* batter);


private:	
    LagType m_type;
    // カーソル入力履歴
    std::deque<Vector2> m_inputHistory;
};

