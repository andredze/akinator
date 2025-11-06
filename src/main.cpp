#include "tree.h"
#include "treeGraph.h"

int main()
{
    Tree_t tree2 = {};

    do
    {
        if (TreeCtor(&tree2))
            break;

        if (TreeInsert(&tree2, 25))
            break;

        if (TreeInsert(&tree2, 20))
            break;

        if (TreeInsert(&tree2, 35))
            break;

        if (TreeInsert(&tree2, 40))
            break;

        if (TreeInsert(&tree2, 22))
            break;

        if (TreeInsert(&tree2, 21))
            break;

        if (TreeInsert(&tree2, 15))
            break;

        if (TreeInsert(&tree2, 30))
            break;

    }
    while (0);

    if (TreeDtor(&tree2))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
