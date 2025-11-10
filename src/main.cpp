#include "akinator.h"
#include <locale.h>
#include <langinfo.h>

//------------------------------------------------------------------------------------------

/* TODO: определение,
         сравнение,
         чтение +
         запись в файл,
         проверка на отрицание в ответе пользователя
*/

//------------------------------------------------------------------------------------------

int main()
{
    if (setlocale(LC_ALL, "C.UTF-8") == NULL)
    {
        printf("Error with setting UTF-8\n");
        return EXIT_FAILURE;
    }

    char* codeset = nl_langinfo(CODESET);
    printf("Текущая кодировка: %s\n", codeset);

    Tree_t tree = {};

    do {
        if (TreeCtor(&tree))
            break;

//         TreeNode_t* sigma  = NULL;
//         TreeNode_t* delta  = NULL;
//         TreeNode_t* woman  = NULL;
//         TreeNode_t* sanek  = NULL;
//         TreeNode_t* angl   = NULL;
//         TreeNode_t* anton  = NULL;
//         TreeNode_t* treki  = NULL;
//         TreeNode_t* makson = NULL;
//         TreeNode_t* baizit = NULL;
//         TreeNode_t* diskra = NULL;
//         TreeNode_t* bbb    = NULL;
//
//         if (TreeNodeCtor(&tree, "сигма",                 &sigma))  break;
//         if (TreeNodeCtor(&tree, "дельта",                &delta))  break;
//         if (TreeNodeCtor(&tree, "признает женскую расу", &woman))  break;
//         if (TreeNodeCtor(&tree, "Санек",                 &sanek))  break;
//         if (TreeNodeCtor(&tree, "завозит на английском", &angl))   break;
//         if (TreeNodeCtor(&tree, "Антон лега",            &anton))  break;
//         if (TreeNodeCtor(&tree, "делает треки",          &treki))  break;
//         if (TreeNodeCtor(&tree, "Макс",                  &makson)) break;
//         if (TreeNodeCtor(&tree, "Баязит музик",          &baizit)) break;
//         if (TreeNodeCtor(&tree, "шарит за дискру",       &diskra)) break;
//         if (TreeNodeCtor(&tree, "Босс Борис Бритва",     &bbb))    break;
//
//         tree.dummy->right = sigma;
//
//         sigma->right = delta;
//         sigma->left  = woman;
//
//         woman->right = sanek;
//         woman->left  = angl;
//
//         angl->left = anton;
//         angl->right = treki;
//
//         treki->left = diskra;
//         treki->right = makson;
//
//         diskra->left = bbb;
//         diskra->right = baizit;

        if (RunAkinator(&tree))
            break;

        // if (AkinatorAddWord(&tree, anton))
            // break;

    } while (0);

    TreeDtor(&tree);

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------------------
