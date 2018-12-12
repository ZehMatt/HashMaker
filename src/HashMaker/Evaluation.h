#pragma once

#include <vector>
#include <stdint.h>

struct HashMakerParams;
struct Genome_t;

using Buffer = std::vector<uint8_t>;

class Evaluation
{
    std::vector<Buffer> _testData;
    size_t _hashSize;
    HashMakerParams& _parameters;

public:
    Evaluation(HashMakerParams& parameters);

    void reset();

    void evaluate(Genome_t& genome);
};
