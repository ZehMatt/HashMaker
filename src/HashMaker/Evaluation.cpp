#include "Evaluation.h"
#include "Genome.h"
#include "Params.h"
#include "HashContext.h"
#include "Operator.h"

#include <unordered_set>

using Buffer = std::vector<uint8_t>;

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

using HashCollection = std::unordered_set< Buffer, BufferHash >;

const std::vector<Buffer>& GetTestData(const HashMakerParams& params)
{
    static std::vector<Buffer> data;

    Random rnd(params.seed);

    if (data.size() != params.countTestData)
    {
        data.resize(params.countTestData);

        for (size_t i = 0; i < params.countTestData; i++)
        {
            Buffer& buf = data[i];
            
            size_t len = rnd.randomIntegerRange(params.minDataSize, params.maxDataSize);
            buf.resize(len);

            for (size_t n = 0; n < len; n++)
            {
                buf[n] = (uint8_t)rnd.randomIntegerRange(0x00, 0xFF);
            }
        }
    }

    return data;
}

double Evaluation::evaluate(const HashMakerParams& params, const Genome_t& genome)
{
    const std::vector<Buffer>& testData = GetTestData(params);

    // Check if operators have effect via input first.
    {
        HashContext_t ctx1 = {};
        ctx1.reset(params.hashSize);
        ctx1.currentInput = 0x00;
        for (const auto& op : genome.operators)
        {
            op->run(ctx1);
        }

        HashContext_t ctx2 = {};
        ctx2.reset(params.hashSize);
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

    for (auto&& buffer : testData)
    {
        HashContext_t ctx = {};
        ctx.reset(params.hashSize);

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
    }

    double fitnessOperations = (operations - (double)params.minOperators) / (double)(params.maxOperators - params.minOperators);
    fitnessOperations = 1.0 - fitnessOperations;

    double fitnessCollisions = 1.0 - (collisions / (double)testData.size());

    double totalFitness = fitnessOperations + fitnessCollisions;

    return std::pow(totalFitness, 4);
}

