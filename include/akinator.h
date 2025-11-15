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
#include <stddef.h>

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

TreeErr_t GetPathStack(Tree_t* tree, TreeNode_t** node, Stack_t* stack);

TreeErr_t AkinatorCompareNodes(Stack_t* stack1,
                               Stack_t* stack2,
                               const TreeNode_t* node1,
                               const TreeNode_t* node2);

void PrintComparison(Stack_t* stack1,
                     Stack_t* stack2,
                     const TreeNode_t* node1,
                     const TreeNode_t* node2,
                     size_t pos1,
                     size_t pos2);

TreeErr_t FindFirstDifference(Stack_t* stack1,   Stack_t* stack2,
                              size_t*  pos1_ptr, size_t*  pos2_ptr);

TreeNode_t* TreeSearch(TreeNode_t* node, const char* word, Stack_t* stack);

int       TreeStepsEqual          (TreeStep_t* step1, TreeStep_t* step2);

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t AkinatorDescribeWord(Stack_t* stack, const TreeNode_t* node);
TreeErr_t PrintStackWordPath(Stack_t* stack, size_t start_pos, size_t end_pos);
void      PrintCondition          (TreeStep_t* step);
void      PrintConditionFormatted (TreeStep_t* step, size_t stack_size);
TreeErr_t FillPathStack(Tree_t* tree, TreeNode_t** node, Stack_t* stack);

//——————————————————————————————————————————————————————————————————————————————————————————

typedef struct AkCmdCase
{
    AkinatorCmd_t cmd;
    const char*   description_cmd;
    const char*   letter_cmd; // wchar_t letter_cmd

    TreeErr_t (* const execute_cmd) (AkinatorCtx_t*);
} AkCmdCase_t;

//——————————————————————————————————————————————————————————————————————————————————————————

const AkCmdCase_t AK_CMD_CASES_TABLE[] =
{
    [AK_GUESS]      = {AK_GUESS,      "óãàäûâàíèå ñëîâà",       "ó", AkinatorExecuteGuess}, // L'ó'
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
