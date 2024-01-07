#include <stdio.h>

int main()
{
    FILE *fp = fopen("program-txt/src/ram_test.txt", "r");
    char str[100] = {};
    int num = 0;

    fscanf(fp, "pop[%d]", &num);

    // printf("%s\n", str);
    printf(">>%d\n", num);
}