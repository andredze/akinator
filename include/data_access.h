#ifndef DATA_ACCESS_H
#define DATA_ACCESS_H

//——————————————————————————————————————————————————————————————————————————————————————————

#include "tree_commands.h"
#include <ctype.h>

//——————————————————————————————————————————————————————————————————————————————————————————

void      WriteNode         (const TreeNode_t* node, FILE* fp, int rank);

TreeErr_t ReadNode          (Tree_t* tree, TreeNode_t** node, char* buffer, long int* pos);
TreeErr_t ReadNodeData      (char* buffer, long int* pos, char** node_data);
TreeErr_t ReadNodeDataStrChr(char* buffer, long int* pos, char** node_data);

TreeErr_t TreeWriteData     (const Tree_t* tree);
TreeErr_t TreeReadData      (Tree_t* tree, const char* data_file_path);

//——————————————————————————————————————————————————————————————————————————————————————————

int  ReadFile         (FILE* fp, char** buffer_ptr, const char* file_path);
int  CountSize        (const char* file_path, size_t* size);
int  SkipLetter       (char* buffer, long int* i, char letter);
void SkipSpaces       (char* buffer, long int* pos);
void MakeDataFilePath (char* data_file_path);

//——————————————————————————————————————————————————————————————————————————————————————————

const int MAX_TIME_LEN      = 64;
const int MAX_FILE_NAME_LEN = 1024;

//——————————————————————————————————————————————————————————————————————————————————————————

#endif /* DATA_ACCESS_H */
