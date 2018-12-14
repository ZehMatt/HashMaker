#include <iostream>
#include "HashMaker.h"
#include "TestData.h"

int main(int argc, const char *argv[])
{
    const char *testDataTextFile = nullptr;
    for (int i = 1; i < argc; i++)
    {
        if (_stricmp(argv[i], "-t") == 0)
        {
            if (i + 1 >= argc)
            {
                printf("Missing parameter -t\n");
                return -1;
            }
            testDataTextFile = argv[++i];
        }
    }

    TestData& testData = TestData::get();

    if (testDataTextFile != nullptr)
    {
        if (testData.importTextFile(testDataTextFile))
        {
            printf("Loaded test data: %s\n", testDataTextFile);
        }
    }

    HashMakerParams params;
    params.hashSize = 2;

    HashMaker maker;
    maker.reset(params);

    printf("TestData: %zu entries\n", testData.size());
    printf("Requested Hash Size: %zu bits\n", params.hashSize * 8);

    while (true)
    {
        maker.evaluate();
        maker.epoch();
        maker.printStats();
    }

    return 0;
}
