#include <TXLib.h>
#include <stdio.h>

int main()
{
    printf("Введите текст: ");

    char text[100] = {};

    scanf("%s", text);

    printf("Ваш текст: %s\n", text);

    return 0;
}
