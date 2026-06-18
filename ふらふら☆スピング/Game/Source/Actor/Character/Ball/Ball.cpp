#include "stdafx.h"
#include "Ball.h"
#include <stdlib.h>
#include"Source/Scene/InGame/Game.h"
#include"Source/UI/InGameUI/InGameUI.h"
#include "Source/Effect/EffectManager.h"
#include"Source/Sound/SoundManager.h"
#include "Source/Actor/GameCamera/GameCamera.h"  

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
	m_modelRender.SetScale({ 8.5f,8.5f,8.0f });

	m_position = { -0.0f, 600.0f, 800.0f };
	m_modelRender.SetPosition(m_position);


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
    bool isReplay = (game && game->m_isReplayPlaying);
    if (!isReplay) {

        if (m_isPaused) return;
        if (!game->IsGameStarted()) return;
        if (game->m_isHitStop) return;
        if (game->m_isPaused) return;

        float dt = (1.0f / 60.0f) * game->GetTimeScale();

        m_throwTimer += dt;

        if (m_throwTimer >= 2.2f && !m_isMove)
        {
            ResetBall();

            Throw({ 0.0f, -20.0f, 0.0f });
            m_throwTimer = 0.0f;
        }

        if (!m_hasHit)
        {
            m_velocity.x += m_curveDir * 2.0f * dt;
        }

        if (m_isMove)
        {
            if (!m_hasHit)
            {
                float accelerationZ = 100.0f;
                m_velocity.z += accelerationZ * dt;
            }
            float baseGravity = 15.5f; // 元々のベース重力

            if (!m_hasHit && m_initialSpeedZ > 0.0f)
            {
                // 「今のZ速度 ÷ 加速前のZ速度」で、どれだけスピードアップしたかの倍率を出す
                float speedMultiplier = m_velocity.z / m_initialSpeedZ;

                // 前に進む勢いが強まった分だけ、下への重力もその2乗に比例して強める
                // これにより、速度が上がっても不自然に浮き上がらなくなります
                m_velocity.y -= baseGravity * (speedMultiplier * speedMultiplier) * dt;
            }
            else
            {
                // 打撃後や加速前は通常の重力処理
                m_velocity.y -= baseGravity * dt;
            }

            //  1. 元の m_velocity を破壊しないよう、このフレーム専用の速度変数を作る
            Vector3 currentFrameVelocity = m_velocity;

            // 2. スローボールかつ打撃前で、バッター手前に来たら一時変数の速度だけを半分にする
          // スローボールかつ打撃前で、バッター手前に来たときの処理
            if (!m_hasHit && m_ballType == SlowBall)
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
            m_position += currentFrameVelocity * dt;

            if (!m_hasHit)
            {
                switch (m_ballType)
                {
                case ShakeHorizontal:
                    m_position.x += sinf(m_position.z * 0.01f) * 3.0f;
                    break;

                case ShakeVertical:
                    m_position.y += sinf(m_position.z * 0.01f) * 3.0f;
                    break;

                case Straight:
                    break;

                case  Curve:
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

                    if (!m_hasShownPrediction && distance >= 10500.0f&&m_position.y >=150.0f) {

                        float predicted = PredictLandingDistance();
                        m_storedPredictedDistance = predicted;
                        InGameUI* ui = game->GetInGameUI();
                        if (ui) {
                            ui->ShowPrediction(predicted);
                        }

                        m_hasShownPrediction = true;
                    }

                    // ★ 空中で100m超えた瞬間にイベント発火
                    if (!game->m_hasTriggered100m && distance >= 11500.0f&&m_position.y>=100.0f) {
                        game->OnOver100m();
                        game->m_hasTriggered100m = true;
                    }
                }
            }



            // Z>=5500 の固定処理
            if (!m_hasFixed && m_position.z >= 6000.0f) {
                if (game) {
                    InGameUI* ui = game->GetInGameUI();
                    if (ui) {
                        ui->FixBallUI(m_position);
                    }
                }
                m_hasFixed = true;
            }

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
            if (!m_hasHit && m_position.z > 9000.0f) {
                Game* game = FindGO<Game>("game");
                if (game) {
                    game->m_isInputLocked = true;
                    InGameUI* ui = game->GetInGameUI();
                    if (ui) {
                        ui->OnStrike(game->m_shots);   // 今の球にバツを付ける
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
                m_isMove = false;

                Game* game = FindGO<Game>("game");
                if (game) {
                    float finalDistance = 0.0f;
                    InGameUI* ui = game->GetInGameUI();

                    if (m_hasHit) {
                        // ★ ポイント1：UIが表示されているなら、UIが持っている数値をそのまま採用する
                        if (m_hasShownPrediction) {
                            // Ballが計算した「生の数値」をそのまま使う（単位をcmに合わせる）
                            finalDistance = m_storedPredictedDistance;
                        }
                        else {
                            // 予測が出る前に着地した場合（ボテボテのゴロなど）
                            float dz = m_position.z - m_hitStartPos.z;
                            finalDistance = -dz;
                        }
                        m_hasHit = false;
                    }

                    // ★ ポイント2：ここで確実に finalDistance をセットする
                    game->SetKmValue(finalDistance);
                    game->OnBallLanded();
                }
            }

            if (m_isRecording) {
                m_replayPath.push_back(m_position);
            }
        }
      
     SetPosition(m_position);

     //距離に応じてスケール変更
     float minZ = 1000.0f;  // ピッチャーマウンド（スタート）
     float maxZ = 6200.0f;  // キャッチャー・バッター付近（最小になる位置）

     // Z座標から 0.0 〜 1.0 の割合(t)を計算
     float t = (m_position.z - minZ) / (maxZ - minZ);

     // 範囲外を安全にクランプ（0.0未満なら0.0、1.0より大きければ1.0に固定）
     t = fmaxf(0.0f, fminf(t, 1.0f));

     // --- スケール計算 ---
     float startScale = 5.5f; // ピッチャーリリース時の視認用サイズ（大きい）
     float finalScale = 2.5f;  // バッター手前での本来のサイズ（小さい）

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

       // いつでも左に寄せる処理（必要なければ 0.0f にしてください）
       loweredPos.x -= 100.0f;

       // ★ バッターに近づくほど、徐々に指定の高さ（-280.0f）へ沈み込ませる
       // t=0(投げた瞬間) はそのままの高さ、t=1(打たれる場所) でジャスト -280.0f 下がります
       loweredPos.y -= 290.0f * t;

       m_modelRender.SetPosition(loweredPos);

       // 2. リプレイ中のボール拡大処理
       float replayScale = 13.0f;
       m_modelRender.SetScale({ replayScale, replayScale, replayScale });
    }

    // ★ UI に毎フレーム位置を送る（必須）
    if (game) {
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->SetPredictedBallPos(m_position);
        }
    }

    m_modelRender.Update();
}


void Ball::Throw(const Vector3& targetPos)
{

    Vector3 dir = { 0.0f,-0.1f,3.0f };
    dir.Normalize();

    float speed = 2000.0f + (rand() % 250);
 
    if (rand() % 10 == 0)
    {
      m_isMagicBall = true; // 10%で魔球になる
    }
    else
    {
        m_isMagicBall = false; // 90%は通常球
    }
    //デバック用スローボール
  /*  if (rand() % 2 == 0)
    {
        m_ballType = SlowBall;
    }
    else
    {
        m_ballType = Straight;
    }*/

    // 【確率調整】0〜99の乱数を取得
    int rate = rand() % 100;

    if (rate < 24)
    {
        // 0〜23（24%の確率）でストレート
        m_ballType = Straight;
    }
    else if (rate < 46)
    {
        // 24〜45（22%の確率）で左右に揺れる
        m_ballType = ShakeHorizontal;
    }
    else if (rate < 56)
    {
        // 46〜55（10%の確率）でスローボール
        m_ballType = SlowBall;
    }
    else if (rate < 78)
    {
        // 56〜77（22%の確率）で上下に揺れる
        m_ballType = ShakeVertical;
    }
    else
    {
        // 78〜99（22%の確率）でカーブ
        m_ballType = Curve;
    }

    //スローボールに魔球を追加しない
    if (m_ballType == SlowBall)
    {
        m_isMagicBall = false;
    }

    //カーブ
    if (m_ballType == Curve)
    {
        if (rand() % 2 == 0)
            m_curveDir = -1;
        else
            m_curveDir = 1;
    }
    else
    {
        m_curveDir = 0;
    }

    m_velocity = dir * speed;
    m_initialSpeedZ = m_velocity.z;
    m_isMove = true;

    // ★ リプレイ記録開始（投球開始時）
    m_replayPath.clear();
    m_isRecording = true;

    // ★ 投げた瞬間の Z を UI に送る（必須）
    Game* game = FindGO<Game>("game");
    if (game) {
        game->m_isKakutei = false;
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->SetStartZ(m_position.z);
        }
    }
}

void Ball::SetPosition(const Vector3& pos)
{
    m_position = pos;
    m_modelRender.SetPosition(m_position);

    m_collisionObject->SetPosition(m_position);
    m_collisionObject->Update();
}

bool Ball::IsMoving() const
{
    return m_isMove;
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
        int shot = game->m_shots;
        game->m_hitStartZ = m_position.z;   // ★ 追加
        game->m_hasStartedDistance = true;   // ★ 追加
        // ★ 打った瞬間のフレームを保存
        game->m_hitFrame[shot] = game->GetReplayFrameCount();
        game->m_hitVelocities[game->m_shots] = m_velocity;     // ← 速度
        game->m_hitDirections[game->m_shots] = dir;            // ← 方向
        game->m_hitStartPos[game->m_shots] = m_position;       // ← 位置
        game->m_hitPower[game->m_shots] = hitPower;            // ← パワー
    }
    if (game) {
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->SetStartZ(m_position.z);
            ui->ResetBatAndMeetOnly();
        }
    }
    // ★ 打った瞬間の予測距離を計算
    float predicted = PredictLandingDistance();
    if (game) {
        bool isReplay = game->m_isReplayPlaying;
        // ★ パーフェクト閾値（あなたのUIと合わせる）
        bool isPerfect = (predicted >= 51500.0f);

        if (isPerfect&& !isReplay) {

            // ★ 確定演出フラグON
            game->m_isKakutei = true;
            game->m_kakuteiTimer = 1.0f;
            game->m_timeScale = 7.0f;
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
    }
}

float Ball::PredictLandingDistance()
{
    Game* game = FindGO<Game>("game");
    if (!game) return 0.0f;

    int shot = game->m_shots;

    // ★ 打った瞬間の位置と速度を使う
    Vector3 pos = m_hitStartPos;
    Vector3 vel = game->m_hitVelocities[shot];

    float dt = 1.0f / 60.0f;

    while (pos.y > 0.0f) {

        vel.y -= 13.5f * dt;   // 重力

        pos += vel * dt;
    }

    return m_hitStartPos.z - pos.z;
}

void Ball::ResetBall()
{
    m_position = { -0.0f, 650.0f, 1000.0f };
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
    m_ballType = Straight;
    m_curveDir = 0;
    SetPosition(m_position);
    Game* game = FindGO<Game>("game");
    if (game) {
        if (game->m_shots < 2) {
            game->m_isInputLocked = false;
        }
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->ResetBatAndMeetOnly();
        }
    }
}

void Ball::Render(RenderContext& rc)
{
    Game* game = FindGO<Game>("game");

    // 【追加】リプレイ中の特殊な非表示・表示ルール
    if (game && game->m_isReplayPlaying)
    {
        // 打った後は無条件で必ず描画する
        if (m_hasHit)
        {
            m_modelRender.Draw(rc);
            return;
        }

        if (game->m_replayDelayTimer > 0.0f)
        {
            return;
        }

        // 2. 打撃ゾーン（Z=7000）に到達するまでは消す（消える魔球演出の再現など）
        if (m_position.z > 6500.0f)
        {
            return;
        }
        // --- ここまで ---

        // 上記の非表示条件を抜けたら描画する（打つ直前の僅かな瞬間など）
        m_modelRender.Draw(rc);
        return;
    }

    // ★ 魔球の消える瞬間（Update側で1回だけ）
    if (m_isMagicBall && !m_hasHit)
    {
        if (!m_hasPlayedDisappearEffect &&
            m_position.z >= 5200.0f)
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

    // ★ 魔球が再出現した瞬間のエフェクト
    //if (m_isMagicBall && !m_hasHit)
    //{
    //    // 消えていた区間を抜けた瞬間（Z >= 6000）
    //    if (m_hasPlayedDisappearEffect && !m_hasPlayedReappearEffect)
    //    {
    //        if (m_position.z >= 6000.0f)
    //        {
    //            g_effectManager->PlayEffect(
    //                enEffect_kemuri,
    //                m_position,
    //                Vector3(10.0f, 10.0f, 10.0f)
    //            );
    //            g_soundManager->PlaySE(enSound_SE14);
    //            m_hasPlayedReappearEffect = true; // 二重発火防止
    //        }
    //    }
    //}


    if (!m_hasHit)
    {
        if (m_isMagicBall)
        {
            if (m_throwTimer < 0.9f&&m_position.z>1100.0f)
            {
                return;
            }
            if (m_position.z >= 5200.0f && m_position.z < 6000.0f)
            {
                return;
            }
        }
        else
        {
            if (m_throwTimer < 0.9f&&m_position.z>1100.0f)
            {
                return;
            }
        }

        // 一定距離で消す（通常プレイ中のバッター手前での消失処理など）
        if (game && game->m_isReplayPlaying &&m_position.z > 6900.0f)
        {
            return;
        }
    }

    // モデルの描画（通常プレイで打った後は無条件でここに来る）
    m_modelRender.Draw(rc);
}