#include <stdio.h>

int main()
{
    FILE *fp_src = fopen("test2.txt", "r");
    FILE *fp_res = fopen("test_res.txt", "w");

    if (fp_src == NULL)
    {
        printf("попка\n");
        return 1;
    }

    int i = 0;
    int index_str = 1;
    char array[100000] = {};

    while (!feof(fp_src))
    {
        char ch = '\0';
        char ch2 = '\0';

        fscanf(fp_src, "%c", &ch);
        if (!feof(fp_src))
        {
            fscanf(fp_src, "%c", &ch2);
            fseek(fp_src, -1, SEEK_CUR);
        }
        else
        {
            array[i] = ch;
            i++;
            break;
        }
        
        if (ch == '\\' && ch2 == '\n')
        {
            if (i < 100 * index_str)
            {
                while (i < 100 * index_str)
                {
                    array[i] = ' ';
                    i++;                
                }
            }
            index_str++;        
        }
        array[i] = ch;
        i++;
    }

    printf("i = %d\n");

    fwrite(array, sizeof(char), i, fp_res);

    fclose(fp_src);
    fclose(fp_res);
}