#pragma once

#include <stdint.h>

struct HashMakerParams
{
    size_t seed = 1;
    size_t populationSize = 150;
    size_t hashSize = 16;
    size_t minOperators = 3;
    size_t maxOperators = 20;
    size_t countTestData = 100'000;
    size_t minDataSize = 3;
    size_t maxDataSize = 100;
};
