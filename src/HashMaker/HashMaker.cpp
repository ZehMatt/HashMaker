#include "HashMaker.h"
#include "Operator.h"
#include "Evaluation.h"

HashMaker::HashMaker() : _evaluator(_parameters)
{
    reset(HashMakerParams());
}

void HashMaker::reset(const HashMakerParams& params)
{
    _parameters = params;
    _random.seed((unsigned int)params.seed);
    _generation = 0;

    _population.clear();
    _population.resize(params.populationSize);

    for (size_t i = 0; i < _population.size(); i++)
    {
        initializeGenome(_population[i]);
    }

    _evaluator.reset();
}

void HashMaker::initializeGenome(Genome_t& genome)
{
    CreateOperators(_parameters, genome, _random);
}

void HashMaker::evaluate()
{
    double bestFitness = 0.0;
    size_t bestIndex = 0;

    evaluatePopulation();

    for (size_t i = 0; i < _population.size(); i++)
    {
        Genome_t& genome = _population[i];

        if (genome.fitness > bestFitness)
        {
            bestFitness = genome.fitness;
            bestIndex = i;
        }
    }

    if (bestFitness > _bestSolution.fitness)
    {
        Genome_t& best = _population[bestIndex];

        _bestSolution.fitness = best.fitness;
        _bestSolution.collisionRate = best.collisionRate;
        _bestSolution.stateUsage = best.stateUsage;

        _bestSolution.operators.clear();
        for (auto&& op : best.operators)
        {
            _bestSolution.operators.emplace_back(op->clone());
        }
    }
}

void HashMaker::evaluatePopulation()
{
#pragma omp parallel for
    for (int32_t i = 0; i < _population.size(); i++)
    {
        Genome_t& genome = _population[i];

        evaluateGenome(genome);
    }
}

void HashMaker::evaluateGenome(Genome_t& genome)
{
    _evaluator.evaluate(genome);
}

void HashMaker::epoch()
{
    // TODO: Add more epoch modes.
    epoch1();

    _generation++;
}

void HashMaker::printStats()
{
    std::string solution;

    for (auto&& op : _bestSolution.operators)
    {
        if(solution.empty() == false)
            solution += "; ";
        solution += op->toString();
    }

    printf("Generation: %zu ; Fitness: %.08f ; Collisions: %.08f ; State: %.08f; -> %s\n",
        _generation, 
        _bestSolution.fitness,
        _bestSolution.collisionRate,
        _bestSolution.stateUsage,
        solution.c_str());
}

void HashMaker::epoch1()
{
    std::sort(_population.begin(), _population.end(), [](const Genome_t& a, const Genome_t& b)->bool
    {
        return a.fitness > b.fitness;
    });

    size_t half = _population.size() / 2;
    _population.resize(half);

    // Include random new ones.
    for (size_t i = 0; i < (half / 4); i++)
    {
        Genome_t child;
        initializeGenome(child);

        _population.emplace_back(std::move(child));
    }

    // Create children based 50% of the best new random ones.
    while(_population.size() < _parameters.populationSize)
    {
        size_t index1;
        size_t index2;
        do 
        {
            index1 = _random.randomIntegerRange<size_t>(0, half - 1);
            index2 = _random.randomIntegerRange<size_t>(0, half - 1);
        } while (index1 == index2);

        Genome_t childA;
        Genome_t childB;

        const Genome_t& parentA = _population[index1];
        const Genome_t& parentB = _population[index2];

        crossover(childA, parentA, parentB);
        crossover(childB, parentB, parentA);

        mutate(childA);
        mutate(childB);

        _population.emplace_back(std::move(childA));
        _population.emplace_back(std::move(childB));
    }

}

void HashMaker::crossover(Genome_t& child, const Genome_t& parentA, const Genome_t& parentB)
{
    size_t rangeMax = std::min(parentA.operators.size(), parentB.operators.size());
    size_t midPoint = _random.randomIntegerRange<size_t>(0, rangeMax - 1);

    size_t i;
    for (i = 0; i < midPoint; i++)
    {
        child.operators.emplace_back(parentA.operators[i]->clone());
    }
    for (; i < parentB.operators.size(); i++)
    {
        child.operators.emplace_back(parentB.operators[i]->clone());
    }
}

void HashMaker::mutate(Genome_t& genome)
{
    for (size_t i = 0; i < genome.operators.size(); i++)
    {
        if (_random.randomChance(_parameters.mutationRate))
        {
            genome.operators[i]->mutatate(_parameters, _random);
        }
        else if (_random.randomChance(_parameters.operatorSwapChance))
        {
            // Swap with new operator.
            HashContext_t ctx = {};
            ctx.data.resize(_parameters.hashSize);

            std::unique_ptr<IHashOperator> op;
            do 
            {
                op = CreateRandomOperator(_parameters, _random);
            } while (op->isValid(ctx) == false);

            genome.operators[i] = std::move(op);
        }
    }
}
