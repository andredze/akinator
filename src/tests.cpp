#include "tests.h"

//------------------------------------------------------------------------------------------

int RunTreeExample()
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

    return 0;
}

//------------------------------------------------------------------------------------------

size_t RunTreeSortingTests(size_t nums_count, size_t tests_count)
{
    size_t passed_tests = 0;

    for (size_t i = 0; i < tests_count; i++)
    {
        passed_tests += !(RunTreeSortingTest(nums_count));
    }

    if (passed_tests == tests_count)
    {
        printf(GREEN "Passed (%zu/%zu) tests\n" RESET_CLR, passed_tests, tests_count);
    }
    else
    {
        printf(RED   "Passed (%zu/%zu) tests\n" RESET_CLR, passed_tests, tests_count);
    }

    return passed_tests;
}

//------------------------------------------------------------------------------------------

int RunTreeSortingTest(size_t nums_count)
{
    int* data = (int*) calloc(nums_count, sizeof(int));

    if (data == NULL)
    {
        PRINTERR("Memory allocation failed");
        return 1;
    }

    for (size_t i = 0; i < nums_count; i++)
    {
        data[i] = rand() % 1000;
    }

    int* sorted_data = NULL;

    if (SortTestData(data, &sorted_data, nums_count))
    {
        return 1;
    }

    int result = IsSorted(sorted_data, nums_count);

    free(sorted_data);

    return result;
}

//------------------------------------------------------------------------------------------

int SortTestData(int* data, int** sorted_data_ptr, size_t nums_count)
{
    Tree_t tree = {};

    if (TreeCtor(&tree))
    {
        TreeDtor(&tree);
        return 1;
    }

    for (size_t i = 0; i < nums_count; i++)
    {
        if (TreeInsert(&tree, data[i]))
        {
            TreeDtor(&tree);
            return 1;
        }
    }

    free(data);

    int* sorted_data = (int*) calloc(nums_count, sizeof(int));

    if (sorted_data == NULL)
    {
        PRINTERR("Memory allocation failed");
        return 1;
    }

    TreeSetValuesToArray(&tree, sorted_data);

    TreeDtor(&tree);

    *sorted_data_ptr = sorted_data;

    return 0;
}

//------------------------------------------------------------------------------------------

int IsSorted(int* array, size_t size)
{
    assert(array);

    DPRINTF("[");

    for (size_t i = 0; i < size - 1; i++)
    {
        DPRINTF("%d, ", array[i]);

        if (array[i] > array[i + 1])
        {
            DPRINTF("%d, ", array[i + 1]);
            printf(RED "\nArray is not sorted!\n" RESET_CLR);
            return 1;
        }
    }

    DPRINTF("%d]\n", array[size - 1]);

    printf(GREEN "Array is sorted!\n" RESET_CLR);

    return 0;
}

//------------------------------------------------------------------------------------------
