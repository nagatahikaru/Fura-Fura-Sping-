#pragma once
#include "Source/Actor/Character/Character.h"
#include "Source/UI/InGameUI/InGameUI.h"
#include "BatterStateMachine.h"
#include "Source/Actor/Character/Ball/Ball.h"
#include "Source/Scene/InGame/Game.h"
#include "Source/Effect/EffectManager.h"
#include "Debuff/DebuffStageStateMachine.h"
#include "Debuff/DebuffStage/DebuffStage.h"

namespace {
	namespace BatterNumber {
		enum {
			UniformNumber_0,
			UniformNumber_1,
			UniformNumber_2,
			UniformNumber_3,
			UniformNumber_4,
			UniformNumber_5,
			UniformNumber_6,
			UniformNumber_7,
			UniformNumber_8,
			UniformNumber_9,
			Num
		};
	}

	namespace BatterBasicSettings
	{
		const Vector3 INITIAL_COORDINATE = Vector3(-420.0f, -50.0f, 5500.0f);//初期座標
		const float BASICS_SPEED = 400.0f; //基本速度
		const Vector3 VECTOR_UP = Vector3(0.0f, 1.0f, 0.0f); //上方向ベクトル
		const Vector3 VECTOR_LOOKAT = Vector3(1.0f, 0.0f, 0.0f); //注視点ベクトル
		const Vector3 INITIAL_SCALE = Vector3(10.0f, 10.0f, 10.0f); //初期スケール
		const Vector3 COLLISION_SCALE = Vector3(50.0f, 35.0f, 50.0f); //当たり判定スケール
		const float NONE_SPEED = 0.0f;//速度なし
	}

	namespace BatBasicSettings
	{
		const Vector3 INITIAL_SCALE = Vector3(10.0f, 10.0f, 10.0f); //初期スケール
		const Vector3 COLLISION_SCALE = Vector3(50.0f, 35.0f, 50.0f); //当たり判定スケール
		const Vector3 ROTATION_OFFSET = Vector3(1.0f, 0.0f, 0.0f); //回転の軸となるオフセット座標
		const float ROTATION_ANGLE = 90.0f; //回転角度
		const Vector3 COLLISION_SCALE_BAT = Vector3(114.285714286f, 80.0f, 40.0f);
	}
};

class Batter : public Character
{
public:

    //=========================================================
    // enum
    //=========================================================

    // アニメーションID
    enum EnAnimationClip
    {
        enAnimationClip_Idle,       // 待機
        enAnimationClip_Rotation,   // 回転
        enAnimationClip_Swing,      // スイング
        enAnimationClip_Num
    };

    //=========================================================
    // constructor / destructor
    //=========================================================

    Batter();
    virtual ~Batter();

    //=========================================================
    // override
    //=========================================================

    // 初期化
    virtual bool Start();

    // 更新処理
    virtual void Update();

    // 描画処理
    virtual void Render(RenderContext& rc);

    //=========================================================
    // animation
    //=========================================================

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
    void PlaySwingAnimation();

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

    // 再生速度設定
    void SetPlaySpeed(float speed)
    {
        m_playSpeed = speed;
    }

    // 再生速度取得
    float GetPlaySpeed() const
    {
        return m_playSpeed;
    }

    //=========================================================
    // cursor
    //=========================================================

    // カーソル座標更新
    void SetCursorPosition();

    // 3Dカーソル更新
    void UpdateCursor3D()
    {
        SetCursorPosition();
        m_meetCursorWorldPos = CalcCursorWorldPos();
    }

    // カーソル位置リセット
    void ResetCursorPosition();

    // カーソルモード切り替え
    void SetCursorMode(bool flag);

    // ワールド座標変換
    Vector3 CalcCursorWorldPos();

    // カーソル座標取得
    Vector3 GetCursorWorldPos() const
    {
        return m_meetCursorWorldPos;
    }

	//デバフによるカーソル揺れのオフセット追加
    void AddCursorOffset(const Vector3& offset)
    {
        m_cursorOffset += offset;
    }

    void SetHitRange(float range)
    {
        m_hitRange = range;
	}

    Vector3 GetMeetCursorPosition() const
    {
        return m_meetPosition;
	}

    //=========================================================
    // bat control
    //=========================================================

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

    // 回転回数カウント
    void GuruGuruCountUP(float currentAngle);

    // 回転回数取得
    int GetGuruGuruBatCount() const
    {
        return m_guruGuruBatCount;
    }

    // グルグル回数取得
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
    void HitEffect();

    //=========================================================
    // collision
    //=========================================================

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

    //=========================================================
    // raycast
    //=========================================================

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


    //=========================================================
    // effect
    //=========================================================

    // エフェクト更新
    void EffectUpdate();

    //=========================================================
    // utility
    //=========================================================

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

    // ランダム値取得
    float SetRandom(const float min, const float max)
    {
        return min + (max - min)
            * (rand() / (float)RAND_MAX);
    }

public:

    // 一時停止フラグ
    bool m_isPaused = false;

private:

    //=========================================================
    // state machine
    //=========================================================

    // バッターステート管理
    std::unique_ptr<BatterStateMachine> m_stateMachine;

    // デバフステート管理
    std::unique_ptr<DebuffStageStateMachine>
        m_debuffStageStateMachine;

    //=========================================================
    // external reference
    //=========================================================

    // ゲーム本体
    Game* m_game = nullptr;

    // UI
    InGameUI* m_inGameUI = nullptr;

    // ボール
    Ball* m_ball = nullptr;

    // 当たり判定
    CollisionObject* m_collisionObject = nullptr;

    // フォント描画
    FontRender m_fontRender;

    DebuffStage m_debuffStage;

    //=========================================================
    // transform / model
    //=========================================================

    // Transform
    Transform m_transform;

    // キャラクターモデル
    std::unique_ptr<nsApp::CharacterModel>
        m_characterModel;

    // 初期回転
    Quaternion m_initialRotation;

    // 現在回転
    Quaternion m_rotation;

    // 向き
    Vector3 m_facingDir =
        Vector3(0.0f, 0.0f, -1.0f);

    // 新座標
    Vector3 newPosition;

    //=========================================================
    // animation
    //=========================================================

    // 再生速度
    float m_playSpeed = 1.0f;

    // アニメーション配列
    AnimationClip m_animationClips[enAnimationClip_Num];

    // 現在アニメーション
    EnAnimationClip m_setAnimation =
        enAnimationClip_Idle;

    //=========================================================
    // cursor
    //=========================================================

    // カーソルモード
    bool m_isCursorMode = true;

    // ミート位置
    Vector3 m_meetPosition;

    // カーソルワールド座標
    Vector3 m_meetCursorWorldPos;

	// カーソルオフセット
    Vector3 m_cursorOffset;

	// ヒット範囲
    float m_hitRange = 100.0f;

    //=========================================================
    // bat
    //=========================================================

    // 回転有効
    bool m_isRotation = true;

    // 回転中心
    Vector3 pivotOffset =
    {
        0.0f,
        0.0f,
        10.0f
    };

    // 前回角度
    float m_prevAngle = 0.0f;

    // 回転タイマー
    float m_guruGuruBatTimer = 0.0f;

    // 総回転量
    float m_totalRotation = 0.0f;

    // 回転回数
    int m_guruGuruBatCount = 0;

    // ユニフォーム番号
    int m_UniformNumber =
        BatterNumber::UniformNumber_1;

    //=========================================================
    // hit
    //=========================================================

    // ヒット予約
    bool m_isHitReserved = false;

    // ヒット遅延
    float m_hitDelayTimer = 0.0f;

    // ヒット威力
    float m_reservedHitPower = 0.0f;

    // ヒット方向
    Vector3 m_reservedHitDir =
        Vector3::Zero;

    //=========================================================
    // effect
    //=========================================================

    struct EffectInfo
    {
        // 出現エフェクト
        uint32_t m_effectDawnID;

        // ヒットエフェクト
        uint32_t m_effectHitID;
    };

    // エフェクト情報
    EffectInfo m_inro;
};

