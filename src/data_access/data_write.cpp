#include "data_access.h"

//------------------------------------------------------------------------------------------

TreeErr_t TreeWriteData(const Tree_t* tree)
{
    DEBUG_TREE_CHECK(tree, "ERROR BEFORE TREE WRITE DATA");

    char data_file_path[MAX_FILE_NAME_LEN] = "";
    MakeDataFilePath(data_file_path);

    FILE* fp = fopen(data_file_path, "w");

    if (fp == NULL)
    {
        PRINTERR("Error with opening file: %s", data_file_path);
        return TREE_FILE_ERR;
    }

    WriteNode(tree->dummy->right, fp);

    fclose(fp);

    printf("База данных записана в файл: %s\n", data_file_path);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

void MakeDataFilePath(char* data_file_path)
{
    time_t rawtime = time(NULL);

    struct tm* info = localtime(&rawtime);

    char time[MAX_TIME_LEN] = "";

    strftime(time, sizeof(time), "data_%d-%b-%Y_%H-%M-%S", info);

    snprintf(data_file_path, MAX_FILE_NAME_LEN, "data/%s.txt", time);
}

//------------------------------------------------------------------------------------------

void WriteNode(const TreeNode_t* node, FILE* fp)
{
    assert(fp != NULL);

    if (node == NULL)
    {
        fprintf(fp, "nil ");
        return;
    }

    fprintf(fp, "( \"%s\" ", node->data);

    WriteNode(node->left, fp);

    WriteNode(node->right, fp);

    fprintf(fp, " )");
}

//------------------------------------------------------------------------------------------
