#include "tree.h"
#include "tests.h"

//------------------------------------------------------------------------------------------

int main()
{
    RunTreeExample();

#ifndef TREE_DEBUG
    RunTreeSortingTests(20, 10);
#endif

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------------------
