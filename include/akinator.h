#ifndef AKINATOR_H
#define AKINATOR_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "treeCommands.h"
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
TreeErr_t AkinatorNodeCtor          (Tree_t* tree, const char* word, TreeNode_t** node_ptr);
TreeErr_t AkinatorGuessWord         (Tree_t* tree, int* user_active);
TreeErr_t AkinatorMakeGuess         (Tree_t* tree, TreeNode_t** node_ptr, int answer);

void  CleanBuffer      (void);
int   GetUserAnswer    (void);
int   GetShortAnswer   (char* str);

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t AkinatorAddWord     (Tree_t* tree, TreeNode_t* guess_node);
TreeErr_t AkinatorGetNewWord  (char** new_word_data);
TreeErr_t AkinatorGetFeature  (char** feature_data, const char* guess_word, const char* new_word);
int       FeatureHasNegatives (char* feature);

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t AkinatorCompareWords      (Tree_t* tree, const char* word1, const char* word2);
void      PrintComparison           (Step_t* path1, Step_t* path2, int size1, int size2);
void      PrintConditionsInReverse  (Step_t* words_path, int start_pos, int end_pos);
int       StepsEqual                (Step_t* step1, Step_t* step2);
TreeErr_t AkinatorDescribeWord      (Tree_t* tree, const char* word);
TreeErr_t FindWordPath              (Tree_t* tree, const char* word, Step_t** words_path, int* pos);
int       DescribeWord              (TreeNode_t* node, const char* word, Step_t* words_path, int* pos);
void      PrintDescription          (Step_t* words_path, int size);
void      PrintCondition            (Step_t step, const char* ending);

//——————————————————————————————————————————————————————————————————————————————————————————

const int  MAX_INPUT_LEN     = 1024;
const char EMPTY_WORD[]      = "Неизвестно что";

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* AKINATOR_H */
