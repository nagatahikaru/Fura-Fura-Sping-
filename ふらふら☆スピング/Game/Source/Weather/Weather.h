#pragma once

#include "Source/Source.h"
#include "Source/Difficulty.h"

enum class WeatherType
{
    Sunny,
    LightRain,
    HeavyRain
};

enum WindType
{
    Wind_None,
    Wind_LeftToRight,
    Wind_RightToLeft,
    Wind_Tailwind,
    Wind_Headwind
};


class Weather :public Source {
public:
    Weather(){}
    ~Weather();

    bool Start();
    void Update();
    void Render(RenderContext& rc);

    void SetWeatherType(WeatherType type)
    {
        m_weatherType = type;
    }

    WeatherType GetWeatherType() const
    {
        return m_weatherType;
    }
    void SetDifficulty(Difficulty difficulty)   
    {
        m_difficulty = difficulty;
    }
    WindType GetWindType() const
    {
        return m_currentWindType;
    }

    bool IsWindActive() const
    {
        return m_isWindActive;
    }
private:
    void StartRainEffect();
    void StartWindEffect();
    void UpdateThunder();

    void StopEffects();
private:
    WeatherType m_weatherType = WeatherType::Sunny;
    Difficulty m_difficulty = Difficulty::Hard;
    WindType m_currentWindType = Wind_None;

    bool m_isWindActive = false;

    uint32_t m_rainEffectId = 0;
    uint32_t m_kazeEffectId = 0;
    uint32_t m_kaze2EffectId = 0;
    uint32_t m_kaze3EffectId = 0;
    uint32_t m_kaze4EffectId = 0;
    uint32_t m_inazumaEffectId = 0;

    SoundSource* m_rainSE = nullptr;
    SoundSource* m_kazeSE = nullptr;
    SoundSource* m_inazumaSE = nullptr;

    float m_thunderTimer = 0.0f;
    float m_thunderInterval = 5.0f;
};