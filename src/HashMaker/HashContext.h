#pragma once

#include <vector>
#include <stdint.h>

struct HashContext_t
{
    std::vector<uint8_t> data;
    std::vector<uint8_t> used;
    uint8_t currentInput;

    void reset(size_t size)
    {
        data.resize(size);
        used.resize(size);
        for (size_t i = 0; i < size; i++)
        {
            data[i] = 0;
            used[i] = 0;
        }
    }

    size_t countUsed() const
    {
        size_t res = 0;
        for (auto&& n : used)
        {
            if(n)
                res++;
        }
        return res;
    }
};
