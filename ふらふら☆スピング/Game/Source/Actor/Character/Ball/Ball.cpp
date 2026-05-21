#include "stdafx.h"
#include "Ball.h"
#include <stdlib.h>
#include"Source/Scene/InGame/Game.h"
#include"Source/UI/InGameUI/InGameUI.h"
#include"Source/Sound/SoundManager.h"

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
	m_modelRender.SetScale({ 8.5f,8.5f,8.5f });

	m_position = { -0.0f, 650.0f, 1000.0f };
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
            m_velocity.y -= 13.5f * dt;

            m_position += m_velocity * dt;

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

                    if (!m_hasShownPrediction && distance >= 10500.0f) {

                        float predicted = PredictLandingDistance();
                        m_storedPredictedDistance = predicted;
                        InGameUI* ui = game->GetInGameUI();
                        if (ui) {
                            ui->ShowPrediction(predicted);
                        }

                        m_hasShownPrediction = true;
                    }

                    // ★ 空中で100m超えた瞬間にイベント発火
                    if (!game->m_hasTriggered100m && distance >= 10800.0f) {
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
    float minZ = 500.0f;
    float maxZ = 9500.0f;

        float t = (m_position.z - minZ) / (maxZ - minZ);
      
      if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;


    float scale = 4.0f * (1.0f - t * 0.8f);

        //最小サイズ制限（消え防止）
        if (scale < 3.0f) scale = 2.0f;

        m_modelRender.SetScale({ scale, scale, scale });
    }
    else
    {
        // ★★★【追加】リプレイ中のボール拡大処理 ★★★
        // 遠くに飛んでも見失わないよう、通常（最大5.0f）より大きいサイズに固定します
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
    int type = rand() % 4;

    switch (type)
    {
    case 0:
        m_ballType = Straight;
        break;

    case 1:
        m_ballType = ShakeHorizontal;
        break;

    case 2:
        m_ballType = ShakeVertical;
        break;

    case 3:
        m_ballType = Curve;
        break;
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

    m_isMove = true;

    // ★ リプレイ記録開始（投球開始時）
    m_replayPath.clear();
    m_isRecording = true;

    // ★ 投げた瞬間の Z を UI に送る（必須）
    Game* game = FindGO<Game>("game");
    if (game) {
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
    SetPosition(m_position);
    Game* game = FindGO<Game>("game");
    if (game) {
        InGameUI* ui = game->GetInGameUI();
        if (ui) {
            ui->ResetBatAndMeetOnly();
        }
    }
}

void Ball::Render(RenderContext& rc)
{
    if (!m_hasHit)
    {
        if (m_isMagicBall)
        {
            if (m_throwTimer < 1.2f)
            {
                return; 
            }
            if (m_position.z >= 5000.0f && m_position.z < 6000.0f)
            {
                return; 
            }
        }
        else
        {
            if (m_throwTimer < 1.2f)
            {
                return;
            }
        }

        // 一定距離で消す（通常プレイ中のバッター手前での消失処理など）
        if (m_position.z > 7000.0f)
        {
            return;
        }
    }

    // モデルの描画（打った後は無条件でここに来るため、必ず描画されます）
    m_modelRender.Draw(rc);
}