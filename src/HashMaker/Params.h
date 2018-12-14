#pragma once

#include <stdint.h>

struct HashMakerParams
{
    size_t seed = 1;
    size_t populationSize = 300;
    size_t hashSize = 16;
    size_t minDataSize = 3;

    double mutationRate = 0.5;
    double operatorReplaceChance = 0.5;
    double operatorSwapChance = 0.2;
};
