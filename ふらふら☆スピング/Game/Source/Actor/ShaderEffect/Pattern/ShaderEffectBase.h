#pragma once

#include "IShaderEffectPattern.h"

// ファイル冒頭付近に追加（std::clampが使えない場合のため）
template <typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

class ShaderEffectBase: public IShaderEffectPattern
{
protected:


public:	
};