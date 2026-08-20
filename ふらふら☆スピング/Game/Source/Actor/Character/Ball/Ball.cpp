#include "stdafx.h"
#include "Ball.h"
#include <stdlib.h>
#include"Source/Scene/InGame/Game.h"
#include"Source/UI/InGameUI/InGameUI.h"
#include "Source/Effect/EffectManager.h"
#include"Source/Sound/SoundManager.h"
#include "Source/Actor/GameCamera/GameCamera.h"  
#include "Source/DifficultyParams.h"

namespace
{
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

    //  角度帯ごとのフェードアウト距離テーブル
    struct AngleThreshold
    {
        float maxAngleDeg;
        float distance;
    };

    const AngleThreshold kFadeThresholdTable[] = {
        { 10.0f, 30000.0f },
        { 20.0f, 28000.0f },
        { 30.0f, 24000.0f },
        { 40.0f, 20000.0f },
        { 50.0f, 16000.0f },
        { 60.0f, 12000.0f },
    };

    float GetThresholdFromTable(float angleDeg)
    {
        float absAngle = fabsf(angleDeg);
        for (const auto& entry : kFadeThresholdTable)
        {
            if (absAngle <= entry.maxAngleDeg)
            {
                return entry.distance;
            }
        }
        return kFadeThresholdTable[sizeof(kFadeThresholdTable) / sizeof(kFadeThresholdTable[0]) - 1].distance;
    }
}


Ball::Ball()
{
}

Ball::~Ball()
{
    if (m_collisionObject) {
        delete m_collisionObject;
        m_collisionObject = nullptr;
    }
}

bool Ball::Start()
{

    //モデルの読み込み
    m_modelRender.Init("Assets/modelData/Ball/Ball.tkm");
    m_modelRender.SetScale({ 3.0f,3.0f,3.0f });

    m_position = { -60.0f, 600.0f,-100.0f };
    m_throwStartPos = m_position;
    m_throwEndPos = m_position;
    m_modelRender.SetPosition(m_position);

    m_hasThrowOnce = false;


    // ★ UI に初期位置を送る（これが重要）
    Game* game = FindGO<Game>("game");
    if (game) {
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->SetPredictedBallPos(m_position);
            ui->SetStartZ(m_position.z);   // ← これを追加！
        }
    }

    //タイマー初期化
    m_throwTimer = 0.0f;
    m_isMagicBall = false;
    m_collisionObject = new CollisionObject();
    m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 25.0f);
    m_collisionObject->Update();

    return true;
}

void Ball::Update()
{
    Game* game = FindGO<Game>("game");
    if (!game) return;  // ← 最重要
    bool isReplay = (game && game->GetIsReplayPlaying());
    if (!isReplay) {

        if (m_isPaused) return;
        if (!game->IsGameStarted()) return;
        if (game->GetIsHitStop()) return;
        if (game->GetIsPaused()) return;
        if (game->GetIsInputLocked() && !m_isRolling) return;
        if (game->GetShouldContinueTutorial()) return;
        float dt = (1.0f / 60.0f) * game->GetTimeScale();

        m_throwTimer += dt;

        if (m_throwTimer >= 3.6f && !m_isMove)
        {
            ResetBall();

            Throw({ 0.0f, -15.0f, 0.0f });
            m_throwTimer = 0.0f;
        }

        if (!m_hasHit)
        {
            //m_velocity.x += m_curveDir * 2.0f * dt;
            if (m_ballType == Straight)
            {
                float minZ = 1000.0f;
                float maxZ = 5500.0f;
                float progress = RemapClamp(m_position.z, minZ, maxZ);

                float dropStartRatio = m_dropStartRatioOverride;

                float gravityScale;
                if (progress <= dropStartRatio)
                {
                    //序盤～中盤:重力はほぼ無効化→水平に伸びて見える
                    gravityScale = 0.6f;
                }
                else
                {
                    //終盤:一気に重力を強める
                    float t = RemapClamp(progress, dropStartRatio, 1.0f);
                    float easedT = t * t * t;
                    gravityScale = 0.5f + easedT * 5.0f;
                }

                m_velocity.y -= m_baseGravity * gravityScale * dt;
            }
            else if (m_initialSpeedZ > 0.0f)
            {
                float speedMultiplier = m_velocity.z / m_initialSpeedZ;
                float clamped = fminf(speedMultiplier, 1.2f);
                m_velocity.y -= m_baseGravity * (clamped * clamped) * dt;
            }
            else
            {
                m_velocity.y -= m_baseGravity * dt;
            }
        }

        if (m_isMove)
        {
            if (!m_hasHit)
            {
                float accelerationZ = 100.0f;
                m_velocity.z += accelerationZ * dt;
            }

            if (!m_hasHit && m_initialSpeedZ > 0.0f)
            {
                // 「今のZ速度 ÷ 加速前のZ速度」で、どれだけスピードアップしたかの倍率を出す
                float speedMultiplier = m_velocity.z / m_initialSpeedZ;

                // 前に進む勢いが強まった分だけ、下への重力もその2乗に比例して強める
                // これにより、速度が上がっても不自然に浮き上がらなくなります
                m_velocity.y -= m_baseGravity * (speedMultiplier * speedMultiplier) * dt;
            }
            else
            {
                // 打撃後や加速前は通常の重力処理
                m_velocity.y -= m_baseGravity * dt;
            }

        // ★ 風の影響（Hardのみ、打った後の打球にだけ適用）
        if (m_hasHit && game->GetIsWindActive())
        {
            float baseX = game->GetIsRainy() ? 1.2f : 1.0f; //雨、晴れの順
            float baseZ = game->GetIsRainy() ? -0.7f : -0.5f; //雨、晴れの順

            const float kWindPowerX = baseX * (1.0f + 0.001f * m_position.y);
            const float kWindPowerZ = baseZ * (1.0f + 0.001f * m_position.y);

            switch (game->GetCurrentWindType())
            {
            case Wind_LeftToRight:
                m_velocity.x += kWindPowerX * dt;
                break;
            case Wind_RightToLeft:
                m_velocity.x -= kWindPowerX * dt;
                break;
            case Wind_Tailwind:
                m_velocity.z -= kWindPowerZ * dt;
                break;
            case Wind_Headwind:
                m_velocity.z += kWindPowerZ * dt;
                if (m_velocity.z > 0.0f) m_velocity.z = 0.0f;
                break;
            default:
                break;
            }
        }

            //  1. 元の m_velocity を破壊しないよう、このフレーム専用の速度変数を作る
            Vector3 currentFrameVelocity = m_velocity;

            // 2. スローボールかつ打撃前で、バッター手前に来たら一時変数の速度だけを半分にする
          // スローボールかつ打撃前で、バッター手前に来たときの処理
            if (!m_hasHit && !m_isRolling && m_ballType == SlowBall)
            {
                if (m_position.z >= 5450.0f && m_position.z < 6500.0f)
                {
                    if (!isReplay) {
                        currentFrameVelocity *= 0.4f; // 通常プレイ時は減速
                    }
                    else {
                        currentFrameVelocity *= 1.0f; // リプレイ時は等速
                    }
                }
            }
            // 3. 安全に計算された currentFrameVelocity を使って座標を移動させる
            m_prevPosition = m_position;
            m_position += currentFrameVelocity * dt;
            m_throwEndPos = m_position;

            if (!m_hasHit && !m_isRolling)
            {
                //ランダムで決めたコース(m_pitchTargetX)へ向けての位置を補正
                float minZ = 1000.0f;
                float maxZ = 6200.0f;
                float progress = RemapClamp(m_position.z, minZ, maxZ);
                m_position.x = m_throwStartPos.x + (m_pitchTargetX - m_throwStartPos.x) * EaseOutCubic(progress);

                switch (m_ballType)
                {
                case ShakeHorizontal:
                    m_position.x += sinf(m_position.z * 0.01f) * 3.0f;
                    break;

                case ShakeVertical:
                    m_position.y += sinf(m_position.z * 0.008f) * 8.0f;
                    break;

                case Straight:
                    break;

                case  Curve:
                    Slider(dt);
                    break;
                default:
                    break;
                }
            }


            // ★ リアルタイム飛距離更新
           // 着地時の最終距離
          // ★ リアルタイム飛距離更新（HitBall してから着地まで）
            if (m_hasHit) {

                float distance = m_hitStartPos.z - m_position.z;
                if (distance < 0) distance = 0;

                if (game) {
                    game->SetKmValue(distance);

                    // ★ 角度を計算して角度帯テーブルからしきい値を取得
                    float dx = m_position.x - m_hitStartPos.x;
                    float dz = m_hitStartPos.z - m_position.z;
                    float angleDeg = atan2f(dx, dz) * (180.0f / 3.14159265f);
                    float threshold = GetFadeThresholdByAngle(angleDeg);

                    if (!m_hasShownPrediction && distance >= threshold && m_position.y >= 300.0f) {

                        float predicted = PredictLandingDistance();
                        m_storedPredictedDistance = predicted;

                        if (predicted >= threshold) {
                            InGameUI* ui = game->GetInGameUI();
                            if (ui) {
                                ui->ShowPrediction(predicted);
                            }
                            m_hasShownPrediction = true;
                        }
                        else {
                            m_hasShownPrediction = true;
                        }
                    }

                    // ★ 空中で一定距離超えた瞬間にイベント発火（予測しきい値+1000で発火）
                    if (!game->GetHasTriggered100m() && distance >= threshold + 1000.0f && m_position.y >= 250.0f) {
                        game->OnOver100m();
                        game->SetHasTriggered100m(true);
                    }
                }
            }

            //// Z>=5500 の固定処理
            //if (!m_hasFixed && m_position.z >= 5500.0f) {
            //        InGameUI* ui = game->GetInGameUI();
            //        if (ui) {
            //            ui->FixBallUI(m_finalPos);
            //        }
            //    
            //    m_hasFixed = true;
            //}

            // ★ ストライク判定（Z が 7000 を超えた瞬間）
            if (!m_hasPlayedSE6 && m_position.z >= 7270.0f) {
                g_soundManager->PlaySE(Sound::enSound_SE6);
                m_hasPlayedSE6 = true;
            }

            // ★ ストライク判定（Z が 7000 を超えた瞬間）
            if (!m_hasStrike && m_position.z >= 7700.0f) {
                g_soundManager->PlaySE(Sound::enSound_SE11);
                Game* game = FindGO<Game>("game");
                if (game) {
                    InGameUI* ui = game->GetInGameUI();
                    if (ui) {
                        ui->StartStrikeAnim();   // ← UI にアニメ開始を指示
                    }
                }
                m_hasStrike = true;  // 二重発火防止
            }

            // ★ 空振り判定（打撃ゾーンを通過したら次へ）
            if (!m_hasHit && !m_isRolling && m_position.z > 9000.0f) {
                Game* game = FindGO<Game>("game");
                if (game) {
                    game->SetIsInputLocked(true);
                    InGameUI* ui = game->GetInGameUI();
                    if (ui) {
                        ui->OnStrike(game->GetShots());   // 今の球にバツを付ける
                        ui->ResetBatAndMeetOnly();     // ★ 追加：空振りした瞬間にバットとミートをリセット
                    }
                    game->SetKmValue(0);   // 空振りは距離0
                    game->OnBallLanded();  // 次の球へ
                }
                m_isMove = false;
                return;
            }

            if (m_position.y <= 0.0f)
            {
                m_position.y = 0.0f;

                Game* game = FindGO<Game>("game");

                // ★ 初回着地時（まだ着地イベントを発行していない場合）だけ距離計算とイベント発火
                if (m_hasHit)
                {
                    if (game) {
                        float finalDistance = 0.0f;
                        InGameUI* ui = game->GetInGameUI();

                        if (m_hasShownPrediction) {
                            finalDistance = m_storedPredictedDistance;
                        }
                        else {
                            float dz = m_position.z - m_hitStartPos.z;
                            finalDistance = -dz;

                            if (ui) {
                                ui->ShowPrediction(finalDistance);
                            }
                        }

                        game->SetKmValue(finalDistance);
                        game->OnBallLanded();
                    }

                    m_hasHit = false;
                    m_velocity.y = 0.0f;   // ★ 着地したら上下速度は消す（跳ねない）
                    m_isRolling = true;    // ★ ここから転がり処理へ
                }

                // ★ 転がり中：水平速度を摩擦で少しずつ減衰させる
                if (m_isRolling)
                {
                    float horizSpeed = sqrtf(m_velocity.x * m_velocity.x + m_velocity.z * m_velocity.z);

                    if (horizSpeed > kRollingStopSpeed)
                    {
                        float newSpeed = horizSpeed - kRollingFriction * dt;
                        if (newSpeed < 0.0f) newSpeed = 0.0f;

                        float scale = (horizSpeed > 0.0001f) ? (newSpeed / horizSpeed) : 0.0f;
                        m_velocity.x *= scale;
                        m_velocity.z *= scale;
                    }
                    else
                    {
                        // 十分遅くなったので完全停止
                        m_velocity = Vector3::Zero;
                        m_isMove = false;
                        m_isRolling = false;
                    }
                }
            }

            if (m_isRecording) {
                m_replayPath.push_back(m_position);
            }
        }

        SetPosition(m_position);

        m_rotationAngle += m_rotateSpeed * dt;

        ////////
        Quaternion rot;
        rot.SetRotationDegX(m_rotationAngle);
        ///////

        m_modelRender.SetRotation(rot);



        //距離に応じてスケール変更
        float minZ = 1000.0f;  // ピッチャーマウンド（スタート）
        float maxZ = 6200.0f;  // キャッチャー・バッター付近（最小になる位置）

        // Z座標から 0.0 〜 1.0 の割合(t)を計算
        float t = (m_position.z - minZ) / (maxZ - minZ);

        // 範囲外を安全にクランプ（0.0未満なら0.0、1.0より大きければ1.0に固定）
        t = fmaxf(0.0f, fminf(t, 1.0f));

        // --- スケール計算 ---
        float startScale = 3.5f; // ピッチャーリリース時の視認用サイズ（小さい）
        float finalScale = 4.5f;  // バッター手前での本来のサイズ（大きい）

        // t=0.0(ピッチャー) のときは startScale、t=1.0(バッター) のときは finalScale になる線形補間
        float scale = startScale + (finalScale - startScale) * t;

        // 計算したスケールを適用
        m_modelRender.SetScale({ scale, scale, scale });
    }
    else {

        // ★ リプレイ中の位置・高さ調整
        Vector3 loweredPos = m_position;

        // ピッチャーマウンド(1000)からバッターボックス(6200)までの進捗率(0.0 〜 1.0)を計算
        float minZ = 1000.0f;
        float maxZ = 6200.0f;
        float t = (m_position.z - minZ) / (maxZ - minZ);
        t = fmaxf(0.0f, fminf(t, 1.0f)); // 0.0〜1.0にクランプ

        if (!m_hasHit) {
            // 投球スタート位置からの X座標の差分（曲がり幅）を計算
            float diffX = m_position.x - m_throwStartPos.x;

            // その差分を5倍（お好みで調整）にして描画位置に適用
            loweredPos.x = m_throwStartPos.x + (diffX * 5.0f);
        }

        // いつでも左に寄せる処理（必要なければ 0.0f にしてください）
       // loweredPos.x -= 100.0f;
        loweredPos.z -= 650.0f;
        // ★ バッターに近づくほど、徐々に指定の高さ（-280.0f）へ沈み込ませる
        // t=0(投げた瞬間) はそのままの高さ、t=1(打たれる場所) でジャスト -280.0f 下がります
        loweredPos.y -= 200.0f * t;

        m_modelRender.SetPosition(loweredPos);

        // 2. リプレイ中のボール拡大処理
        float replayScale = 14.0f;
        m_modelRender.SetScale({ replayScale, replayScale, replayScale });

        float dt = game->GetIsHitStop() ? 0.0f : g_gameTime->GetFrameDeltaTime();

        m_rotationAngle += m_rotateSpeed * dt;

        Quaternion rot;
        rot.SetRotationDegX(m_rotationAngle);
        m_modelRender.SetRotation(rot);
    }

    // ★ UI に毎フレーム位置を送る（必須）
    if (game) {
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            Vector3 uiPos = m_position;

            ui->SetPredictedBallPos(m_finalPos);
        }
    }

    m_modelRender.Update();
}



void Ball::Slider(float dt)
{
    float minZ = 1000.0f;
    float maxZ = 6500.0f; // ★ 6200.0f → 6500.0f（ヒットゾーンの出口）に変更

    float progress = RemapClamp(m_position.z, minZ, maxZ);
    float peakRatio = 0.6f; // ★ 0.35f → 0.55f に変更（戻り始めを遅らせる）
    float outwardAmount = 40.0f; // ★ 80.0f → 150.0f に変更

    float startX = m_throwStartPos.x;
    float targetX = m_pitchTargetX;
    float dir = (m_curveDir != 0) ? (float)m_curveDir : 1.0f;
    float peakX = startX + outwardAmount * dir;

    if (progress <= peakRatio)
    {
        float t = RemapClamp(progress, 0.0f, peakRatio);
        m_position.x = startX + EaseOutCubic(t) * outwardAmount * dir;
    }
    else if (progress < 1.0f)
    {
        float t = RemapClamp(progress, peakRatio, 1.0f);
        m_position.x = peakX + (targetX - peakX) * EaseInOutCubic(t);
    }
    else
    {
        m_position.x = targetX;
    }
}

void Ball::ApplyProSpiritsDrop(float dt)
{
    float minZ = 1000.0f;  //リリース地点
    float maxZ = 6500.0f;  //バッター手前(ヒットゾーン出口)

    float progress = RemapClamp(m_position.z, minZ, maxZ);

    float dropStartRatio = 0.72f;
    float totalDrop = 90.0f;

    float dropOffset = 0.0f;

    if (progress > dropStartRatio)
    {
        float t = RemapClamp(progress, dropStartRatio, 1.0f);

        float easedT = t * t * t;

        dropOffset = easedT * totalDrop;
    }

    m_position.y = m_pitchStartY - dropOffset;
}

// 角度帯テーブルからしきい値を取得
float Ball::GetFadeThresholdByAngle(float angleDeg) const
{
    return GetThresholdFromTable(angleDeg);
}

void Ball::Throw(const Vector3& targetPos)
{
    m_rotationAngle = 0.0f;
    m_pitchStartY = m_position.y;
    m_throwStartPos = m_position;
    m_targetPos = targetPos;
    m_throwEndPos = targetPos;

    m_hasThrowOnce = true;

    // ★ 1. ゲーム側から現在の難易度を取得する
    Difficulty currentDifficulty = Normal; // デフォルト
    Game* game = FindGO<Game>("game");
    if (game) {
        currentDifficulty = game->GetDifficulty(); // ※Gameクラスにある難易度取得関数
    }

    //難易度ごとの球速のベース値を変える
    float baseSpeed = 2000.0f;

    switch (currentDifficulty)
    {
    case Tutorial:
        baseSpeed = 1250.0f;
        m_baseGravity = 2.5f;
        break;
    case Easy:
        baseSpeed = 1250.0f;
        m_baseGravity = 2.5f;
        break;
    case Normal:
        baseSpeed = 1500.0f;
        m_baseGravity = 4.5f;
        break;
    default: //Hard
        baseSpeed = 2000.0f;
        m_baseGravity = 16.0f;
        break;
    }

    float speed = baseSpeed;

  /*  if (!m_isDifficultyConfigured) {
        const DifficultyParams& p = GetDifficultyParams(currentDifficulty);
        m_baseBallSpeed = p.ballBaseSpeed;
        m_baseGravity = p.ballGravity;
        m_isDifficultyConfigured = true;
    }

    SelectBallType(currentDifficulty);*/

    const float KReferenceSpeed = 2000.0f; //KReferenceSpeed:基準となる球速(Y方向の初速を計算するときに基準として使う球速の値)
    const float KInitialVYRatio = -0.1 / 4.0f; //KInitialVYRatio:傾きの比率(Z方向に対してY方向がどのくらいの割合)
    const float KFixedInitialVY = KInitialVYRatio * KReferenceSpeed; //常に同じ値

    //高さにもランダム幅を持たせる(値を大きくするほど上下のばらつきが増える)
    float randomVYOffset = ((rand() % 2001) / 1000.0f - 1.0f) * 8.0f;

    m_velocity.x = 0.0f;
    m_velocity.y = KFixedInitialVY;
    m_velocity.z = speed;

    m_initialSpeedZ = m_velocity.z;
    m_isMove = true;

    // ★ 2. 難易度に応じた球種の確率調整
    m_isMagicBall = false; // 初期化

    if (currentDifficulty == Easy || currentDifficulty == Tutorial)
    {
        m_ballType = Straight;
    }
    else if (currentDifficulty == Normal)
    {
        int rate = rand() % 100;
        if (rate < 60) {
            m_ballType = Straight;          // 70% ストレート
        }
        else if (rate < 30) {
            m_ballType = Curve;             // 20% カーブ
        }
        else {
            m_ballType = SlowBall;          // 10% スローボール
        }
    }
    else
    {
        if (rand() % 10 == 0) {
            m_isMagicBall = true;
        }

        int rate = rand() % 100;
        if (rate < 24)        m_ballType = Straight;
        else if (rate < 46)   m_ballType = ShakeHorizontal;
        else if (rate < 68)   m_ballType = Curve;
        else if (rate < 90)   m_ballType = ShakeVertical;
        else                  m_ballType = SlowBall;

        //////////////////////////////
       //m_ballType = Straight;
        //m_ballType = ShakeHorizontal;
       // m_ballType = Curve;
        //m_ballType = ShakeVertical;
       // m_ballType = SlowBall;
       /* if (rand() % 1 == 0) {
            m_isMagicBall = true;
        }*/
    }

    //スローボールに魔球を追加しない
    if (m_ballType == SlowBall)
    {
        m_isMagicBall = false;
    }

    if (game) {
        game->SetIsMagicBallShot(game->GetShots(), m_isMagicBall);
    }

    //カーブ
    if (m_ballType == Curve || m_ballType == ShakeHorizontal)
    {
        m_curveDir = (rand() % 2 == 0) ? -1.0f : 1.0f;
    }
    else
    {
        m_curveDir = 0.0f;
    }
     
    //投球コースをランダムに散らす(ワールド座標基準)
    {
        const float frameCenterX = 5.0f;
        const float targetHalfW = 15.0f;  //ここの数値で散らばりの幅を調整
        float randOffsetX = ((rand() % 2001) / 1000.0f - 1.0f) * targetHalfW;
        m_pitchTargetX = m_throwStartPos.x + randOffsetX;
    }

    // ★ リプレイ記録開始（投球開始時）
    m_replayPath.clear();
    m_isRecording = true;

    //投げた瞬間にボールの到達予定位置を計算
    m_finalPos = m_throwStartPos;

    const float finalZ = 5500.0f;

    //投球コースの最終位置
    float finalX = m_pitchTargetX;

    //予測点の左右位置の調整
    finalX += 7.0f;

    if (m_pitchTargetX < m_throwStartPos.x)
    {
        finalX += 3.0f;
    }

    //スローボールの時の補正
    if (m_ballType == SlowBall)
    {
        finalX += 1.0f;
    }
    //変化球の最終的な変化を反映
    if (m_ballType == Curve)
    {
        finalX += m_curveDir * m_breakAmount;
    }
    else if (m_ballType == ShakeHorizontal)
    {
        finalX += sinf(finalZ * 0.01f) * 3.0f;
    }

    //縦方向
    float finalY = m_pitchStartY;

    if (m_ballType == ShakeVertical)
    {
        finalY += sinf(finalZ * 0.008f) * 8.0f;
    }

    //最終位置
    m_finalPos = Vector3{
        finalX,
        finalY,
        finalZ
    };

    //投球開始時点でUIを最終到達位置に固定
    if (game)
    {
        InGameUI* ui = game->GetInGameUI();

        if (ui)
        {
            ui->FixBallUI(m_finalPos);
        }
    }

    // ★ 投げた瞬間の Z を UI に送る（必須）
    if (game) {
        game->SetIsKakutei(false);
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->SetStartZ(m_position.z);
        }
    }
}

void Ball::SetPosition(const Vector3& pos)
{
    m_position = pos;
    m_throwEndPos = pos;
    m_modelRender.SetPosition(m_position);

    m_collisionObject->SetPosition(m_position);
    m_collisionObject->Update();
}

bool Ball::IsMoving() const
{
    return m_isMove;
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

float Ball::GetFlightLength() const
{
    return (m_throwEndPos - m_throwStartPos).Length();
}

// ボールと指定した位置・半径の球との衝突判定
// ボールの中心と指定した位置との距離が、両方の半径の和より小さい場合に衝突とみなす
// 例: ボールの半径が10、指定した半径が5の場合、両方の半径の和は15
// ボールの中心と指定した位置との距離が15未満なら衝突と判定
// つまり、ボールの中心が指定した位置から15未満の距離にある場合に衝突とみなす
bool Ball::CheckCollision(const Vector3& pos, float radius)
{
    float dist = (m_position - pos).Length();
    return dist < (m_radius + radius);
}

void Ball::HitBall(const Vector3& hitDirection, float hitPower)
{
    Vector3 dir = hitDirection;
    dir.Normalize();
    m_velocity = dir * hitPower;
    m_isMove = true;
    // ★ 打った瞬間の位置を記録
    m_hitStartPos = m_position;
    m_hasHit = true;
    Game* game = FindGO<Game>("game");
    if (game) {
        int shot = game->GetShots();
        game->SetHitStartZ(m_position.z);   // ★ 追加
        game->SetHasStartedDistance(true);   // ★ 追加
        // ★ 打った瞬間のフレームを保存
        game->SetHitFrame(shot, game->GetReplayFrameCount());
        game->SetHitVelocity(game->GetShots(), m_velocity);     // ← 速度
        game->SetHitDirection(game->GetShots(), dir);            // ← 方向
        game->SetHitStartPos(game->GetShots(), m_position);       // ← 位置
        game->SetHitPower(game->GetShots(), hitPower);            // ← パワー
    }
    if (game) {
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->SetStartZ(m_position.z);
            ui->ResetBatAndMeetOnly();
            ui->HitBallUI();
        }
    }
    // ★ 打った瞬間の予測距離を計算
    float predicted = PredictLandingDistance();
    if (game) {
        bool isReplay = game->GetIsReplayPlaying();
        // ★ パーフェクト閾値（あなたのUIと合わせる）
        bool isPerfect = (predicted >= 99500.0f);

        if (isPerfect && !isReplay) {

            // ★ 確定演出フラグON
            game->SetIsKakutei(true);
            game->SetKakuteiTimer(1.0f);
            game->SetTimeScale(7.0f);
            GameCamera* cam = game->GetGameCamera();
            if (cam) {
                cam->SetkakuteiCamera();
                cam->FreezeCamera();
            }

            // ★ 集中線を出し続ける
            InGameUI* ui = game->GetInGameUI();
            if (ui) {
                ui->m_shuchusenTimer = 9999.0f;
            }
        }
        else if (!isReplay) {
            // ★ 通常ヒット時にも軽い間を入れる
            game->StartHitGlance(0.5f);
        }
    }
}

float Ball::PredictLandingDistance()
{
    Game* game = FindGO<Game>("game");
    if (!game) return 0.0f;

    int shot = game->GetShots();

    Vector3 pos = m_hitStartPos;
    Vector3 vel = game->GetHitVelocity(shot);

    float dt = 1.0f / 60.0f;
    Difficulty currentDifficulty = Normal;
    if (game) {
        currentDifficulty = game->GetDifficulty();
    }

    float gravity = 0.0f;
    switch (currentDifficulty)
    {
    case Tutorial: gravity = 5.5f; break;
    case Easy:     gravity = 5.5f; break;
    case Normal:   gravity = 9.5f; break;
    default:       gravity = 16.0f; break;
    }

    // ★ 風の影響もシミュレーション内で反映
    bool windActive = game->GetIsWindActive();
    WindType windType = game->GetCurrentWindType();

    float baseX = game->GetIsRainy() ? 1.2f : 1.0f;
    float baseZ = game->GetIsRainy() ? -0.7f : -0.5f;

    while (pos.y > 0.0f) {

        vel.y -= gravity * dt;

        if (windActive) {
            // ★ ここで毎回、シミュレーション中の pos.y を使って計算し直す
            const float kWindPowerX = baseX * (1.0f + 0.001f * pos.y);
            const float kWindPowerZ = baseZ * (1.0f + 0.001f * pos.y);

            switch (windType)
            {
            case Wind_LeftToRight: vel.x += kWindPowerX * dt; break;
            case Wind_RightToLeft: vel.x -= kWindPowerX * dt; break;
            case Wind_Tailwind:    vel.z -= kWindPowerZ * dt; break;
            case Wind_Headwind:
                vel.z += kWindPowerZ * dt;
                if (vel.z > 0.0f) vel.z = 0.0f;
                break;
            default: break;
            }
        }

        pos += vel * dt;
    }

    return m_hitStartPos.z - pos.z;
}

void Ball::ResetBall()
{
    m_position = { -60.0f, 650.0f, 1000.0f };
    m_velocity = Vector3::Zero;
    m_isMove = false;
    m_hasHit = false;
    m_hasFixed = false;
    m_hasStrike = false;
    m_hasShownPrediction = false;
    m_hasPlayedSE6 = false;
    m_isMagicBall = false;
    m_hasPlayedDisappearEffect = false;
    m_hasPlayedReappearEffect = false;
    m_hasThrowOnce = false;
    m_ballType = Straight;
    m_curveDir = 0;
    m_isRolling = false;
    SetPosition(m_position);
    Game* game = FindGO<Game>("game");
    if (game) {
        // 入力ロック解除は Game 側で一元管理するためここでは操作しない
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->ResetBatAndMeetOnly();
        }
    }
}
// 追加: 着地など外部から投球タイマーをリセットするための関数
void Ball::ResetThrowTimer()
{
    m_throwTimer = 0.0f;
}

void Ball::Render(RenderContext& rc)
{
   /* if (!m_hasThrowOnce)
    {
        return;
    }*/

    Game* game = FindGO<Game>("game");

    // 【追加】リプレイ中の特殊な非表示・表示ルール
    if (game && game->GetIsReplayPlaying())
    {
        if (m_hasHit)
        {
            m_modelRender.Draw(rc);
            return;
        }

        if (game->GetReplayDelayTimer() > 0.0f)
        {
            return;
        }

        // ★ 魔球の消える演出をリプレイでも再現
        if (m_isMagicBall)
        {
            if (!m_hasPlayedDisappearEffect && m_position.z >= 4600.0f)
            {
                g_effectManager->PlayEffect(
                    enEffect_kemuri,
                    m_position,
                    Vector3(50.0f, 50.0f, 50.0f)
                );
                g_soundManager->PlaySE(enSound_SE14);
                m_hasPlayedDisappearEffect = true;
            }

            if (m_position.z >= 4600.0f && m_position.z < 6000.0f)
            {
                return;
            }
        }

        // ★ 空振り球（打っていない）がキャッチャーミットに到達したら瞬時に消す
        if (!m_hasHit && m_position.z >= 6500.0f)
        {
            return;
        }

        m_modelRender.Draw(rc);
        return;
    }

    // ★ 魔球の消える瞬間（Update側で1回だけ）
    if (m_isMagicBall && !m_hasHit)
    {
        if (!m_hasPlayedDisappearEffect &&
            m_position.z >= 4600.0f)
        {
            g_effectManager->PlayEffect(
                enEffect_kemuri,
                m_position,
                Vector3(20.0f, 20.0f, 20.0f)
            );
            g_soundManager->PlaySE(enSound_SE14);

            m_hasPlayedDisappearEffect = true;
        }
    }

    if (!m_hasHit)
    {
        if (m_isMagicBall)
        {
            if (m_throwTimer < 0.9f && m_position.z>1100.0f)
            {
                return;
            }
            if (m_position.z >= 4600.0f && m_position.z < 6000.0f)
            {
                return;
            }
        }
        else
        {
            if (m_throwTimer < 0.9f && m_position.z>1100.0f)
            {
                return;
            }
        }

    }

    // モデルの描画（通常プレイで打った後は無条件でここに来る）
    m_modelRender.Draw(rc);
}
