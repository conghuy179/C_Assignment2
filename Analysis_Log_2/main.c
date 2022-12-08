#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

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

int count_messages(char *str, char *phase) //Ham dem ban tin
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

int count_messages_from_device(char *file_str, char *network_address) //ham trich xuat ban tin tu file log
{

    char info[18][100]; //co 18 info
    int i = 0;

     //Cat chuoi JSON bang dau ","
    char *token = strtok(file_str, "\n");


   // Tiep tuc cat chuoi cho den khi duoc 18 doan gia tri
    while (token != NULL)
    {
        printf("Token = %s\n", token);
        strcpy(info[i], token);
        printf("Info[%d] = %s\n", i, info[i]);
        i++;
        token = strtok(NULL, "\n");
    }

    // khai bao cac bien can thiet

    char command_phase[] = "\"cmd\":\"set\"";
    int message_count = 0;
    char *temp1, *temp2;
    int info_len = strlen(info);

     // dem command phase va network address trong info
     // neu tim thay ca 2, count++

    temp1 = strstr(info,command_phase);
    temp2 = strstr(info, network_address);

    if (temp1 != NULL && temp2 != NULL)
    {
        message_count++;
    }

    token = strtok(NULL, "\n");

    return message_count;

}

int main()
{
    // Cau 2
    char network_address[100];
    printf("Nhap dia chi network: \n");
    scanf("%d", network_address);
    int sent_count = count_messages_from_device(file_str,network_address);
    printf("So ban tin gui di: %d", sent_count);

    // Cau 1:
    file_to_str(file_str);
    int slbt = count_messages(file_str, "\"cmd\":\"set\"");
    printf("So luong ban tin: %d\n", slbt);

    return 0;
}
