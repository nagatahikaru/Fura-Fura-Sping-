#pragma once

class Batter;

class IDebuffPattern
{
public:
    virtual ~IDebuffPattern() = default;
    virtual void Update(Batter* batter) = 0;
};

