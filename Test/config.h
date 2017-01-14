#ifndef CONFIG_H
#define CONFIG_H

class Config
{
public:
    Config();

    int getColumnsCount();
    double getScaleParameter();
    int getBrightnessLevel();
    int getBrightnessMax();
    int getBrightnessMin();
};

#endif // CONFIG_H
