#ifndef DATA_ACCESS_H
#define DATA_ACCESS_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "treeCommands.h"
#include <ctype.h>

//——————————————————————————————————————————————————————————————————————————————————————————

void      WriteNode         (const TreeNode_t* node, FILE* fp);

TreeErr_t ReadNode          (Tree_t* tree, TreeNode_t** node, char* buffer, int* i);
TreeErr_t ReadNodeData      (char* buffer, int* pos, char** node_data);
TreeErr_t ReadNodeDataStrChr(char* buffer, int* pos, char** node_data);

TreeErr_t TreeWriteData     (const Tree_t* tree);
TreeErr_t TreeReadData      (Tree_t* tree, const char* data_file_path);

//——————————————————————————————————————————————————————————————————————————————————————————

int  ReadFile         (FILE* fp, char** buffer_ptr, const char* file_path);
int  CountSize        (const char* file_path, size_t* size);
int  SkipLetter       (char* buffer, int* i, char letter);
void SkipSpaces       (char* buffer, int* pos);
void MakeDataFilePath (char* data_file_path);

//——————————————————————————————————————————————————————————————————————————————————————————

const int MAX_TIME_LEN      = 64;
const int MAX_FILE_NAME_LEN = 1024;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* DATA_ACCESS_H */
