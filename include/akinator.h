#ifndef AKINATOR_H
#define AKINATOR_H

//——————————————————————————————————————————————————————————————————————————————————————————

#ifdef TREE_DEBUG
    #define DEBUG
#endif

//——————————————————————————————————————————————————————————————————————————————————————————

#include "tree_commands.h"
#include "colors.h"
#include "speak.h"
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

typedef enum AkinatorCmd
{
    AK_GUESS,
    AK_COMPARE,
    AK_DESCRIBE,
    AK_READ_DATA,
    AK_WRITE_DATA,

    AK_EXIT,
    AK_CMD_COUNT,

    AK_UNKNOWN_CMD
} AkinatorCmd_t;

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct AkinatorCtx
{
    Tree_t        tree;
    int           user_active;
    AkinatorCmd_t cmd;
} AkinatorCtx_t;

//——————————————————————————————————————————————————————————————————————————————————————————

AkinatorCmd_t GetUserCommand(AkinatorCtx_t* ak_ctx);

TreeErr_t AkinatorCtor              (AkinatorCtx_t* ak_ctx);
TreeErr_t AkinatorDtor              (AkinatorCtx_t* ak_ctx);
TreeErr_t AkinatorExecuteCmd        (AkinatorCtx_t* ak_ctx);
TreeErr_t AkinatorExecuteGuess      (AkinatorCtx_t* ak_ctx);
TreeErr_t AkinatorExecuteProgram    (AkinatorCtx_t* ak_ctx);
TreeErr_t AkinatorExecuteCompare    (AkinatorCtx_t* ak_ctx);
TreeErr_t AkinatorExecuteDescribe   (AkinatorCtx_t* ak_ctx);
TreeErr_t AkinatorExecuteReadData   (AkinatorCtx_t* ak_ctx);
TreeErr_t AkinatorExecuteWriteData  (AkinatorCtx_t* ak_ctx);

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t AkinatorNodeCtor          (Tree_t* tree, const char* word, TreeNode_t** node_ptr, TreeNode_t* parent);
TreeErr_t AkinatorGuessWord         (Tree_t* tree);
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

void      PrintComparison           (const char* word1, const char* word2,
                                     Stack_t* stack1, Stack_t* stack2, Stack_t* common_stack);
TreeErr_t ConstructComparisonStacks (Stack_t* stack1, Stack_t* stack2, Stack_t* common_stack);
TreeErr_t GetCommonConditionsStack  (const Tree_t* tree, Stack_t* stack1, Stack_t* stack2, StaÛck_t* common_stack);
void      DestroyComparisonStacks   (Stack_t* stack1, Stack_t* stack2, Stack_t* common_stack);
TreeErr_t GetComparisonStacks       (Stack_t* stack1, Stack_t* stack2, Stack_t* common_stack,
                                     TreeNode_t* node1, TreeNode_t* node2, Tree_t* tree);

TreeNode_t* TreeSearch            (TreeNode_t* node, const char* word);
int       TreeStepsEqual          (TreeStep_t* step1, TreeStep_t* step2);

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t AkinatorDescribeWord    (const Tree_t* tree, const char* word);
TreeErr_t PrintStackWordPath      (Stack_t* stack);
void      PrintCondition          (TreeStep_t* step);
void      PrintConditionFormatted (TreeStep_t* step, size_t stack_size);
TreeErr_t   GetStackWordPath      (const Tree_t* tree, TreeNode_t* leaf, Stack_t* stack);

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct AkCmdCase
{
    AkinatorCmd_t cmd;
    const char*   description_cmd;
    const char*   letter_cmd;

    TreeErr_t (* const execute_cmd) (AkinatorCtx_t*);
} AkCmdCase_t;

//——————————————————————————————————————————————————————————————————————————————————————————

const AkCmdCase_t AK_CMD_CASES_TABLE[] =
{
    [AK_GUESS]      = {AK_GUESS,      "óãàäûâàíèå ñëîâà",       "ó", AkinatorExecuteGuess},
    [AK_COMPARE]    = {AK_COMPARE,    "ñðàâíåíèå ñëîâ",         "ñ", AkinatorExecuteCompare},
    [AK_DESCRIBE]   = {AK_DESCRIBE,   "îïðåäåëåíèå ñëîâà",      "î", AkinatorExecuteDescribe},
    [AK_READ_DATA]  = {AK_READ_DATA,  "ñ÷èòàòü áàçó äàííûõ",    "÷", AkinatorExecuteReadData},
    [AK_WRITE_DATA] = {AK_WRITE_DATA, "çàïèñàòü â áàçó äàííûõ", "ç", AkinatorExecuteWriteData},
    [AK_EXIT]       = {AK_EXIT,       "âûõîä èç ïðîãðàììû",     "â", NULL}
};

//——————————————————————————————————————————————————————————————————————————————————————————

const int  MAX_INPUT_LEN          = 1024;
const char EMPTY_WORD[]           = "Íåèçâåñòíî ÷òî";
const char AK_DEFAULT_DATA_PATH[] = "data/data_13-Nov-2025_03-00-22.txt";

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* AKINATOR_H */
