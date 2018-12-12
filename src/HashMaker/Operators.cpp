#include "Operator.h"
#include "Genome.h"

enum HashOperatorTypes : size_t
{
    HashOperatorTypeStateMovInput = 0,
    HashOperatorTypeStateMulMagic,
    HashOperatorTypeStateXorInput,
    HashOperatorTypeStateAddInput,
    HashOperatorTypeStateSubInput,
    HashOperatorTypeStateMovState,
    HashOperatorTypeStateXorState,
    HashOperatorTypeStateSubState,
    HashOperatorTypeStateAddState,
    HashOperatorTypeStateNot,
};

class HashOperatorStateMovInput : public IHashOperator
{
    size_t _index;
public:
    enum {
        k_Type = HashOperatorTypeStateMovInput
    };
    HashOperatorStateMovInput(size_t index) : _index(index) {}

    virtual void run(HashContext_t& context) override
    {
        context.data[_index] = context.currentInput;
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateMovInput>(_index);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "STATE[%zu] = INPUT", _index);
        return str;
    }
};

class HashOperatorStateMulMagic : public IHashOperator
{
    size_t _index;
    uint8_t _value;
public:
    enum {
        k_Type = HashOperatorTypeStateMulMagic
    };
    HashOperatorStateMulMagic(size_t index, uint8_t value) : _index(index), _value(value) {}

    virtual void run(HashContext_t& context) override
    {
        context.data[_index] *= _value;
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateMulMagic>(_index, _value);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "STATE[%zu] *= %u", _index, _value);
        return str;
    }
};

class HashOperatorStateXorInput : public IHashOperator
{
    size_t _index;

public:
    enum {
        k_Type = HashOperatorTypeStateXorInput
    };
    HashOperatorStateXorInput(size_t index) : _index(index) {}

    virtual void run(HashContext_t& context) override
    {
        context.data[_index] ^= context.currentInput;
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateXorInput>(_index);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "STATE[%zu] ^= INPUT", _index);
        return str;
    }
};

class HashOperatorStateAddInput : public IHashOperator
{
    size_t _index;

public:
    enum {
        k_Type = HashOperatorTypeStateAddInput
    };
    HashOperatorStateAddInput(size_t index) : _index(index) {}

    virtual void run(HashContext_t& context) override
    {
        context.data[_index] += context.currentInput;
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateAddInput>(_index);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "STATE[%zu] += INPUT", _index);
        return str;
    }
};

class HashOperatorStateSubInput : public IHashOperator
{
    size_t _index;

public:
    enum {
        k_Type = HashOperatorTypeStateSubInput
    };
    HashOperatorStateSubInput(size_t index) : _index(index) {}

    virtual void run(HashContext_t& context) override
    {
        context.data[_index] -= context.currentInput;
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateSubInput>(_index);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "STATE[%zu] -= INPUT", _index);
        return str;
    }
};

class HashOperatorStateMovState : public IHashOperator
{
    size_t _index1;
    size_t _index2;
public:
    enum {
        k_Type = HashOperatorTypeStateMovState,
    };
    HashOperatorStateMovState(size_t index1, size_t index2) : _index1(index1), _index2(index2) {}

    virtual void run(HashContext_t& context) override
    {
        context.data[_index1] = context.data[_index2];
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateMovState>(_index1, _index2);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "STATE[%zu] = STATE[%zu]", _index1, _index2);
        return str;
    }
};

class HashOperatorStateXorState : public IHashOperator
{
    size_t _index1;
    size_t _index2;
public:
    enum {
        k_Type = HashOperatorTypeStateXorState
    };
    HashOperatorStateXorState(size_t index1, size_t index2) : _index1(index1), _index2(index2) {}

    virtual void run(HashContext_t& context) override
    {
        context.data[_index1] ^= context.data[_index2];
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateXorState>(_index1, _index2);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "STATE[%zu] ^= STATE[%zu]", _index1, _index2);
        return str;
    }
};

class HashOperatorStateSubState : public IHashOperator
{
    size_t _index1;
    size_t _index2;
public:
    enum {
        k_Type = HashOperatorTypeStateSubState
    };
    HashOperatorStateSubState(size_t index1, size_t index2) : _index1(index1), _index2(index2) {}

    virtual void run(HashContext_t& context) override
    {
        context.data[_index1] -= context.data[_index2];
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateSubState>(_index1, _index2);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "STATE[%zu] -= STATE[%zu]", _index1, _index2);
        return str;
    }
};

class HashOperatorStateAddState : public IHashOperator
{
    size_t _index1;
    size_t _index2;
public:
    enum {
        k_Type = HashOperatorTypeStateAddState
    };
    HashOperatorStateAddState(size_t index1, size_t index2) : _index1(index1), _index2(index2) {}

    virtual void run(HashContext_t& context) override
    {
        context.data[_index1] += context.data[_index2];
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateAddState>(_index1, _index2);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "STATE[%zu] += STATE[%zu]", _index1, _index2);
        return str;
    }
};

class HashOperatorStateNot : public IHashOperator
{
    size_t _index;
public:
    enum {
        k_Type = HashOperatorTypeStateNot
    };
    HashOperatorStateNot(size_t index) : _index(index) {}

    virtual void run(HashContext_t& context) override
    {
        context.data[_index] = ~context.data[_index];
    }

    virtual std::unique_ptr<IHashOperator> clone() override
    {
        return std::make_unique<HashOperatorStateNot>(_index);
    }

    virtual std::string toString() override
    {
        char str[100] = {};
        sprintf_s(str, "STATE[%zu] = ~STATE[%zu]", _index, _index);
        return str;
    }
};

constexpr std::pair<size_t, double> k_Operators[] = 
{
    { HashOperatorStateMovInput::k_Type, 1.0 },
    { HashOperatorStateMulMagic::k_Type, 1.0 },
    { HashOperatorStateXorInput::k_Type, 1.0 },
    { HashOperatorStateAddInput::k_Type, 1.0 },
    { HashOperatorStateSubInput::k_Type, 1.0 },
    { HashOperatorStateMovState::k_Type, 1.0 },
    { HashOperatorStateXorState::k_Type, 1.0 },
    { HashOperatorStateSubState::k_Type, 1.0 },
    { HashOperatorStateAddState::k_Type, 1.0 },
    { HashOperatorStateNot::k_Type, 1.0 },
};

static std::vector<HashOperatorTypes>& GetOperatorList()
{
    static std::vector<HashOperatorTypes> res;
    if (res.empty())
    {
        for (size_t i = 0; i < std::size(k_Operators); i++)
        {
            size_t count = (size_t)std::ceil(k_Operators[i].second * 100.0);
            for (size_t n = 0; n < count; n++)
            {
                res.push_back((HashOperatorTypes)k_Operators[i].first);
            }
        }
    }
    return res;
}

void CreateOperators(const HashMakerParams& params, Genome_t& genome, Random& random)
{
    const std::vector<HashOperatorTypes>& operators = GetOperatorList();

    size_t count = random.randomIntegerRange(params.minOperators, params.maxOperators);
    for (size_t i = 0; i < count; i++)
    {
        std::unique_ptr<IHashOperator> op;

        const HashOperatorTypes& opType = random.randomElement(operators);
        switch (opType)
        {
        case HashOperatorStateMovInput::k_Type:
            {
                size_t index = random.randomIntegerRange(params.hashSize - 1);
                op = std::make_unique<HashOperatorStateMovInput>(index);
            }
            break;
        case HashOperatorStateMulMagic::k_Type:
            {
                size_t index = random.randomIntegerRange(params.hashSize - 1);
                uint8_t value = random.randomIntegerRange(0x00, 0xFF);
                op = std::make_unique<HashOperatorStateMulMagic>(index, value);
            }
            break;
        case HashOperatorStateXorInput::k_Type:
            {
                size_t index = random.randomIntegerRange(params.hashSize - 1);
                op = std::make_unique<HashOperatorStateXorInput>(index);
            }
            break;
        case HashOperatorStateAddInput::k_Type:
            {
                size_t index = random.randomIntegerRange(params.hashSize - 1);
                op = std::make_unique<HashOperatorStateAddInput>(index);
            }
            break;
        case HashOperatorStateSubInput::k_Type:
            {
                size_t index = random.randomIntegerRange(params.hashSize - 1);
                op = std::make_unique<HashOperatorStateSubInput>(index);
            }
            break;
        case HashOperatorStateMovState::k_Type:
            {
                size_t index1;
                size_t index2;
                do
                {
                    index1 = random.randomIntegerRange(params.hashSize - 1);
                    index2 = random.randomIntegerRange(params.hashSize - 1);
                } while (index1 == index2);
                op = std::make_unique<HashOperatorStateMovState>(index1, index2);
            }
            break;
        case HashOperatorStateXorState::k_Type:
            {
                size_t index1;
                size_t index2;
                do 
                {
                    index1 = random.randomIntegerRange(params.hashSize - 1);
                    index2 = random.randomIntegerRange(params.hashSize - 1);
                } while (index1 == index2);
                op = std::make_unique<HashOperatorStateXorState>(index1, index2);
            }
            break;
        case HashOperatorStateSubState::k_Type:
            {
                size_t index1;
                size_t index2;
                do
                {
                    index1 = random.randomIntegerRange(params.hashSize - 1);
                    index2 = random.randomIntegerRange(params.hashSize - 1);
                } while (index1 == index2);
                op = std::make_unique<HashOperatorStateSubState>(index1, index2);
            }
            break;
        case HashOperatorStateAddState::k_Type:
            {
                size_t index1;
                size_t index2;
                do
                {
                    index1 = random.randomIntegerRange(params.hashSize - 1);
                    index2 = random.randomIntegerRange(params.hashSize - 1);
                } while (index1 == index2);
                op = std::make_unique<HashOperatorStateAddState>(index1, index2);
            }
            break;
        case HashOperatorStateNot::k_Type:
            {
                size_t index = random.randomIntegerRange(params.hashSize - 1);
                op = std::make_unique<HashOperatorStateNot>(index);
            }
            break;
        }

        genome.operators.emplace_back(std::move(op));
    }
}
