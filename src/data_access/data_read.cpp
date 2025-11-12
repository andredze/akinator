#include "data_access.h"

//------------------------------------------------------------------------------------------

TreeErr_t TreeReadData(Tree_t* tree, const char* data_file_path)
{
    DEBUG_TREE_CHECK(tree, "ERROR BEFORE TREE READ DATA");

    assert(data_file_path != NULL);

    DPRINTF("Reading file %s\n", data_file_path);

    FILE* fp = fopen(data_file_path, "r");

    if (fp == NULL)
    {
        PRINTERR("Error with opening file: %s", data_file_path);
        return TREE_FILE_ERR;
    }

    char* buffer = NULL;

    if (ReadFile(fp, &buffer, data_file_path))
    {
        return TREE_FILE_ERR;
    }

    TreeErr_t err = TREE_SUCCESS;
    long int i = 0;

    if ((err = ReadNode(tree, &tree->dummy->right, buffer, &i)))
    {
        return err;
    }

    tree->buffer = buffer;

    DEBUG_TREE_CHECK(tree, "ERROR AFTER TREE READ DATA");
    TREE_CALL_DUMP  (tree, "DUMP AFTER TREE READ DATA");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t ReadNode(Tree_t* tree, TreeNode_t** node, char* buffer, long int* pos)
{
    assert(buffer != NULL);
    assert(tree   != NULL);
    assert(node   != NULL);

    TreeErr_t error = TREE_SUCCESS;
    char first_char = buffer[*pos];

    if (first_char == '(')
    {
        (*pos)++;
        SkipSpaces(buffer, pos);
        TreeReadBufferDump(buffer, *pos, "BUFFER DUMP SKIPPING OPENING BRACKET");

        char* data = NULL;

        if ((error = ReadNodeDataStrChr(buffer, pos, &data)))
            return error;

        TreeReadBufferDump(buffer, *pos, "BUFFER DUMP AFTER READING %s", data);

        if ((error = TreeNodeCtor(tree, data, node)))
            return error;

        TREE_CALL_DUMP(tree, "DUMP AFTER NODE CTOR %s", data);

        if ((error = ReadNode(tree, &(*node)->left,  buffer, pos)))
            return error;

        TreeReadBufferDump(buffer, *pos, "BUFFER DUMP AFTER READING NODE LEFT TO %s", data);

        if ((error = ReadNode(tree, &(*node)->right, buffer, pos)))
            return error;

        TreeReadBufferDump(buffer, *pos, "BUFFER DUMP AFTER READING NODE RIGHT TO %s", data);

        if (SkipLetter(buffer, pos, ')'))
            return TREE_INVALID_INPUT;

        SkipSpaces(buffer, pos);

        TreeReadBufferDump(buffer, *pos, "BUFFER DUMP AFTER SKIPPING CLOSING BRACKET after %s", data);
    }
    else if (strncmp(&buffer[*pos], "nil", 3) == 0)
    {
        (*pos) += 3;
        SkipSpaces(buffer, pos);
        TreeReadBufferDump(buffer, *pos, "BUFFER DUMP AFTER READING NULL");
    }
    else
    {
        PRINTERR("Syntax error in tree data (unknown symbol = \"%c\" )\n", first_char);
        return TREE_INVALID_INPUT;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t ReadNodeData(char* buffer, long int* pos, char** node_data)
{
    assert(node_data != NULL);
    assert(buffer    != NULL);
    assert(pos       != NULL);

    int data_len = 0;

    if (sscanf(&buffer[*pos], "\"%*[^\"]\"%n", &data_len) != 0)
    {
        PRINTERR("Error with reading data");
        return TREE_FILE_ERR;
    }

    *node_data = buffer + *pos + 1; /* node_data points to the start of the word in buffer */

    (*pos) += data_len; /* moving pos to the next word */

    buffer[*pos - 1] = '\0'; /* setting null-terminator for node_data (instead of closing quote) */

    SkipSpaces(buffer, pos);

    DPRINTF("   word = %s;\n", *node_data);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t ReadNodeDataStrChr(char* buffer, long int* pos, char** node_data)
{
    assert(node_data != NULL);
    assert(buffer    != NULL);
    assert(pos       != NULL);

    if (buffer[*pos] != '"')
    {
        PRINTERR("Error with reading data");
        return TREE_FILE_ERR;
    }

    (*pos)++; /* skip opening quote (") */

    char* curptr = NULL;

    if ((curptr = strchr(buffer + *pos, '"')) == NULL)
    {
        PRINTERR("Error with reading data");
        return TREE_FILE_ERR;
    }

    *node_data = buffer + *pos; /* node_data points to the start of the word in buffer */

    *curptr = '\0'; /* setting null-terminator for node_data (instead of closing quote) */

    /* moving pos to the next word */
    (*pos) += curptr - *node_data + 1;
    SkipSpaces(buffer, pos);

    DPRINTF("   word = %s;\n", *node_data);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

void SkipSpaces(char* buffer, long int* pos)
{
    assert(buffer != NULL);
    assert(pos    != NULL);

    char ch = '\0';

    while ((ch = buffer[*pos]) != '\0' && isspace(ch))
    {
        (*pos)++;
    }
}

//------------------------------------------------------------------------------------------

int SkipLetter(char* buffer, long int* pos, char letter)
{
    assert(buffer != NULL);
    assert(pos    != NULL);

    char current_char = buffer[*pos];

    if (current_char != letter)
    {
        PRINTERR("Syntax error: expected %c, got %c (%d)", letter, current_char, current_char);
        return 1;
    }
    (*pos)++;

    return 0;
}

//------------------------------------------------------------------------------------------

int ReadFile(FILE* fp, char** buffer_ptr, const char* file_path)
{
    assert(fp         != NULL);
    assert(file_path  != NULL);
    assert(buffer_ptr != NULL);

    size_t size = 0;

    if (CountSize(file_path, &size))
    {
        return 1;
    }

    char* buffer = (char*) calloc(size, 1);

    if (buffer == NULL)
    {
        PRINTERR("Memory allocation failed");
        return 1;
    }

    if (fread(buffer, size - 1, 1, fp) != 1)
    {
        PRINTERR("Reading file error");
        return 1;
    }

    buffer[size - 1] = '\0'; /* set null-term */

    *buffer_ptr = buffer;

    return 0;
}

//------------------------------------------------------------------------------------------

int CountSize(const char* file_path, size_t* size)
{
    struct stat fileinfo = {};

    if (stat(file_path, &fileinfo) == -1)
    {
        PRINTERR("Error with stat()");
        return 1;
    }

    *size = (size_t) fileinfo.st_size + 1;

    return 0;
}

//------------------------------------------------------------------------------------------
