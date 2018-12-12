#pragma once

#include <vector>
#include <stdint.h>

struct HashContext_t
{
    std::vector<uint8_t> data;
    uint8_t currentInput;

    void reset(size_t size)
    {
        data.resize(size);
        for (size_t i = 0; i < size; i++)
        {
            data[i] = 0;
        }
    }
};
