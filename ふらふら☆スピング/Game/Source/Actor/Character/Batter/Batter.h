#pragma once
#include "Source/Actor/Character/Character.h"
#include "Source/UI/InGameUI/InGameUI.h"
#include "BatterStateMachine.h"
#include "Source/Actor/Character/Ball/Ball.h"
#include "Source/Scene/InGame/Game.h"
#include "Source/Effect/EffectManager.h"
#include "Debuff/DebuffStageStateMachine.h"
#include "Debuff/DebuffStage/DebuffStage.h"


namespace BATTER{
   extern std::string FILE_PATH_BATTER;
   extern std::string FILE_PATH_DDS;
   extern std::string FILE_PATH_ANIMATION[4];

    namespace BAT
    {
        const Vector3 OFFSET_BAT = Vector3(100.0f, 150.0f, 0.0f); //バットのオフセット
        const Vector3 COLLISION_SCALE_BAT = Vector3(114.285714286f, 80.0f, 40.0f);
        const Vector3 ROTATION_ANGLE = Vector3(0.0f, 0.0f, 1.0f); //バットの回転角度と回転速度
		constexpr float SPIN_TIME_LIMIT = 5.0f;             //回転の時間制限
		constexpr float HALF_ROTATION_ANGLE = 180.0f;   //回転の半分の角度
		constexpr float FULL_ROTATION_ANGLE = 360.0f;   //回転の全体の角度		
        constexpr float HEIGHT_OFFSET = 200.0f; //バットの高さオフセット
    }

    const Vector3 INITIAL_COORDINATE = Vector3(-420.0f, -95.0f, 5500.0f);//初期座標
    const Vector3 INITIAL_SCALE = Vector3(10.0f, 10.0f, 10.0f); //初期スケール
    const Vector3 COLLISION_SCALE = Vector3(50.0f, 35.0f, 50.0f); //当たり判定スケール

    constexpr float CURSOR_MIN_X = -300.0f;
    constexpr float CURSOR_MAX_X = 300.0f;
    constexpr float CURSOR_MIN_Y = -300.0f;
    constexpr float CURSOR_MAX_Y = 300.0f;
    constexpr float SCREEN_WIDTH = 1920.0f;				// 画面幅（ピクセル）
    constexpr float	SCREEN_HEIGHT = 1080.0f;			// 画面高さ（ピクセル）
    constexpr float HIT_ZONE_UPPER_LIMIT = 6500.0f;		// ヒットゾーンの上限（バッターの位置からの相対距離）
    constexpr float HIT_ZONE_LOWER_LIMIT = 6200.0f;		// ヒットゾーンの下限（バッターの位置からの相対距離）
    constexpr float HIT_ZONE_CENTER = 6400.0f;			// ヒットゾーンの中心（バッターの位置からの相対距離）
    constexpr float HIT_ZONE_RADIUS = 50.0f;			// 真ん中から端までの最大距離 (6080 - 6075)
    constexpr float RAD_TO_DEG = 180.0f / 3.14159265f;	//ラジアンを度に変換するための定数
    constexpr float CURSOR_MOVE_SPEED = 500.0f;			// カーソルの移動速度（ピクセル/秒）
	constexpr float ROTATION_RADIUS = 30.0f;			// 回転の半径
	constexpr float LENGTH_EPSILON = 0.001f;            // 長さの比較に使用する小さな値
	constexpr float READY_TIME = 5.0f;					// 準備時間
}


class Batter : public Character
{
private:
    // アニメーションID
    enum EnAnimationClip
    {
        enAnimationClip_Idle,       // 待機
        enAnimationClip_Rotation,   // 回転
        enAnimationClip_Swing,      // スイング
		enAnimationClip_SwingGo,    // スイングゴー
        enAnimationClip_Num
    };
    std::unique_ptr<BatterStateMachine> m_stateMachine;                 // バッターステート管理   
    std::unique_ptr<DebuffStageStateMachine> m_debuffStageStateMachine; // デバフステート管理    
    std::unique_ptr<nsApp::CharacterModel> m_characterModel;            // キャラクターモデル
    Vector3 m_posIdle = Vector3(-420.0f, -90.0f, 5500.0f);
    Vector3 m_posSwingGo = Vector3(-420.0f, -40.0f, 5500.0f);
    Vector3 m_posSwing = Vector3(-420.0f, -40.0f, 5500.0f); 
    void UpdateModelPosition();
    Game* m_game = nullptr;                                             // ゲーム本体    
    InGameUI* m_inGameUI = nullptr;                                     // UI   
    Ball* m_ball = nullptr;                                             // ボール
    DebuffStage* m_debuffStage;                                         // デバフステージ
    CollisionObject* m_collisionObject = nullptr;                       // 当たり判定    

    AnimationClip m_animationClips[enAnimationClip_Num];                // アニメーション配列    
    EnAnimationClip m_setAnimation = enAnimationClip_Idle;              // 現在アニメーション    
    Transform m_transform;                                              // トランスフォーム    
    Quaternion m_initialRotation;                                       // 初期回転

    Vector3 m_facingDir = Vector3(1.0f, 0.0f, 0.0f);                   // プレイヤーの向き
    Vector3 m_meetPosition;                                             // ミート位置    
    Vector3 m_meetCursorWorldPos;                                       // カーソルワールド座標    
    Vector3 m_cursorOffset;                                             // カーソルオフセット
    Vector3 m_shakeCursorOffset = Vector3::Zero;                        // 揺れによるカーソルオフセット    
    Vector3 m_assistCursorOffset = Vector3::Zero;                       // 揺れによるカーソルオフセットの減衰速度    
    Vector3 m_magnetCursorOffset = Vector3::Zero;                       // 磁石によるカーソルオフセットの減衰速度    
    Vector3 m_noiseCursorOffset = Vector3::Zero;                        // ノイズによるカーソルオフセットの減衰速度    
    Vector3 m_driftCursorOffset = Vector3::Zero;                        // ドリフトによるカーソルオフセットの減衰速度
    Vector2 m_inputdelayScale = Vector2(1.0f, 1.0f);                    // カーソル移動の遅延スケール
    Vector3 m_bodyCenter;                                               // 本体中心
    Vector3 m_modelPos;                                                 // モデルの半径
	Vector3 m_hitPosition;                                              // ヒット位置

    Vector2 m_inputScale = Vector2(1.0f, 1.0f);                         // カーソル入力スケール   
    Vector2 m_inversioninputScale = Vector2(1.0f, 1.0f);                // 反転カーソル入力スケール

    bool m_isCursorMode = true;                                         // カーソルモード
    bool m_isDelayFrag = false;                                         // カーソル移動の遅延フラグ    
    bool m_isRotation = true;                                           // 回転有効
    bool m_isReplay = false;                                            // リプレイ中フラグ

    float m_meatRange = 50.0f;                                         // ヒット範囲    
    float m_cursorMoveScale = 1.0f;                                     // カーソル移動速度倍率 
    float m_prevAngle = 0.0f;                                           // 前回角度    
    float m_guruGuruBatTimer = 0.0f;                                    // 回転タイマー    
    float m_totalRotation = 0.0f;                                       // 総回転量

    int m_guruGuruBatCount = 0;                                         // 回転回数

    struct EffectInfo
    {
        // 出現エフェクト
        uint32_t m_effectDawnID;

        // ヒットエフェクト
        uint32_t m_effectHitID;
    };
    EffectInfo m_inro;                                                  // エフェクト情報


private:



public:
    // 一時停止フラグ
    bool m_isPaused = false;

    Batter();
    virtual ~Batter();

    // 初期化
    virtual bool Start();

    // 更新処理
    virtual void Update();

    // 描画処理
    virtual void Render(RenderContext& rc);

    // アニメーション再生
    void SetPlayAnimation(int enAnimationClip)
    {
        m_characterModel->PlayAnimation(enAnimationClip, 0.2f);
    }

    // 現在のアニメーション取得
    EnAnimationClip GetEnAnimationClip() const
    {
        return m_setAnimation;
    }

    // スイング開始
    void Swing()
    {
        m_setAnimation = enAnimationClip_Swing;
    }

    // スイングアニメーション再生
    void PlaySwingAnimation()
    {
        // ★ スイングアニメーションを再生
        m_characterModel->PlayAnimation(enAnimationClip_Swing, 1.0f);
    }

    // スイング状態リセット
    void ResetSwing();

    // アニメーション更新
    void AnimationUpdate()
    {
        m_characterModel->Update();
    }

    // 回転アニメーション設定
    void SetPlayRotation()
    {
        m_setAnimation = enAnimationClip_Rotation;
    }

    // 待機アニメーション設定
    void SetIdleAnimation()
    {
        m_setAnimation = enAnimationClip_Idle;
    }

    void SetSwingGoAnimation()
    {
        m_setAnimation = enAnimationClip_SwingGo;
	}

    bool GetSwingGoAnimation() const
    {
        return m_setAnimation == enAnimationClip_SwingGo;
	}

    // アニメーション再生中判定
    bool IsPlayAnimation()
    {
        return m_characterModel->IsPlayAnimation();
    }

    // スイング再生中判定
    bool IsSwingAnimationPlaying()
    {
        return m_characterModel->IsPlayAnimation()
            && m_setAnimation == enAnimationClip_Swing;
    }

    bool IsSwingGoAnimationPlaying()
    {
        return m_characterModel->IsPlayAnimation()
            && m_setAnimation == enAnimationClip_SwingGo;
	}

    // カーソル座標更新
    void SetCursorPosition();

    // 3Dカーソル更新
    void UpdateCursor3D()
    {
        SetCursorPosition();
        m_meetCursorWorldPos = CalcCursorWorldPos();
    }

    // カーソルモード切り替え
    void SetCursorMode(bool flag)
    {
        m_isCursorMode = flag;
    }

    // ワールド座標変換
    Vector3 CalcCursorWorldPos();

    // 揺れによるカーソル揺れのオフセット追加
    void SetShakeCursorOffset(const Vector3& offset)
    {
        m_shakeCursorOffset = offset;
    }

    // ノイズによるカーソル揺れのオフセット追加
    void SetNoiseCursorOffset(const Vector3& offset)
    {
        m_noiseCursorOffset = offset;
    }

    // ドリフトによるカーソル揺れのオフセット追加
    void SetDriftCursorOffset(const Vector3& offset)
    {
        m_driftCursorOffset = offset;
    }

    //引き寄せによるカーソル揺れのオフセット追加
    void SetMagnetCursorOffset(const Vector3& offset)
    {
        m_magnetCursorOffset += offset;
    }

    Vector3 GetMagnetCursorOffset() const
    {
        return m_magnetCursorOffset;
    }

    // カーソル移動速度倍率
    void AddCursorMoveScale(float scale)
    {
        m_cursorMoveScale *= scale;
    }

    //　反転カーソル入力スケール
    void SetInversionInputScale(float x, float y)
    {
        m_inversioninputScale = Vector2(x, y);
    }

    void SetMeatRange(float range)
    {
        m_meatRange = range;
        DebugLogFloat("range", range);
        DebugLogFloat("m_meatRange", m_meatRange);
    }

    // カーソル移動の遅延
    void SetInputMoveScale(Vector2 scale)
    {
        m_inputdelayScale = scale;
    }

    void SetDelayFrag(bool flag)
    {
        m_isDelayFrag = flag;
    }

    Vector3 GetMeetCursorPosition() const
    {
        return m_meetPosition;
    }

    Vector3 GetFinalCursorOffset() const
    {
        return
            m_shakeCursorOffset +
            m_assistCursorOffset +
            m_noiseCursorOffset +
            m_magnetCursorOffset;
    }

    // カーソルオフセットリセット
    void ResetCursorOffset()
    {
        m_shakeCursorOffset = Vector3::Zero;
        m_assistCursorOffset = Vector3::Zero;
        m_noiseCursorOffset = Vector3::Zero;
        m_driftCursorOffset = Vector3::Zero;
        m_magnetCursorOffset = Vector3::Zero;
        m_inversioninputScale = Vector2(1.0f, 1.0f);
        m_cursorMoveScale = 1.0f;
        m_isDelayFrag = false;
    }

    Vector3 GetFinalCursorPosition() const
    {
        return
            m_meetPosition
            + GetFinalCursorOffset();
    }


    Vector2 GetInputScale() const
    {
        return m_inputScale;
    }


    // バット回転開始
    void Rotation();

    // 回転更新
    void RotationUpdate();

    // バット位置更新
    void SetBatSwingPosition()
    {
        Vector3 meetCursorPosition =
            m_inGameUI->GetMeetCursorPosition();

        m_characterModel->SetWeaponOffset(
            meetCursorPosition - m_transform.m_position);
    }

    // バットの向き更新
    void UpdateBatAim();

    // 当たり判定更新
    void BatHitBoxPosition();

    // グルグルバット更新
    void RoundAndRoundBat();

    void SetGuruGuruCount(int count)
    {
        m_guruGuruBatCount = 3 * count;
    }

    // 回転回数カウント
    void GuruGuruCountUP(float currentAngle);

    // 回転回数取得
    int GetGuruGuru() const
    {
        return m_guruGuruBatCount;
    }

    // バット座標取得
    Vector3 GetBatPostion() const
    {
        return m_characterModel->GetWeaponWorldPosition();
    }

    // 打撃処理
    void HitBat();

    // ヒットエフェクト
    void HitEffect(Vector3 pos);

    // 点と線分距離計算
    float DistancePointToSegment(
        const Vector3& ballpos,
        const Vector3& base,
        const Vector3& tip)
    {
        Vector3 ab = tip - base;
        Vector3 ac = ballpos - base;

        float lenSq = ab.Dot(ab);

        if (lenSq < 0.0001f)
        {
            return (ballpos - base).Length();
        }

        float t = ac.Dot(ab) / lenSq;

        t = max(0.0f, min(1.0f, t));

        Vector3 closestPoint = base + ab * t;

        return (ballpos - closestPoint).Length();
    }

    // スクリーン座標 → レイ変換
    Vector3 ScreenToRay(
        float mouseX,
        float mouseY,
        float screenWidth,
        float screenHeight,
        const Matrix& view,
        const Matrix& proj,
        const Vector3& cameraPos);

    // 行列逆変換
    Vector4 InverseProjectionMatrix(
        const Vector4& v,
        const Matrix& m);

    // レイと平面の交点取得
    Vector3 RayToPlane(
        const Vector3& rayOrigin,
        const Vector3& rayDir,
        const Vector3& planePoint,
        const Vector3& planeNormal);

    // エフェクト更新
    void DownArrowEffect();

    // 接地判定
    const bool GetIsOnGround() const
    {
        return m_characterController.IsOnGround();
    }

    // 回転状態設定
    void SetRotationSeen(bool isRotation)
    {
        m_isRotation = isRotation;
    }

    // 回転状態取得
    bool GetRotationSeen() const
    {
        return m_isRotation;
    }

    // CharacterModel取得
    nsApp::CharacterModel* GetCharacterModel() const
    {
        return m_characterModel.get();
    }

    bool GetIsReplay() const
    {
        return m_isReplay;
    }

    float GetMeatRange() const
    {
        return m_meatRange;
    }

    Vector3 GetHitPosition() const
    {
        return m_hitPosition;
	}

    Ball* GetBall() const
    {
        return m_ball;
    }

    Game* GetGame() const 
    {
        return m_game;
    }
};