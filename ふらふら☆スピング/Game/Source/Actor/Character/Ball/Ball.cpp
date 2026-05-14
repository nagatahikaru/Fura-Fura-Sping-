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
	m_modelRender.SetScale({ 9.0f,9.0f,9.0f });

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
    if(!isReplay){

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

   if (m_isMove)
{
    m_velocity.y -= 14.0f * dt;

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
        if (!m_hasStrike && m_position.z >= 7270.0f) {
            g_soundManager->PlaySE(Sound::enSound_SE6);
        }

        // ★ ストライク判定（Z が 7000 を超えた瞬間）
        if (!m_hasStrike && m_position.z >= 7300.0f) {
            g_soundManager->PlaySE(Sound::enSound_SE5);
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
    float minZ = 1000.0f;
    float maxZ = 9500.0f;

    float t = (m_position.z - minZ) / (maxZ - minZ);

    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    float scale = 5.0f * (1.0f - t * 0.8f);

    //最小サイズ制限（消え防止）
    if (scale < 2.5f) scale = 2.0f;

    m_modelRender.SetScale({ scale, scale, scale });
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

    float speed = 1900.0f + (rand() % 200);

    int type = rand() % 3;

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
        }
    }
}

float Ball::PredictLandingDistance()
{
    Vector3 pos = m_position;
    Vector3 vel = m_velocity;

    float dt = 1.0f / 60.0f;

    // 地面に落ちるまでシミュレーション
    while (pos.y > 0.0f) {
        vel.y -= 14.5f * dt;   // 重力
        pos += vel * dt;
    }

    // 着地地点の Z から飛距離を算出
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
    SetPosition(m_position);
}

void Ball::Render(RenderContext& rc)
{

    // 一定距離で消える
    if (m_position.z > 7000.0f)
    {
        return;
    }

	//モデルの描画
		m_modelRender.Draw(rc);	
}