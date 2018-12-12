#pragma once

#include <vector>
#include <memory>

class IHashOperator;

struct Genome_t
{
    std::vector<std::unique_ptr<IHashOperator>> operators;
    double fitness = 0.0;
};
