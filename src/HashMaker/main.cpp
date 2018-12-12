#include <iostream>
#include "HashMaker.h"

int main()
{
    HashMakerParams params;

    HashMaker maker;
    maker.reset(params);

    while (true)
    {
        maker.evaluate();
        maker.epoch();
        maker.printStats();
    }

    return 0;
}
