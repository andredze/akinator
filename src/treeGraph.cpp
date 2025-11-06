#include "treeGraph.h"

//------------------------------------------------------------------------------------------

TreeErr_t TreeDump(const Tree_t* tree)
{
    assert(tree != NULL); // NOTE: if

    FILE* log = fopen("tree_log.dot", "w");

    if (log == NULL)
    {
        PRINTERR("Failed opening logfile");
        return TREE_DUMP_ERROR;
    }

    fprintf(log, "digraph Tree\n{\n\t"
    R"(graph [splines=ortho];
    ranksep=0.75;
    nodesep=0.5;
    node [
        fontname  = "Arial",
        shape     = "Mrecord",
        style     = "filled",
        color     = "#3E3A22",
        fillcolor = "#E3DFC9",
        fontcolor = "#3E3A22"
    ];)""\n");

    TreeErr_t error = TREE_SUCCESS;

    if ((error = TreeNodeDump(tree->dummy, log)))
    {
        return error;
    }

    fprintf(log, "}\n");

    fclose(log);

    // new func
    char command[MAX_COMMAND_LEN] = "";
    snprintf(command, 200, "dot tree_log.dot -T svg -o tree_log.svg");

    system(command);

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeNodeDump(const TreeNode_t* node, FILE* fp)
{
    assert(node != NULL);

    TreeErr_t error = TREE_SUCCESS;

    printf("(");

    if (node->left != NULL)
    {
        if ((error = TreeNodeDump(node->left, fp)))
        {
            return error;
        }
    }

    printf(" "SPEC " ", node->data);

    if ((error = TreeSingleNodeDump(node, fp)))
    {
        return error;
    }

    if (node->right != NULL)
    {
        if ((error = TreeNodeDump(node->right, fp)))
        {
            return error;
        }
    }

    printf(")");

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

TreeErr_t TreeSingleNodeDump(const TreeNode_t* node, FILE* fp)
{
    if (MakeTreeDefaultNode(node, fp))
    {
        return TREE_DUMP_ERROR;
    }
    if (MakeTreeEdges      (node, fp))
    {
        return TREE_DUMP_ERROR;
    }

    return TREE_SUCCESS;
}

//------------------------------------------------------------------------------------------

int MakeTreeEdges(const TreeNode_t* node, FILE* fp)
{
    assert(fp   != NULL);
    assert(node != NULL);

    if (node->left  == NULL &&
        node->right == NULL)
    {
        return 0;
    }

    char name_node [MAX_NODE_NAME_LEN] = "";
    sprintf (name_node , "node%p", node       );

    if (node->left != NULL)
    {
        char name_left [MAX_NODE_NAME_LEN] = "";
        sprintf (name_left , "node%p", node->left );
        MakeEdge(name_node, name_left , NULL, NULL, NULL, NULL, NULL, NULL, fp);
    }
    if (node->right != NULL)
    {
        char name_right[MAX_NODE_NAME_LEN] = "";
        sprintf (name_right, "node%p", node->right);
        MakeEdge(name_node, name_right, NULL, NULL, NULL, NULL, NULL, NULL, fp);
    }

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeTreeDefaultNode(const TreeNode_t* node, FILE* fp)
{
    assert(fp   != NULL);
    assert(node != NULL);

    char name[MAX_NODE_NAME_LEN] = "";

    sprintf(name, "node%p", node);

    char label[MAX_LABEL_LEN] = "";
    int  current_pos = 0;

    current_pos += sprintf(label, "{ ptr = %p | data = ", node);

    if (node->data == TREE_POISON)
    {
        current_pos += sprintf(current_pos + label, "PZN");
    }
    else
    {
        current_pos += sprintf(current_pos + label, SPEC, node->data);
    }

    sprintf(current_pos + label, " | { left = %p | right = %p }}",
                                 node->left,
                                 node->right);

    MakeNode(name, label, NULL, NULL, NULL, NULL, fp);

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeNode(const char* name,
             const char* label,
             const char* color,
             const char* fillcolor,
             const char* fontcolor,
             const char* shape,
             FILE* fp)
{
    assert(name != NULL);
    assert(fp   != NULL);

    fprintf(fp, "\t%s", name);

    int is_first_arg = 1;

    PrintArg("label",     label,     &is_first_arg, fp);
    PrintArg("color",     color,     &is_first_arg, fp);
    PrintArg("fillcolor", fillcolor, &is_first_arg, fp);
    PrintArg("fontcolor", fontcolor, &is_first_arg, fp);
    PrintArg("shape",     shape,     &is_first_arg, fp);

    if (is_first_arg == 0)
    {
        fprintf(fp, "]");
    }

    fprintf(fp, ";\n");

    return 0;
}

//------------------------------------------------------------------------------------------

int MakeEdge(const char* node1,
             const char* node2,
             const char* color,
             const char* constraint,
             const char* dir,
             const char* style,
             const char* arrowhead,
             const char* arrowtail,
             FILE*       fp)
{
    assert(node1 != NULL);
    assert(node2 != NULL);

    fprintf(fp, "\t%s->%s", node1, node2);

    int is_first_arg = 1;

    PrintArg("color",      color,      &is_first_arg, fp);
    PrintArg("constraint", constraint, &is_first_arg, fp);
    PrintArg("dir",        dir,        &is_first_arg, fp);
    PrintArg("style",      style,      &is_first_arg, fp);
    PrintArg("arrowhead",  arrowhead,  &is_first_arg, fp);
    PrintArg("arrowtail",  arrowtail,  &is_first_arg, fp);

    if (is_first_arg == 0)
    {
        fprintf(fp, "]");
    }

    fprintf(fp, ";\n");

    return 0;
}

//------------------------------------------------------------------------------------------

int PrintArg(const char* arg_name,
             const char* arg_value,
             int*        is_first_arg,
             FILE*       fp)
{
    assert(arg_name     != NULL);
    assert(is_first_arg != NULL);

    if (arg_value != NULL)
    {
        if (*is_first_arg)
        {
            fprintf(fp, " [");
            *is_first_arg = 0;
        }
        else
        {
            fprintf(fp, ", ");
        }

        fprintf(fp, "%s = \"%s\"", arg_name, arg_value);
    }

    return 0;
}

//------------------------------------------------------------------------------------------
