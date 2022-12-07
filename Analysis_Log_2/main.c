#include <stdio.h>
#include <stdlib.h>
#define FILENAME "log.txt"
#define MAXLENGTHFILE 5000

char file_str[MAXLENGTHFILE];

int file_to_str (char* str)
{
    int status;
    FILE *fp = NULL;

    fp = fopen(FILENAME, "r");
    if (fp == NULL)
    {
        printf("File does not exist\n");
        return -1;
    }
    status = fread(str, MAXLENGTHFILE, 1, fp);
    // printf("Noi dung cua file log.txt: \n%s", str);

    fclose(fp);
    fp = NULL;
    return status;
}

int count_messages(char *str, char *phase)
{
    // khoi tao bien dem
    int count = 0;
    char *temp;
    int phase_len = strlen(phase);

    temp = strstr(str, phase);
    while (temp != NULL)
    {
        count++;
        temp = strstr(temp + phase_len, phase);
    }
    return count;
}


int main()
{
    file_to_str(file_str);
    int slbt = count_messages(file_str, "\"cmd\":\"set\"");
    printf("So luong ban tin: %d", slbt);
    return 0;

    // cau 2

    printf("Nhap ma network: \n");
    scanf("%d", )
}
