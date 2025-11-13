#include "akinator.h"
#include "data_access.h"

//------------------------------------------------------------------------------------------

int main()
{
    Tree_t tree = {};

    do {
        DPRINTF("Program start\n");

        if (TreeCtor(&tree))
            break;

        if (TreeReadData(&tree, "data/data_13-Nov-2025_03-00-22.txt"))
            break;

        if (AkinatorCompareWords(&tree, "Босс Борис", "Баязит Музик"))
            break;

        if (AkinatorDescribeWord(&tree, "Босс Борис"))
            break;

        if (AkinatorDescribeWord(&tree, "Баязит Музик"))
            break;

        if (RunAkinator(&tree))
            break;

        if (TreeWriteData(&tree))
            break;
    } while (0);

    TreeDtor(&tree);

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------------------
