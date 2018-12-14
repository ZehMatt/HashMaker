#pragma once

#include <stdint.h>

struct HashMakerParams
{
    size_t seed = 1;
    size_t populationSize = 300;
    size_t hashSize = 16;
    size_t minOperators = 3;
    size_t maxOperators = 20;
    size_t minDataSize = 3;
};
