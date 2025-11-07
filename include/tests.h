#ifndef TESTS_H
#define TESTS_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "tree.h"
#include "common.h"
#include <stdlib.h>

//——————————————————————————————————————————————————————————————————————————————————————————

size_t RunTreeSortingTests (size_t nums_count, size_t tests_count);
int    RunTreeSortingTest  (size_t nums_count);
int    SortTestData        (int* data, int** sorted_data_ptr, size_t nums_count);
int    IsSorted            (int* array, size_t size);

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* TESTS_H */
