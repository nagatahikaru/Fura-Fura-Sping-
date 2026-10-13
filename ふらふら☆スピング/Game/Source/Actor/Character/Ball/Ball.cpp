#include "stdafx.h"
#include "Ball.h"
#include <stdlib.h>
#include "Source/Scene/InGame/Game.h"
#include "Source/UI/InGameUI/InGameUI.h"
#include "Source/Effect/EffectManager.h"
#include "Source/Sound/SoundManager.h"
#include "Source/Actor/GameCamera/GameCamera.h"
#include "Source/DifficultyParams.h"

namespace
{
    //==============================================================
    // 定数
    //==============================================================
    // Z座標（ピッチャー側が小さく、バッター側が大きい）
    constexpr float kReleaseZ = 1000.0f;       // リリース地点（ピッチャーマウンド）
    constexpr float kBatterZ = 6200.0f;        // バッター付近
    constexpr float kStraightDropEndZ = 5500.0f;
    constexpr float kHitZoneExitZ = 6500.0f;   // ヒットゾーン出口
    constexpr float kMagicHideStartZ = 4600.0f;  // 魔球が消え始める位置
    constexpr float kMagicHideEndZ = 6000.0f;    // 魔球が再び現れる位置
    constexpr float kWhiffZ = 9000.0f;         // これを超えたら空振り
    constexpr float kSlowBallZoneStartZ = 5000.0f;  // スローボールが減速する区間
    constexpr float kSlowBallZoneEndZ = 6000.0f;

    constexpr float kPi = 3.14159265f;

    // パーフェクト判定の飛距離（確定演出と軌跡エフェクトで共通）
    constexpr float kPerfectDistance = 99500.0f;

    // 軌跡エフェクト
    const Vector3 kTrailTailAxis(1.0f, 0.0f, 0.0f);  // efk上で「尾が伸びる方向」。向きが合わなければ調整する
    constexpr float kTrailScale = 42.0f;
    constexpr float kTrailOffsetX = 21.5f;           // ボールの中心とefkの原点のずれ（実機で合わせた値）

    // 軌跡エフェクトの位置：ボールの描画位置にずれの分を足す
    Vector3 TrailPosition(const Vector3& renderPos)
    {
        return Vector3(renderPos.x + kTrailOffsetX, renderPos.y, renderPos.z);
    }

    // 飛距離判定ごとの軌跡エフェクト。距離の大きい順に並べ、最初に minDistance 以上になった行を使う
    struct TrailRank
    {
        float minDistance;
        EffectType effect;
    };

    // InGameUI::ShowPrediction の判定と同じ下限値（距離はcm単位。100000 = 1000m）
    // ゴロ(kGoroDistance未満)は軌跡エフェクトを出さない
    constexpr float kGoroDistance = 32000.0f;
    constexpr float kGreatDistance = 70000.0f;

    const TrailRank kTrailRanks[] = {
        { 100000.0f, enEffect_Trajectory_perfect },    // パーフェクト
        {  90000.0f, enEffect_Trajectory_excellent },  // エクセレント
        {  kGreatDistance, enEffect_Trajectory_great },// グレート
        {  kGoroDistance, enEffect_Trajectory_nice },  // ナイス
    };

    EffectType GetTrailEffect(float distance)
    {
        for (const auto& rank : kTrailRanks)
        {
            if (distance >= rank.minDistance)
            {
                return rank.effect;
            }
        }
        return enEffect_Trajectory_nice;
    }

    // ゴロは軌跡なし
    bool HasTrailEffect(float distance) { return distance >= kGoroDistance; }

    //==============================================================
    // 数学ヘルパー
    //==============================================================
    float RemapClamp(float value, float start, float end)
    {
        if (end <= start) return (value >= end) ? 1.0f : 0.0f;
        float t = (value - start) / (end - start);
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        return t;
    }

    // 序盤に勢いよく外へ膨らませる（最初は速く、後半は減速）
    float EaseOutCubic(float t)
    {
        float x = 1.0f - t;
        return 1.0f - x * x * x;
    }

    // 中盤から終盤にかけて、ゆっくり始まりゆっくり終わる戻り
    float EaseInOutCubic(float t)
    {
        return (t < 0.5f)
            ? 4.0f * t * t * t
            : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) / 2.0f;
    }

    //==============================================================
    // 角度帯ごとのフェードアウト距離テーブル
    //==============================================================
    struct AngleThreshold
    {
        float maxAngleDeg;
        float distance;
    };

    const AngleThreshold kFadeThresholdTable[] = {
        { 10.0f, 32000.0f },
        { 20.0f, 30000.0f },
        { 30.0f, 26000.0f },
        { 40.0f, 20000.0f },
        { 50.0f, 16000.0f },
        { 60.0f, 14000.0f },
    };

    float GetThresholdFromTable(float angleDeg)
    {
        const float absAngle = fabsf(angleDeg);
        for (const auto& entry : kFadeThresholdTable)
        {
            if (absAngle <= entry.maxAngleDeg)
            {
                return entry.distance;
            }
        }
        return kFadeThresholdTable[_countof(kFadeThresholdTable) - 1].distance;
    }

    //==============================================================
    // 難易度ごとの球速・重力
    //==============================================================
    struct PitchParams
    {
        float speed;    // Z方向の初速
        float gravity;
    };

    PitchParams GetPitchParams(Difficulty difficulty)
    {
        switch (difficulty)
        {
        case Tutorial:
        case Easy:   return { 1250.0f, 2.5f };
        case Normal: return { 1500.0f, 4.5f };
        default:     return { 2000.0f, 16.0f };  // Hard
        }
    }

    //==============================================================
    // 風（実際の飛行と着地予測で共通）
    //==============================================================
    float GetWindDifficultyScale(Difficulty difficulty)
    {
        switch (difficulty)
        {
        case Tutorial: return 0.0f;
        case Easy:     return 0.3f;
        case Normal:   return 0.5f;
        case Hard:     return 1.0f;
        default:       return 1.0f;
        }
    }

    struct WindBase
    {
        float x;  // 横風の強さ
        float z;  // 向かい風・追い風の強さ
    };

    // 天候と難易度から風の基本の強さを求める
    WindBase GetWindBase(WeatherType weather, Difficulty difficulty)
    {
        WindBase base = { 1.0f, -0.5f };  // Sunny
        switch (weather)
        {
        case WeatherType::LightRain: base = { 1.1f, -0.6f }; break;
        case WeatherType::HeavyRain: base = { 1.2f, -0.7f }; break;
        default: break;
        }

        const float scale = GetWindDifficultyScale(difficulty);
        base.x *= scale;
        base.z *= scale;
        return base;
    }

    // 風を速度に加える。高いところほど風が強い
    void ApplyWindToVelocity(Vector3& vel, WindType windType, const WindBase& base, float height, float dt)
    {
        const float heightRate = 1.0f + 0.001f * height;
        const float powerX = base.x * heightRate;
        const float powerZ = base.z * heightRate;

        switch (windType)
        {
        case Wind_LeftToRight:
            vel.x += powerX * dt;
            break;
        case Wind_RightToLeft:
            vel.x -= powerX * dt;
            break;
        case Wind_Tailwind:
            vel.z -= powerZ * dt;
            break;
        case Wind_Headwind:
            vel.z += powerZ * dt;
            if (vel.z > 0.0f) vel.z = 0.0f;
            break;
        default:
            break;
        }
    }
}


//==================================================================
// 生成・破棄・初期化
//==================================================================
Ball::Ball()
{
}

Ball::~Ball()
{
    StopTrajectoryEffect();
    if (m_collisionObject) {
        delete m_collisionObject;
        m_collisionObject = nullptr;
    }
}

bool Ball::Start()
{
    // モデルの読み込み
    m_modelRender.Init("Assets/modelData/Ball/Ball.tkm");
    m_modelRender.SetScale({ 3.0f, 3.0f, 3.0f });

    m_position = { -60.0f, 600.0f, -100.0f };
    m_throwStartPos = m_position;
    m_throwEndPos = m_position;
    m_modelRender.SetPosition(m_position);

    // UI に初期位置を送る
    Game* game = FindGO<Game>("game");
    if (game) {
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->SetPredictedBallPos(m_position);
            ui->SetStartZ(m_position.z);
        }
    }

    // タイマー初期化
    m_throwTimer = 0.0f;
    m_isMagicBall = false;
    m_collisionObject = new CollisionObject();
    m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 25.0f);
    m_collisionObject->Update();

    return true;
}


//==================================================================
// 軌跡エフェクト
//==================================================================
void Ball::StartTrajectoryEffect(float distance)
{
    StopTrajectoryEffect();
    if (!g_effectManager || !HasTrailEffect(distance)) return;

    // 初期方向：速度があればそれ、なければ打球方向(奥→手前)の-Zを仮定
    Vector3 v = m_velocity;
    if (v.Length() > 0.001f) {
        v.Normalize();
        m_trailDir = v;
    }
    else {
        m_trailDir = Vector3(0.0f, 0.0f, -1.0f);
    }
    m_trailPrevPos = m_renderPos;
    m_trailRot = CalcTrailRotation(m_trailDir);

    // 飛距離判定に対応するエフェクトを、ボールの現在地から再生する
    m_trailEffectId = g_effectManager->PlayEffect(
        GetTrailEffect(distance),
        TrailPosition(m_renderPos),
        { kTrailScale, kTrailScale, kTrailScale },
        m_trailRot);
    m_hasTrailEffect = true;
}

void Ball::StopTrajectoryEffect()
{
    if (!m_hasTrailEffect) return;
    if (g_effectManager) {
        g_effectManager->StopEffect(m_trailEffectId);
    }
    m_hasTrailEffect = false;
}

void Ball::UpdateTrajectoryEffect()
{
    if (!m_hasTrailEffect || !g_effectManager) return;

    // 描画位置の差分から進行方向を求める
    Vector3 delta = m_renderPos - m_trailPrevPos;
    m_trailPrevPos = m_renderPos;

    if (delta.Length() > 0.01f) {  // 停止中・ヒットストップ中は向きを維持
        delta.Normalize();
        Vector3 blended = m_trailDir * 0.5f + delta * 0.5f;  // 急な変化をならす
        if (blended.Length() > 0.0001f) {
            blended.Normalize();
            m_trailDir = blended;
        }
        m_trailRot = CalcTrailRotation(m_trailDir);
    }

    // ボールの現在地(描画位置)に出す
    if (!g_effectManager->MoveEffect(
        m_trailEffectId,
        TrailPosition(m_renderPos),
        { kTrailScale, kTrailScale, kTrailScale },
        m_trailRot)) {
        m_hasTrailEffect = false;
    }
}

Quaternion Ball::CalcTrailRotation(const Vector3& travelDir) const
{
    // 尾はボールの進行方向の「後ろ」に伸ばす
    Vector3 target = travelDir * -1.0f;
    target.Normalize();

    const float dot = kTrailTailAxis.Dot(target);
    if (dot > 0.9999f) {
        return Quaternion::Identity;
    }

    Vector3 axis;
    float angle;
    if (dot < -0.9999f) {
        // 真逆：tailAxisと垂直な軸で180度回転
        axis = (fabsf(kTrailTailAxis.y) > 0.99f)
            ? Vector3(1.0f, 0.0f, 0.0f)
            : Vector3(0.0f, 1.0f, 0.0f);
        angle = kPi;
    }
    else {
        axis.Cross(kTrailTailAxis, target);
        axis.Normalize();
        angle = acosf(dot);
    }

    Quaternion rot;
    rot.SetRotation(axis, angle);
    return rot;
}


//==================================================================
// 更新
//==================================================================
void Ball::Update()
{
    Game* game = FindGO<Game>("game");
    if (!game) return;

    if (game->GetIsReplayPlaying()) {
        UpdateReplayVisual(game);
    }
    else if (!UpdatePlay(game)) {
        return;  // ポーズ中・空振りなど。以降の更新は行わない
    }

    // UI に毎フレーム到達予定位置を送る
    if (InGameUI* ui = game->GetInGameUI()) {
        ui->SetPredictedBallPos(m_finalPos);
    }

    m_modelRender.Update();
    UpdateTrajectoryEffect();  // 描画位置が確定した後に更新する
}

// 通常プレイの更新。途中で処理を打ち切った場合は false を返す
bool Ball::UpdatePlay(Game* game)
{
    if (m_isPaused) return false;
    if (!game->IsGameStarted()) return false;
    if (game->GetIsHitStop()) return false;
    if (game->GetIsPaused()) return false;
    if (game->GetIsInputLocked() && !m_isRolling && m_bounceCount == 0) return false;
    if (game->GetShouldContinueTutorial()) return false;

    const float dt = (1.0f / 60.0f) * game->GetTimeScale();

    // 一定時間たっても動いていなければ次の球を投げる
    m_throwTimer += dt;
    if (m_throwTimer >= 3.6f && !m_isMove)
    {
        ResetBall();
        Throw({ 0.0f, -15.0f, 0.0f });
        m_throwTimer = 0.0f;
    }

    if (!m_hasHit)
    {
        ApplyPreHitGravity(dt);
    }

    if (m_isMove)
    {
        ApplyMovingForces(dt);

        if (m_hasHit && game->IsWindActive())
        {
            ApplyWind(game, dt);
        }

        MoveBall(dt);

        if (!m_hasHit && !m_isRolling)
        {
            ApplyPitchCourse(dt);
        }

        if (m_hasHit)
        {
            UpdateHitDistance(game);
        }

        CheckPassEvents(game);

        if (CheckWhiff(game))
        {
            return false;
        }

        if (m_position.y <= 0.0f)
        {
            HandleGroundContact(game, dt);
        }

        if (m_isRecording) {
            m_replayPath.push_back(m_position);
        }
    }

    SetPosition(m_position);

    // 回転
    m_rotationAngle += m_rotateSpeed * dt;
    Quaternion rot;
    rot.SetRotationDegX(m_rotationAngle);
    m_modelRender.SetRotation(rot);

    UpdateModelScale(dt);
    return true;
}

// 打撃前の重力
void Ball::ApplyPreHitGravity(float dt)
{
    if (m_ballType == Straight)
    {
        const float progress = RemapClamp(m_position.z, kReleaseZ, kStraightDropEndZ);
        const float dropStartRatio = m_dropStartRatioOverride;

        float gravityScale;
        if (progress <= dropStartRatio)
        {
            // 序盤～中盤:重力はほぼ無効化→水平に伸びて見える
            gravityScale = 0.4f;
        }
        else
        {
            // 終盤:一気に重力を強める
            const float t = RemapClamp(progress, dropStartRatio, 1.0f);
            gravityScale = 0.4f + t * t * t * 6.0f;
        }

        m_velocity.y -= m_baseGravity * gravityScale * dt;
    }
    else if (m_initialSpeedZ > 0.0f)
    {
        const float speedMultiplier = m_velocity.z / m_initialSpeedZ;
        const float clamped = fminf(speedMultiplier, 1.2f);
        m_velocity.y -= m_baseGravity * (clamped * clamped) * dt;
    }
    else
    {
        m_velocity.y -= m_baseGravity * dt;
    }
}

// 移動中の加速・重力
void Ball::ApplyMovingForces(float dt)
{
    if (!m_hasHit)
    {
        const float accelerationZ = 100.0f;
        m_velocity.z += accelerationZ * dt;
    }

    if (!m_hasHit && m_initialSpeedZ > 0.0f)
    {
        // 前に進む勢いが強まった分だけ、下への重力もその2乗に比例して強める
        // （速度が上がっても不自然に浮き上がらなくなる）
        const float speedMultiplier = m_velocity.z / m_initialSpeedZ;
        m_velocity.y -= m_baseGravity * (speedMultiplier * speedMultiplier) * dt;
    }
    else
    {
        // 打撃後や加速前は通常の重力
        m_velocity.y -= m_baseGravity * dt;
    }
}

// 風（Hardのみ、打った後の打球にだけ適用）
void Ball::ApplyWind(Game* game, float dt)
{
    const WindBase base = GetWindBase(game->GetWeatherType(), game->GetDifficulty());
    ApplyWindToVelocity(m_velocity, game->GetWindType(), base, m_position.y, dt);
}

// 速度を位置に反映
void Ball::MoveBall(float dt)
{
    // m_velocity 自体は変えず、このフレーム専用の速度を使う
    Vector3 frameVelocity = m_velocity;

    // スローボールは打撃前にバッター手前へ来たら速度を半分にする
    if (!m_hasHit && !m_isRolling && m_ballType == SlowBall
        && m_position.z >= kSlowBallZoneStartZ && m_position.z < kSlowBallZoneEndZ)
    {
        frameVelocity *= 0.5f;
    }

    m_prevPosition = m_position;
    m_position += frameVelocity * dt;
    m_throwEndPos = m_position;
}

// 投球コース(m_pitchTargetX)への位置補正と変化球
void Ball::ApplyPitchCourse(float dt)
{
    const float progress = RemapClamp(m_position.z, kReleaseZ, kBatterZ);
    m_position.x = m_throwStartPos.x + (m_pitchTargetX - m_throwStartPos.x) * EaseOutCubic(progress);

    switch (m_ballType)
    {
    case ShakeHorizontal:
        m_position.x += sinf(m_position.z * 0.01f) * 3.0f;
        break;
    case ShakeVertical:
        m_position.y += sinf(m_position.z * 0.008f) * 8.0f;
        break;
    case Curve:
        Slider(dt);
        break;
    default:
        break;
    }
}

// 飛距離の更新・予測距離の表示・100m超えイベント
void Ball::UpdateHitDistance(Game* game)
{
    const float distance = fmaxf(0.0f, m_hitStartPos.z - m_position.z);
    game->SetKmValue(distance);

    // 予測表示も100m超えイベントも済んでいれば、しきい値は不要
    const bool needPrediction = !m_hasShownPrediction;
    const bool needOver100m = !game->GetHasTriggered100m();
    if (!needPrediction && !needOver100m) return;

    // 打球の角度に応じたしきい値
    const float dx = m_position.x - m_hitStartPos.x;
    const float dz = m_hitStartPos.z - m_position.z;
    const float angleDeg = atan2f(dx, dz) * (180.0f / kPi);
    const float threshold = GetFadeThresholdByAngle(angleDeg);

    // 一定距離まで飛んだ時点で着地位置を予測して表示
    if (needPrediction && distance >= threshold && m_position.y >= 300.0f)
    {
        const float predicted = PredictLandingDistance();
        m_storedPredictedDistance = predicted;

        if (predicted >= threshold)
        {
            if (InGameUI* ui = game->GetInGameUI()) {
                ui->ShowPrediction(predicted);
            }
        }
        m_hasShownPrediction = true;
    }

    // 空中で一定距離を超えた瞬間にイベント発火（しきい値+1000）
    if (needOver100m && distance >= threshold + 1000.0f && m_position.y >= 250.0f)
    {
        game->OnOver100m();
        game->SetHasTriggered100m(true);
    }
}

// 通過位置に応じたSE・ストライク演出
void Ball::CheckPassEvents(Game* game)
{
    if (!m_hasPlayedSE6 && m_position.z >= 7270.0f) {
        g_soundManager->PlaySE(Sound::enSound_SE6);
        m_hasPlayedSE6 = true;
    }

    if (!m_hasStrike && m_position.z >= 7700.0f) {
        g_soundManager->PlaySE(Sound::enSound_SE11);
        if (InGameUI* ui = game->GetInGameUI()) {
            ui->StartStrikeAnim();
        }
        m_hasStrike = true;  // 二重発火防止
    }
}

// 空振り判定（打撃ゾーンを通過したら次の球へ）。空振りなら true
bool Ball::CheckWhiff(Game* game)
{
    if (m_hasHit || m_isRolling || m_position.z <= kWhiffZ) {
        return false;
    }

    game->SetIsInputLocked(true);
    if (InGameUI* ui = game->GetInGameUI()) {
        ui->OnStrike(game->GetShots());  // 今の球にバツを付ける
        ui->ResetBatAndMeetOnly();       // バットとミートをリセット
    }
    game->SetKmValue(0);   // 空振りは距離0
    game->OnBallLanded();  // 次の球へ

    m_isMove = false;
    return true;
}

// 着地・バウンド・転がり
void Ball::HandleGroundContact(Game* game, float dt)
{
    m_position.y = 0.0f;

    // 初回着地時だけ距離を確定してイベントを発火する
    if (m_hasHit)
    {
        float finalDistance;
        InGameUI* ui = game->GetInGameUI();

        if (m_hasShownPrediction) {
            finalDistance = m_storedPredictedDistance;
        }
        else {
            finalDistance = m_hitStartPos.z - m_position.z;
            if (ui) {
                ui->ShowPrediction(finalDistance);
            }
        }

        game->SetKmValue(finalDistance);
        game->OnBallLanded();  // 初回着地のみ（バウンド中は再度呼ばない）

        m_hasHit = false;
        m_bounceCount = 0;
    }

    // バウンド：反発係数でY速度を反転、規定回数を超えたら転がりへ移行
    if (!m_isRolling)
    {
        const float incomingSpeed = fabsf(m_velocity.y);

        if (m_bounceCount < kMaxBounces && incomingSpeed > kMinBounceSpeed)
        {
            m_velocity.y = incomingSpeed * kBounceRestitution;  // 跳ねるたびに弱くなる
            m_bounceCount++;
        }
        else
        {
            m_velocity.y = 0.0f;
            m_isRolling = true;
        }
    }

    // 転がり中：水平速度を摩擦で少しずつ減衰させる
    if (m_isRolling)
    {
        const float horizSpeed = sqrtf(m_velocity.x * m_velocity.x + m_velocity.z * m_velocity.z);

        if (horizSpeed > kRollingStopSpeed)
        {
            const float newSpeed = fmaxf(0.0f, horizSpeed - kRollingFriction * dt);
            const float scale = newSpeed / horizSpeed;
            m_velocity.x *= scale;
            m_velocity.z *= scale;
        }
        else
        {
            // 十分遅くなったので完全停止
            m_velocity = Vector3::Zero;
            m_isMove = false;
            m_isRolling = false;
            StopTrajectoryEffect();
        }
    }
}

// 通常プレイ時の拡大率
void Ball::UpdateModelScale(float dt)
{
    float scale;

    if (m_wasHit)
    {
        // 打たれてから 0.5秒かけて大きくする
        const float kHitScaleDuration = 0.5f;
        const float kPreHitScale = 4.5f;  // 打たれる直前の投球スケール（kFinalScaleと合わせる）
        const float kHitScale = 8.5f;     // 最終的な大きさ

        m_hitScaleTimer += dt;
        const float t = RemapClamp(m_hitScaleTimer, 0.0f, kHitScaleDuration);
        scale = kPreHitScale + (kHitScale - kPreHitScale) * EaseOutCubic(t);
    }
    else
    {
        // ピッチャーからバッターへ近づくほど大きくする
        const float kStartScale = 3.5f;  // リリース直後（小さい）
        const float kFinalScale = 4.5f;  // バッター手前（本来のサイズ）

        const float t = RemapClamp(m_position.z, kReleaseZ, kBatterZ);
        scale = kStartScale + (kFinalScale - kStartScale) * t;
    }

    m_modelRender.SetScale({ scale, scale, scale });
}

// リプレイ中の描画位置・拡大・回転の更新
void Ball::UpdateReplayVisual(Game* game)
{
    // 描画位置：投球の曲がりを強調し、バッターに近づくほど沈ませる
    Vector3 drawPos = m_position;
    const float t = RemapClamp(m_position.z, kReleaseZ, kBatterZ);

    if (!m_hasHit) {
        const float kCurveGain = 5.0f;  // 曲がり幅の倍率
        drawPos.x = m_throwStartPos.x + (m_position.x - m_throwStartPos.x) * kCurveGain;
    }
    drawPos.z -= 650.0f;
    drawPos.y -= 200.0f * t;  // t=0(投げた瞬間)はそのまま、t=1(打たれる場所)で200下がる

    m_modelRender.SetPosition(drawPos);
    m_renderPos = drawPos;

    // 打球が始まったら軌跡を開始（一度だけ）。ベストショットを打った時の予測飛距離で判定する
    if (m_hasHit && !m_hasTrailEffect && !m_replayTrailStarted) {
        StartTrajectoryEffect(game->GetPredictedDistance(game->GetBestShotIndex()));
        m_replayTrailStarted = true;
    }

    // 拡大
    const float kReplayScale = 14.0f;
    m_modelRender.SetScale({ kReplayScale, kReplayScale, kReplayScale });

    // 回転
    const float dt = game->GetIsHitStop() ? 0.0f : g_gameTime->GetFrameDeltaTime();
    m_rotationAngle += m_rotateSpeed * dt;

    Quaternion rot;
    rot.SetRotationDegX(m_rotationAngle);
    m_modelRender.SetRotation(rot);
}


//==================================================================
// 変化球・落ち込み
//==================================================================
void Ball::Slider(float dt)
{
    const float peakRatio = 0.6f;        // 最も外へ膨らむ位置の割合
    const float outwardAmount = 40.0f;   // 外へ膨らむ量

    const float progress = RemapClamp(m_position.z, kReleaseZ, kHitZoneExitZ);

    const float startX = m_throwStartPos.x;
    const float targetX = m_pitchTargetX;
    const float dir = (m_curveDir != 0) ? m_curveDir : 1.0f;
    const float peakX = startX + outwardAmount * dir;

    if (progress <= peakRatio)
    {
        const float t = RemapClamp(progress, 0.0f, peakRatio);
        m_position.x = startX + EaseOutCubic(t) * outwardAmount * dir;
    }
    else if (progress < 1.0f)
    {
        const float t = RemapClamp(progress, peakRatio, 1.0f);
        m_position.x = peakX + (targetX - peakX) * EaseInOutCubic(t);
    }
    else
    {
        m_position.x = targetX;
    }
}

void Ball::ApplyProSpiritsDrop(float dt)
{
    const float progress = RemapClamp(m_position.z, kReleaseZ, kHitZoneExitZ);

    const float dropStartRatio = 0.72f;
    const float totalDrop = 90.0f;

    float dropOffset = 0.0f;
    if (progress > dropStartRatio)
    {
        const float t = RemapClamp(progress, dropStartRatio, 1.0f);
        dropOffset = t * t * t * totalDrop;
    }

    m_position.y = m_pitchStartY - dropOffset;
}


//==================================================================
// 投球
//==================================================================
void Ball::Throw(const Vector3& targetPos)
{
    m_rotationAngle = 0.0f;
    m_pitchStartY = m_position.y;
    m_throwStartPos = m_position;
    m_throwEndPos = targetPos;

    // 難易度に応じた球速・重力
    Game* game = FindGO<Game>("game");
    const Difficulty difficulty = game ? game->GetDifficulty() : Normal;
    const PitchParams params = GetPitchParams(difficulty);
    m_baseGravity = params.gravity;

    // 初速：Z方向は球速、Y方向は常に同じ値
    const float kReferenceSpeed = 2000.0f;
    const float kInitialVYRatio = -0.1 / 4.0f;  // Z方向に対してY方向がどのくらいの割合か
    const float kFixedInitialVY = kInitialVYRatio * kReferenceSpeed;

    // 元の処理で呼ばれていた(結果は未使用の)乱数。乱数列を変えないために残している
    (void)rand();

    m_velocity.x = 0.0f;
    m_velocity.y = kFixedInitialVY;
    m_velocity.z = params.speed;

    m_initialSpeedZ = m_velocity.z;
    m_isMove = true;

    // 球種・魔球
    ChoosePitchType(difficulty);
    if (game) {
        game->SetIsMagicBallShot(game->GetShots(), m_isMagicBall);
    }

    // カーブ・横揺れの曲がる向き
    if (m_ballType == Curve || m_ballType == ShakeHorizontal) {
        m_curveDir = (rand() % 2 == 0) ? -1.0f : 1.0f;
    }
    else {
        m_curveDir = 0.0f;
    }

    // 投球コースをランダムに散らす(ワールド座標基準)
    const float kTargetHalfWidth = 15.0f;  // 散らばりの幅
    const float randOffsetX = ((rand() % 2001) / 1000.0f - 1.0f) * kTargetHalfWidth;
    m_pitchTargetX = m_throwStartPos.x + randOffsetX;

    // リプレイ記録開始
    m_replayPath.clear();
    m_isRecording = true;

    // 投げた瞬間にボールの到達予定位置を計算
    const float finalZ = kStraightDropEndZ;
    float finalX = m_pitchTargetX + 7.0f;  // 予測点の左右位置の調整
    if (m_pitchTargetX < m_throwStartPos.x) {
        finalX += 3.0f;
    }
    if (m_ballType == SlowBall) {
        finalX += 1.0f;
    }
    else if (m_ballType == ShakeHorizontal) {
        finalX += sinf(finalZ * 0.01f) * 3.0f;
    }

    float finalY = m_pitchStartY;
    if (m_ballType == ShakeVertical) {
        finalY += sinf(finalZ * 0.008f) * 8.0f;
    }

    m_finalPos = Vector3{ finalX, finalY, finalZ };

    // UI に到達予定位置と投げた瞬間のZを送る
    if (game)
    {
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->FixBallUI(m_finalPos);
        }

        game->SetIsKakutei(false);

        if (ui) {
            ui->SetStartZ(m_position.z);
        }
    }
}

// 難易度に応じて球種・魔球を決める（乱数の呼び出し順は変えないこと）
void Ball::ChoosePitchType(Difficulty difficulty)
{
    m_isMagicBall = false;

    if (difficulty == Easy || difficulty == Tutorial)
    {
        m_ballType = Straight;
    }
    else if (difficulty == Normal)
    {
        const int rate = rand() % 100;
        if (rate < 70)      m_ballType = Straight;   // 70%
        else if (rate < 90) m_ballType = Curve;      // 20%
        else                m_ballType = SlowBall;   // 10%
    }
    else  // Hard
    {
        if (rand() % 10 == 0) {
            m_isMagicBall = true;
        }

        const int rate = rand() % 100;
        if (rate < 24)      m_ballType = Straight;
        else if (rate < 46) m_ballType = ShakeHorizontal;
        else if (rate < 68) m_ballType = Curve;
        else if (rate < 90) m_ballType = ShakeVertical;
        else                m_ballType = SlowBall;
    }

    // スローボールには魔球を付けない
    if (m_ballType == SlowBall)
    {
        m_isMagicBall = false;
    }
}


//==================================================================
// 位置・判定
//==================================================================
void Ball::SetPosition(const Vector3& pos)
{
    m_position = pos;
    m_renderPos = pos;
    m_throwEndPos = pos;
    m_modelRender.SetPosition(m_position);

    m_collisionObject->SetPosition(m_position);
    m_collisionObject->Update();
}

void Ball::GetFlightRay(Vector3& startPos, Vector3& endPos) const
{
    startPos = m_throwStartPos;
    endPos = m_throwEndPos;
}

Vector3 Ball::GetFlightDirection() const
{
    Vector3 dir = m_throwEndPos - m_throwStartPos;
    if (dir.Length() > 0.0001f) {
        dir.Normalize();
    }
    return dir;
}

float Ball::GetFadeThresholdByAngle(float angleDeg) const
{
    return GetThresholdFromTable(angleDeg);
}

// ボールと指定した球の衝突判定
// ボールの中心との距離が、両方の半径の和より小さければ衝突とみなす
bool Ball::CheckCollision(const Vector3& pos, float radius)
{
    const float dist = (m_position - pos).Length();
    return dist < (m_radius + radius);
}


//==================================================================
// 打撃
//==================================================================
void Ball::HitBall(const Vector3& hitDirection, float hitPower)
{
    Vector3 dir = hitDirection;
    dir.Normalize();
    m_velocity = dir * hitPower;
    m_isMove = true;
    m_hitStartPos = m_position;  // 打った瞬間の位置を記録
    m_hasHit = true;
    m_wasHit = true;
    m_hitScaleTimer = 0.0f;

    Game* game = FindGO<Game>("game");
    if (!game) {
        StartTrajectoryEffect(kGoroDistance);  // 判定が無い場合は通常の軌跡
        return;
    }

    // 打撃情報をゲームに保存（リプレイ用）
    const int shot = game->GetShots();
    game->SetHitStartZ(m_position.z);
    game->SetHasStartedDistance(true);
    game->SetHitFrame(shot, game->GetReplayFrameCount());
    game->SetHitVelocity(shot, m_velocity);
    game->SetHitDirection(shot, dir);
    game->SetHitStartPos(shot, m_position);
    game->SetHitPower(shot, hitPower);

    if (InGameUI* ui = game->GetInGameUI()) {
        ui->SetStartZ(m_position.z);
        ui->ResetBatAndMeetOnly();
        ui->HitBallUI();
    }

    // 打った瞬間の予測距離（HitVelocity の保存後に計算する）
    const float predicted = PredictLandingDistance();
    game->SetPredictedDistance(shot, predicted);  // リプレイの軌跡でも同じ判定を使う

    // 予測した飛距離の判定に対応する軌跡を、打った瞬間から出す
    StartTrajectoryEffect(predicted);

    const bool isReplay = game->GetIsReplayPlaying();
    const bool isPerfect = (predicted >= kPerfectDistance);  // UIのパーフェクト閾値と合わせる

    if (isPerfect && !isReplay)
    {
        // 確定演出
        game->SetIsKakutei(true);
        game->SetKakuteiTimer(1.0f);
        game->SetTimeScale(7.0f);
        if (GameCamera* cam = game->GetGameCamera()) {
            cam->SetkakuteiCamera();
            cam->FreezeCamera();
        }

        // 集中線を出し続ける
        if (InGameUI* ui = game->GetInGameUI()) {
            ui->m_shuchusenTimer = 9999.0f;
        }
    }
    else if (!isReplay)
    {
        // 通常ヒット時にも軽い間を入れる
        game->StartHitGlance(0.5f);
    }
}

// 打った瞬間の打球が着地するまでの水平距離を予測する
float Ball::PredictLandingDistance()
{
    Game* game = FindGO<Game>("game");
    if (!game) return 0.0f;

    Vector3 pos = m_hitStartPos;
    Vector3 vel = game->GetHitVelocity(game->GetShots());

    const float dt = 1.0f / 60.0f;
    const Difficulty difficulty = game->GetDifficulty();
    const float gravity = GetPitchParams(difficulty).gravity;

    // 風の影響もシミュレーション内で反映
    const bool windActive = game->IsWindActive();
    const WindType windType = game->GetWindType();
    const WindBase windBase = GetWindBase(game->GetWeatherType(), difficulty);

    while (pos.y > 0.0f) {
        vel.y -= gravity * dt;

        if (windActive) {
            // シミュレーション中の高さで風の強さを毎回計算し直す
            ApplyWindToVelocity(vel, windType, windBase, pos.y, dt);
        }

        pos += vel * dt;
    }

    return m_hitStartPos.z - pos.z;
}


//==================================================================
// リセット
//==================================================================
void Ball::ResetBall()
{
    m_position = { -60.0f, 650.0f, 1000.0f };
    m_velocity = Vector3::Zero;
    m_isMove = false;
    m_hasHit = false;
    m_wasHit = false;
    m_hitScaleTimer = 0.0f;
    m_hasStrike = false;
    m_hasShownPrediction = false;
    m_hasPlayedSE6 = false;
    m_isMagicBall = false;
    m_hasPlayedDisappearEffect = false;
    m_ballType = Straight;
    m_curveDir = 0.0f;
    m_bounceCount = 0;
    m_isRolling = false;
    SetPosition(m_position);
    StopTrajectoryEffect();

    // 入力ロック解除は Game 側で一元管理するためここでは操作しない
    Game* game = FindGO<Game>("game");
    if (game) {
        if (InGameUI* ui = game->GetInGameUI()) {
            ui->ResetBatAndMeetOnly();
        }
    }
}

// 着地など外部から投球タイマーをリセットするための関数
void Ball::ResetThrowTimer()
{
    m_throwTimer = 0.0f;
}


//==================================================================
// 描画
//==================================================================
// 魔球が消える演出（位置に到達した瞬間に一度だけ）
void Ball::TryPlayDisappearEffect(float effectScale)
{
    if (m_hasPlayedDisappearEffect || m_position.z < kMagicHideStartZ) {
        return;
    }

    g_effectManager->PlayEffect(
        enEffect_kemuri,
        m_position,
        Vector3(effectScale, effectScale, effectScale));
    g_soundManager->PlaySE(enSound_SE14);
    m_hasPlayedDisappearEffect = true;
}

void Ball::Render(RenderContext& rc)
{
    Game* game = FindGO<Game>("game");
    const bool isMagicHidden = m_isMagicBall
        && m_position.z >= kMagicHideStartZ && m_position.z < kMagicHideEndZ;

    // リプレイ中の表示ルール
    if (game && game->GetIsReplayPlaying())
    {
        // 打球は常に表示
        if (m_hasHit)
        {
            m_modelRender.Draw(rc);
            return;
        }

        if (game->GetReplayDelayTimer() > 0.0f) return;

        // 魔球の消える演出をリプレイでも再現
        if (m_isMagicBall)
        {
            TryPlayDisappearEffect(50.0f);
            if (isMagicHidden) return;
        }

        // 空振り球がキャッチャーミットに到達したら消す
        if (m_position.z >= kHitZoneExitZ) return;

        m_modelRender.Draw(rc);
        return;
    }

    if (!m_hasHit)
    {
        // 魔球の消える瞬間
        if (m_isMagicBall)
        {
            TryPlayDisappearEffect(20.0f);
        }

        // 投げた直後は表示しない
        if (m_throwTimer < 0.9f && m_position.z > 1100.0f) return;

        // 魔球はバッター手前で見えなくなる
        if (isMagicHidden) return;
    }

    // 通常プレイで打った後は無条件でここに来る
    m_modelRender.Draw(rc);
}