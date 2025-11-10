#ifndef AKINATOR_H
#define AKINATOR_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "treeCommands.h"
#include <time.h>

//——————————————————————————————————————————————————————————————————————————————————————————

TreeErr_t RunAkinator       (Tree_t* tree);
TreeErr_t MakeEmptyNode     (Tree_t* tree);

TreeErr_t AkinatorGuessWord (Tree_t* tree, int* user_active);
TreeErr_t AkinatorMakeGuess (Tree_t* tree, TreeNode_t** node_ptr, int answer);

TreeErr_t AkinatorAddWord   (Tree_t* tree, TreeNode_t* guess_node);
TreeErr_t AkinatorGetNewWord(char** new_word_data);
TreeErr_t AkinatorGetFeature(char** feature_data, const char* guess_word, const char* new_word);

TreeErr_t AkinatorWriteData (const Tree_t* tree);

TreeErr_t WriteNode         (const TreeNode_t* node, FILE* fp);

//——————————————————————————————————————————————————————————————————————————————————————————

int   GetUserAnswer    (    void    );
int   GetShortAnswer   (char* str,  int size);
char* StringToLower    (char* dest, const char* src);
char  ToLower          (char  letter);
void  CleanBuffer      (    void    );
void  MakeDataFilePath (char* data_file_path);

//——————————————————————————————————————————————————————————————————————————————————————————

const int  MAX_TIME_LEN      = 64;
const int  MAX_INPUT_LEN     = 1024;
const int  MAX_FILE_NAME_LEN = 1024;
const char EMPTY_WORD[]      = "Неизвестно что";

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* AKINATOR_H */
