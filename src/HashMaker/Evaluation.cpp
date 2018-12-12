#include "Evaluation.h"
#include "Genome.h"
#include "Params.h"
#include "HashContext.h"
#include "Operator.h"

#include <unordered_set>

struct BufferHash
{
    size_t operator()(const Buffer& data) const
    {
        std::hash<uint8_t> hasher;
        size_t seed = 0;
        for (auto&& elem : data)
        {
            seed ^= hasher(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

using HashCollection = std::unordered_set<Buffer, BufferHash>;

struct DataGenerator
{
    void resize(size_t n)
    {
        states.resize(n);
        state.resize(n);
    }

    const std::vector<uint8_t>& next()
    {
        std::vector<uint8_t> res;
        res.resize(states.size());

        int32_t n = (int32_t)states.size() - 1;
        int32_t updateIndex = n;

        while (n >= 0)
        {
            state[n] = (uint8_t)states[n];

            if (updateIndex == n)
            {
                states[n]++;
                if (states[n] > 0xFF)
                {
                    states[n] = 0;
                    updateIndex--;
                }
            }

            n--;
        }
        if (updateIndex < 0)
        {
            states.push_back(0);
            state.push_back(0);
        }

        return state;
    }

private:
    std::vector<int32_t> states;
    std::vector<uint8_t> state;
};

static std::vector<Buffer> CreateTestData(const HashMakerParams& params)
{
    std::vector<Buffer> data;
    return data;
}

Evaluation::Evaluation(HashMakerParams& parameters)
    : _parameters(parameters)
{
}

void Evaluation::reset()
{
    _testData = CreateTestData(_parameters);
    _hashSize = _parameters.hashSize;
}

double Evaluation::evaluate(const Genome_t& genome)
{
    const std::vector<Buffer>& testData = _testData;

    // Check if operators have effect via input first.
    {
        HashContext_t ctx1 = {};
        ctx1.reset(_hashSize);
        ctx1.currentInput = 0x00;
        for (const auto& op : genome.operators)
        {
            op->run(ctx1);
        }

        HashContext_t ctx2 = {};
        ctx2.reset(_hashSize);
        ctx2.currentInput = 0xFF;
        for (const auto& op : genome.operators)
        {
            op->run(ctx2);
        }

        if (ctx1.data == ctx2.data)
        {
            return 0.0f;
        }
    }

    // Compute hashes for all the test data and store the hash in the collection.
    HashCollection uniqueHashes;
    uniqueHashes.reserve(testData.size());

    double collisions = 0.0;
    double operations = (double)genome.operators.size();

    size_t k_MaxIterations = 500'000;
    size_t dataSize = 16;

    DataGenerator gen;
    gen.resize(dataSize);

    double numTests = 0.0;
    double usedStates = 0.0;
    double totalStates = 0.0;
    for(size_t iter = 0; iter < k_MaxIterations; iter++)
    {
        const std::vector<uint8_t>& buffer = gen.next();
        if(buffer.size() > dataSize)
            break;

        numTests++;

        HashContext_t ctx = {};
        ctx.reset(_hashSize);

        for (auto&& byte : buffer)
        {
            ctx.currentInput = byte;

            for (const auto& op : genome.operators)
            {
                op->run(ctx);
            }
        }

        auto res = uniqueHashes.emplace(std::move(ctx.data));
        if (res.second == false)
        {
            collisions++;
        }

        usedStates += ctx.countUsed();
        totalStates += _hashSize;
    }

    double fitnessOperations = (operations - (double)_parameters.minOperators) / (double)(_parameters.maxOperators - _parameters.minOperators);
    fitnessOperations = 1.0 - fitnessOperations;

    double fitnessCollisions = 1.0 - (collisions / numTests);

    double fitnessStateUsed = usedStates / totalStates;

    double totalFitness = fitnessOperations + fitnessCollisions + fitnessStateUsed;

    return std::pow(totalFitness, 4);
}

