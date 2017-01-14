#include "config.h"

Config::Config()
{
}

int Config::getColumnsCount()
{
    return 3;
}

double Config::getScaleParameter()
{
    return 0.2;
}

int Config::getBrightnessLevel()
{
    return 200;
}

int Config::getBrightnessMax()
{
    return 255;
}

int Config::getBrightnessMin()
{
    return 50;
}
