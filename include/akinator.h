#ifndef AKINATOR_H
#define AKINATOR_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "tree_commands.h"
#include "stack.h"
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct Step
{
    char* word;
    int   is_condition;
} Step_t;

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t RunAkinator               (Tree_t* tree);
TreeErr_t AkinatorExecuteProgramOnce(Tree_t* tree, int answer, int* user_active);
TreeErr_t AkinatorNodeCtor          (Tree_t* tree, const char* word, TreeNode_t** node_ptr, TreeNode_t* parent);
TreeErr_t AkinatorGuessWord         (Tree_t* tree, int* user_active);
TreeErr_t AkinatorMakeGuess         (Tree_t* tree, TreeNode_t** node_ptr, int answer);

void  CleanBuffer      (void);
int   GetUserAnswer    (void);
int   GetShortAnswer   (char* str);

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t AkinatorAddWord       (Tree_t* tree, TreeNode_t* guess_node);
TreeErr_t AkinatorGetNewWord    (char** new_word_data);
TreeErr_t AkinatorGetCondition  (char** condition_data, const char* guess_word, const char* new_word);
int       ConditionHasNegatives (char* condition);

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t AkinatorCompareWords    (Tree_t* tree, const char* word1, const char* word2);
TreeErr_t AkinatorDescribeWord    (const Tree_t* tree, const char* word);
TreeErr_t GetCommonConditionsStack(Stack_t* stack1, Stack_t* stack2, Stack_t* common_stack);
int       TreeStepsEqual          (TreeStep_t* step1, TreeStep_t* step2);

TreeErr_t GetComparisonStacks(Stack_t* stack1, Stack_t* stack2, Stack_t* common_stack,
                              TreeNode_t* node1, TreeNode_t* node2, Tree_t* tree);

TreeErr_t ReverseStack(Stack_t* src_stack, Stack_t* dest_stack);

TreeErr_t PrintStackWordPath      (Stack_t* stack);
void      PrintCondition          (TreeStep_t* step);
void      PrintConditionFormatted (TreeStep_t* step, size_t stack_size);

TreeNode_t* TreeSearch            (TreeNode_t* node, const char* word);
TreeErr_t   GetStackWordPath      (const Tree_t* tree, TreeNode_t* leaf, Stack_t* stack);

//——————————————————————————————————————————————————————————————————————————————————————————

const int  MAX_INPUT_LEN = 1024;
const char EMPTY_WORD[]  = "Неизвестно что";

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* AKINATOR_H */
