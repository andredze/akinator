    FILE* log = fopen("log.dot", "w");

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

    Tree_t tree = {};

    TreeNode_t* left3_1  = NULL;
    TreeNode_t* right3_1 = NULL;

    TreeFullNodeCtor(&tree, &left3_1,  NULL, NULL, 10);
    TreeSingleNodeDump(left3_1,  log);

    TreeFullNodeCtor(&tree, &right3_1, NULL, NULL, 15);
    TreeSingleNodeDump(right3_1, log);

    TreeNode_t* left3_2  = NULL;
    TreeNode_t* right3_2 = NULL;

    TreeFullNodeCtor(&tree, &left3_2,  NULL, NULL, 110);
    TreeSingleNodeDump(left3_2 , log);

    TreeFullNodeCtor(&tree, &right3_2, NULL, NULL, 115);
    TreeSingleNodeDump(right3_2, log);

    TreeNode_t* left2  = NULL;
    TreeNode_t* right2 = NULL;

    TreeFullNodeCtor(&tree, &left2,   left3_1, right3_1, 1010);
    TreeSingleNodeDump(left2 , log);

    TreeFullNodeCtor(&tree, &right2,  left3_2, right3_2, 1015);
    TreeSingleNodeDump(right2, log);

    TreeNode_t* root   = NULL;

    TreeFullNodeCtor(&tree, &root,    left2, right2, 888);
    TreeSingleNodeDump(root , log);

    tree.dummy = (TreeNode_t*) calloc(1, sizeof(TreeNode_t));

    tree.dummy->data  = 0;
    tree.dummy->left  = NULL;
    tree.dummy->right = root;

    TreeSingleNodeDump(tree.dummy, log);

    fprintf(log, "}\n");

    fclose(log);

    if (TreeDump(&tree))
    {
        return EXIT_FAILURE;
    }

    TreeSingleNodeDtor(left3_1 );
    TreeSingleNodeDtor(right3_1);
    TreeSingleNodeDtor(left3_2 );
    TreeSingleNodeDtor(right3_2);
    TreeSingleNodeDtor(left2   );
    TreeSingleNodeDtor(right2  );
    TreeSingleNodeDtor(root    );

    TreeSingleNodeDtor(tree.dummy);

    char command[MAX_COMMAND_LEN] = "";
    snprintf(command, 200, "dot log.dot -T svg -o log.svg");

    system(command);


//------------------------------------------------------------------------------------------


TreeErr_t TreeFullNodeCtor(Tree_t*      tree,
                           TreeNode_t** new_node,
                           TreeNode_t*  left,
                           TreeNode_t*  right,
                           TreeElem_t   data)
{
    assert(tree     != NULL);
    assert(new_node != NULL);

    TreeNode_t* node = (TreeNode_t*) calloc (1, sizeof(TreeNode_t));

    if (node == NULL)
    {
        PRINTERR("Memory allocation for a new node failed");
        return TREE_CALLOC_ERROR;
    }

    node->data  = data;
    node->left  = left;
    node->right = right;

    tree->size++;

    *new_node = node;

    return TREE_SUCCESS;
}

