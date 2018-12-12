#pragma once

struct HashMakerParams;
struct Genome_t;

class Evaluation
{
public:
    double evaluate(const HashMakerParams& params, const Genome_t& genome);
};
