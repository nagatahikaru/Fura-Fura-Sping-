#include "stdafx.h"
#include "Weather.h"

#include "Source/Effect/EffectManager.h"
#include "Source/Sound/SoundManager.h"

#include <cstdlib>

Weather::~Weather()
{
    StopEffects();
}


bool Weather::Start()
{
    // —‹‚ÌÅ‰‚Ì”­¶ŠÔ
    m_thunderTimer = 0.0f;
    m_thunderInterval = 5.0f + static_cast<float>(rand() % 6);

    StartRainEffect();
    StartWindEffect();

    return true;
}


void Weather::Update()
{
    UpdateThunder();
}


void Weather::Render(RenderContext& rc)
{
}


void Weather::StartRainEffect()
{
    if (!g_effectManager)
    {
        return;
    }

    if (m_weatherType == WeatherType::LightRain)
    {
        // ¬‰J
        m_rainEffectId = g_effectManager->PlayEffect(
            enEffect_ame,
            Vector3(0, 30000.0f, 5500.0f),
            Vector3(40, 40, 70),
            Quaternion::Identity
        );

        if (g_soundManager)
        {
            m_rainSE = g_soundManager->PlaySE(enSound_SE16);
        }
    }
    else if (m_weatherType == WeatherType::HeavyRain)
    {
        // ‘å‰J
        m_rainEffectId = g_effectManager->PlayEffect(
            enEffect_ame2,
            Vector3(0, 30000.0f, 5500.0f),
            Vector3(40, 40, 70),
            Quaternion::Identity
        );

        if (g_soundManager)
        {
            m_rainSE = g_soundManager->PlaySE(enSound_SE19);
        }
    }
}


void Weather::StartWindEffect()
{
    if (!g_effectManager)
    {
        return;
    }

    int r;

    // ‘å‰J‚Ìê‡
    // –³•—‚È‚µA4í—Ş‚©‚ç‘I‘ğ
    if (m_weatherType == WeatherType::HeavyRain)
    {
        r = rand() % 4;

        switch (r)
        {
        case 0:

            m_kazeEffectId = g_effectManager->PlayEffect(
                enEffect_kaze01,
                Vector3(0, 0.0f, 5000.0f),
                Vector3(40, 40, 70),
                Quaternion::Identity
            );

            m_currentWindType = Wind_LeftToRight;
            break;


        case 1:

            m_kaze2EffectId = g_effectManager->PlayEffect(
                enEffect_kaze02,
                Vector3(0, 0.0f, 5000.0f),
                Vector3(40, 40, 70),
                Quaternion::Identity
            );

            m_currentWindType = Wind_RightToLeft;
            break;


        case 2:

            m_kaze3EffectId = g_effectManager->PlayEffect(
                enEffect_kaze03,
                Vector3(0, 0.0f, 2000.0f),
                Vector3(40, 40, 70),
                Quaternion::Identity
            );

            m_currentWindType = Wind_Tailwind;
            break;


        case 3:

            m_kaze4EffectId = g_effectManager->PlayEffect(
                enEffect_kaze04,
                Vector3(0, 0.0f, 2000.0f),
                Vector3(40, 40, 70),
                Quaternion::Identity
            );

            m_currentWindType = Wind_Headwind;
            break;
        }

        m_isWindActive = true;

        if (g_soundManager)
        {
            m_kazeSE = g_soundManager->PlaySE(enSound_SE17);
        }
    }

    // °‚êE¬‰J
    else
    {
        // 5í—Ş
        // 4í—Ş‚Ì•— + –³•—
        r = rand() % 5;

        switch (r)
        {
        case 0:

            m_kazeEffectId = g_effectManager->PlayEffect(
                enEffect_kaze,
                Vector3(0, 0.0f, 5000.0f),
                Vector3(40, 40, 70),
                Quaternion::Identity
            );

            m_currentWindType = Wind_LeftToRight;
            break;


        case 1:

            m_kaze2EffectId = g_effectManager->PlayEffect(
                enEffect_kaze2,
                Vector3(0, 0.0f, 5000.0f),
                Vector3(40, 40, 70),
                Quaternion::Identity
            );

            m_currentWindType = Wind_RightToLeft;
            break;


        case 2:

            m_kaze3EffectId = g_effectManager->PlayEffect(
                enEffect_kaze3,
                Vector3(0, 0.0f, 2000.0f),
                Vector3(40, 40, 70),
                Quaternion::Identity
            );

            m_currentWindType = Wind_Tailwind;
            break;


        case 3:

            m_kaze4EffectId = g_effectManager->PlayEffect(
                enEffect_kaze4,
                Vector3(0, 0.0f, 2000.0f),
                Vector3(40, 40, 70),
                Quaternion::Identity
            );

            m_currentWindType = Wind_Headwind;
            break;


        case 4:

            m_currentWindType = Wind_None;
            m_isWindActive = false;
            break;
        }

        if (r != 4)
        {
            m_isWindActive = true;

            if (g_soundManager)
            {
                m_kazeSE = g_soundManager->PlaySE(enSound_SE17);
            }
        }
    }
}


void Weather::UpdateThunder()
{
    // ‘å‰J‚¾‚¯—‹‚ğ”­¶
    if (m_weatherType != WeatherType::HeavyRain)
    {
        return;
    }

    if (!g_effectManager || !g_gameTime)
    {
        return;
    }

    m_thunderTimer += g_gameTime->GetFrameDeltaTime();

    if (m_thunderTimer < m_thunderInterval)
    {
        return;
    }

    m_thunderTimer = 0.0f;

    // Ÿ‚Ì—‹‚Ü‚Å5`10•b
    m_thunderInterval =
        5.0f + static_cast<float>(rand() % 6);

    Vector3 thunderPos;

    int r = rand() % 10;

    if (r == 0)
    {
        float randX =
            static_cast<float>(rand() % 2001) - 2000.0f;

        float randZ =
            static_cast<float>(rand() % 1001) - 100.0f;

        thunderPos =
            Vector3(randX, 0.0f, randZ);
    }
    else
    {
        float randX =
            static_cast<float>(rand() % 10001) - 5000.0f;

        thunderPos =
            Vector3(randX, 2000.0f, -5000.0f);
    }

    g_effectManager->PlayEffect(
        enEffect_inazuma,
        thunderPos,
        Vector3(40, 40, 70),
        Quaternion::Identity
    );
    if (g_soundManager)
    {
        m_inazumaSE = g_soundManager->PlaySE(enSound_SE18);
    }
}


void Weather::StopEffects()
{
    if (g_effectManager)
    {
        if (m_rainEffectId != 0)
        {
            g_effectManager->StopEffect(m_rainEffectId);
            m_rainEffectId = 0;
        }

        if (m_kazeEffectId != 0)
        {
            g_effectManager->StopEffect(m_kazeEffectId);
            m_kazeEffectId = 0;
        }

        if (m_kaze2EffectId != 0)
        {
            g_effectManager->StopEffect(m_kaze2EffectId);
            m_kaze2EffectId = 0;
        }

        if (m_kaze3EffectId != 0)
        {
            g_effectManager->StopEffect(m_kaze3EffectId);
            m_kaze3EffectId = 0;
        }

        if (m_kaze4EffectId != 0)
        {
            g_effectManager->StopEffect(m_kaze4EffectId);
            m_kaze4EffectId = 0;
        }
    }

    if (m_rainSE)
    {
        m_rainSE->Stop();
        DeleteGO(m_rainSE);
        m_rainSE = nullptr;
    }

    if (m_kazeSE)
    {
        m_kazeSE->Stop();
        DeleteGO(m_kazeSE);
        m_kazeSE = nullptr;
    }
}