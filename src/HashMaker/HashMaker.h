#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <vector>

#include "HashContext.h"
#include "Evaluation.h"
#include "Random.h"
#include "Operator.h"
#include "Genome.h"
#include "Params.h"

class HashMaker
{
public:
    HashMaker();

    void reset(const HashMakerParams& params);

    void evaluate();
    void epoch();
    void printStats();

private:
    
    void initializeGenome(Genome_t& genome);
    void evaluateGenome(Genome_t& genome);
    void epoch1();

    void crossover(Genome_t& child, const Genome_t& parentA, const Genome_t& parentB);

private:
    Random _random;
    HashMakerParams _parameters;
    Evaluation _evaluator;
    Genome_t _bestSolution;
    std::vector<Genome_t> _population;
    size_t _generation;
};
