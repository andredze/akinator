#include "akinator.h"
#include <locale.h>
#include <langinfo.h>

//------------------------------------------------------------------------------------------

/* TODO:
        определение запоминать путь,
        сравнение,

        чтение + менять " на \0 + пробелы допустимы + дамп с серым, красным, синим + оптимизировать алгоритм
        указатели на буфер вместо strdup

        запись в файл с пробелами,
        проверка на отрицание в ответе пользователя
        
        флаги пофиксить

        узнать про locale, langinfo
        mkdir, clock_t, trdsc, как меняется кодировка, как происходит запись букв в UTF-8 и как их сравнивать, выводить в консоль

        sscanf с %* и strchr
        свой принтф со спецификатором %#640000 и vsprintf
        гугл тесты для сорт. дерева

        на 9 озвучка сообщений TXspeak
        на 10 анимация Txlib
*/

//         TreeNode_t* sigma = NULL;
//         TreeNode_t* delta = NULL;
//         TreeNode_t* woman = NULL;
//         TreeNode_t* sanek = NULL;
//         TreeNode_t* angl = NULL;
//         TreeNode_t* anton = NULL;
//         TreeNode_t* treki = NULL;
//         TreeNode_t* makson = NULL;
//         TreeNode_t* baizit = NULL;
//         TreeNode_t* diskra = NULL;
//         TreeNode_t* bbb = NULL;
//
//         if (AkinatorNodeCtor(&tree, "сигма",                 &sigma))  break;
//         if (AkinatorNodeCtor(&tree, "дельта",                &delta))  break;
//         if (AkinatorNodeCtor(&tree, "признает женскую расу", &woman))  break;
//         if (AkinatorNodeCtor(&tree, "Санек",                 &sanek))  break;
//         if (AkinatorNodeCtor(&tree, "завозит на английском", &angl))   break;
//         if (AkinatorNodeCtor(&tree, "Антон лега",            &anton))  break;
//         if (AkinatorNodeCtor(&tree, "делает треки",          &treki))  break;
//         if (AkinatorNodeCtor(&tree, "Макс",                  &makson)) break;
//         if (AkinatorNodeCtor(&tree, "Баязит музик",          &baizit)) break;
//         if (AkinatorNodeCtor(&tree, "шарит за дискру",       &diskra)) break;
//         if (AkinatorNodeCtor(&tree, "Босс Борис Бритва",     &bbb))    break;
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

//------------------------------------------------------------------------------------------

int main()
{
    // if (setlocale(LC_ALL, "C.UTF-8") == NULL)
    // {
    //     printf("Error with setting UTF-8\n");
    //     return EXIT_FAILURE;
    // }
    // printf("Current codeset: %s\n", setlocale(LC_ALL, NULL));

    Tree_t tree = {};

    do {
        DPRINTF("Program start\n");

        if (TreeCtor(&tree))
            break;

        if (AkinatorReadData(&tree, "data/11112025_120911.txt")) // 11112025_154608.txt
            break;

        if (RunAkinator(&tree))
            break;

        if (AkinatorWriteData(&tree))
            break;

        // if (AkinatorDescribeWord(&tree, "Босс Борис"))
        //     break;

    } while (0);

    TreeDtor(&tree);

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------------------
