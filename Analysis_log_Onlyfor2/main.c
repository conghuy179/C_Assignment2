#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define FILENAME "log.txt"
#define MAXLENGTHFILE 5000
#define NUM_FILE_LINES 18
#define MAX_LINE_LENGTH 300

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

/*
    TODO: Sua comment theo format de bai
    Ham slice_str cat cac ki tu tu vi tri start den vi tri end
    (khong bao gom ki tu tai vi tri end) trong xau string
    va luu ket qua vao out.
*/
void slice_str(const char *str, char *out, int start, int end)

{
    int j = 0;
    for (int i = start; i < end; ++i)
    {
        out[j] = str[i];
        j++;
    }
    out[j] = '\0';
}

/*
    TODO: Sua comment theo format de bai
    Ham dem ban tin
*/
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

/*
    TODO: Sua comment theo format de bai
    ham dem so ban tin tu file log
*/
int count_messages_from_device(char info[NUM_FILE_LINES][MAX_LINE_LENGTH], char *network_address)
{
    // khai bao cac bien can thiet

    char command_phase[] = "\"cmd\":\"set\"";
    int message_count = 0;

     // dem command phase va network address trong info
     // neu tim thay ca 2, count++

    for (int j = 0; j < NUM_FILE_LINES; j++)
    {
        if(strstr(info[j],command_phase) != NULL && strstr(info[j], network_address) != NULL)
        {
            // printf("%s\n", info[j]);
            message_count++;
        }
    }

    return message_count;
}

/*
    TODO: Sua comment theo format de bai
    Ham count_network_endpoint
    ham tinh so luong va dia chi cong tac da trao doi
    thong tin voi do dieu khien trung tam
*/
void count_network_endpoint(char info[NUM_FILE_LINES][MAX_LINE_LENGTH])
{
    // Step1: khoi tao mang luu danh sach dia chi network
    char network_address_arr[NUM_FILE_LINES][5];
    //  Mang luu danh sach dia chi endpoint
    char endpoint_address_arr[NUM_FILE_LINES][2];

    // So luong cap network, endpoint hien tai
    int num_devices = 0;

    // Step 3: kiem tra ban tin co chua "type":"switch"" khong
    char type_switch[] = "\"type\":\"switch\"";
    // khai bao pattern de bat dau vi tri tim network va endpoint
    char net_pattern[] = "\"data\":\[\"";
    int len_net_pattern = strlen(net_pattern);
    int zwave_len = strlen("zwave-");
    int address_len = 4;

    // tao 2 bien de luu tam thoi 2 dia chi network va endpoint
    char tmp_network_address[5];
    char tmp_endpoint_address[2];

    for (int j = 0; j < NUM_FILE_LINES; j++)
    {
        if (strstr(info[j], type_switch) != NULL)
        {
            char *data = strstr(info[j], net_pattern);
            if (data != NULL)
            {
                slice_str(data,tmp_network_address,len_net_pattern + zwave_len, len_net_pattern + zwave_len + 4);
                slice_str(data,tmp_endpoint_address,len_net_pattern + zwave_len + 7, len_net_pattern + zwave_len + 8);
                // printf("Temp network: %s\n",tmp_network_address);
                // printf("Temp endpoint: %s\n",tmp_endpoint_address);

                bool found = false;
                for (int k = 0; k < num_devices; k++)
                {
                    if (strcmp(tmp_network_address,network_address_arr[k]) == 0 && strcmp(tmp_endpoint_address, endpoint_address_arr[k]) == 0)
                    {
                        found = true;
                        break;
                    }
                }
                // neu ko tim thay => them vao
                if (!found)
                {
                    strcpy(network_address_arr[num_devices], tmp_network_address);
                    strcpy(endpoint_address_arr[num_devices], tmp_endpoint_address);
                    num_devices++;
                }
            }
        }
    }
    for (int i = 0; i <num_devices; i++)
    {
        printf("thiet bi %d co dia chi la: NWK - %s, ENDPOINT - %s\n", i+1, network_address_arr[i], endpoint_address_arr[i]);
    }
}
/*
    TODO:
    Ham ....
*/
void count_error_messages(char info[NUM_FILE_LINES][MAX_LINE_LENGTH])
{
    // Khoi tao so luong ban tin loi = 0
    int error_num = 0;

    //Step 3: Duyet tung cap ban tin
    //3.1. tim vi tri xau "reqid" trong ban tin
    char reqid_phase[] = "\"reqid\"";
    int reqid_phase_len = strlen(reqid_phase);
    char current_reqid[5];
    char next_reqid[5];

    for (int i = 0; i < NUM_FILE_LINES; i += 2)
    {
        // lay reqid hien tai
        char *data = strstr(info[i], reqid_phase);
        if (data != NULL)
        {
            slice_str(data,current_reqid,reqid_phase_len + 3,reqid_phase_len + 7);
        }

          // lay reqid tiep theo
        data = strstr(info[i + 1], reqid_phase);
        if (data != NULL)
        {
            slice_str(data,next_reqid,reqid_phase_len + 3,reqid_phase_len + 7);

        }

          // so sanh reqid hien tai voi tiep theo xem ban tin co loi khong
        if (strcmp(current_reqid, next_reqid) != 0)
        {
            error_num++;
        }
        // printf("Current = %s. Next = %s\n", current_reqid, next_reqid);
    }
    printf("So ban tin loi: %d\n", error_num);
}

/*
TODO:
Ham tinh do tre lon nhat
*/
void calculate_max_delay(char info[NUM_FILE_LINES][MAX_LINE_LENGTH])
{
    int max_delay = 0;
    char reqid_phase[] = "\"reqid\"";
    int reqid_phase_len = strlen(reqid_phase);
    char current_reqid[5];
    char next_reqid[5];

    char time_str[30];

    int current_millis;
    int next_millis;
    int yyyy, mm, ddhh, MM, ss, SSS;

    for (int i = 0; i < NUM_FILE_LINES; i += 2)
    {
        char *data = strstr(info[i], reqid_phase);
        if (data != NULL)
        {
            slice_str(data,current_reqid,reqid_phase_len + 3,reqid_phase_len + 7);
        }
        // tach thoi gian va lay gia tri thoi gian theo millis
        // khong luu tru cac gia tri khac vi giong nhau
        slice_str(info[i], time_str, 7, 29);
        sscanf(time_str, "%d-%d-%d:%d:%d.%d", &yyyy, &mm, &ddhh, &MM, &ss, &SSS);
        current_millis = SSS + ss * 1000 + MM * 60 * 1000;

          // lay reqid tiep theo
        data = strstr(info[i + 1], reqid_phase);
        if (data != NULL)
        {
            slice_str(data,next_reqid,reqid_phase_len + 3,reqid_phase_len + 7);
        }
        slice_str(info[i + 1], time_str, 7, 29);
        sscanf(time_str, "%d-%d-%d:%d:%d.%d", &yyyy, &mm, &ddhh, &MM, &ss, &SSS);
        next_millis = SSS + ss * 1000 + MM * 60 * 1000;

          // so sanh reqid hien tai voi tiep theo xem ban tin co loi khong
        if (strcmp(current_reqid, next_reqid) == 0)
        {
            int delay = next_millis - current_millis;
            if (delay > max_delay)
            {
                max_delay = delay;
            }
        }
    }
    printf("Do tre lon nhat la: %d Millisecond\n", max_delay);
}
/*
TODO
*/
void calculate_average_delay(char info[NUM_FILE_LINES][MAX_LINE_LENGTH])
{
    int total_delay = 0;
    int num_reqs = 0;

    char reqid_phase[] = "\"reqid\"";
    int reqid_phase_len = strlen(reqid_phase);
    char current_reqid[5];
    char next_reqid[5];

    char time_str[30];

    int current_millis;
    int next_millis;
    int yyyy, mm, ddhh, MM, ss, SSS;

    for (int i = 0; i < NUM_FILE_LINES; i += 2)
    {
        char *data = strstr(info[i], reqid_phase);
        if (data != NULL)
        {
            slice_str(data,current_reqid,reqid_phase_len + 3,reqid_phase_len + 7);
        }
        // tach thoi gian va lay gia tri thoi gian theo millis
        // khong luu tru cac gia tri khac vi giong nhau
        slice_str(info[i], time_str, 7, 29);
        sscanf(time_str, "%d-%d-%d:%d:%d.%d", &yyyy, &mm, &ddhh, &MM, &ss, &SSS);
        current_millis = SSS + ss * 1000 + MM * 60 * 1000;

          // lay reqid tiep theo
        data = strstr(info[i + 1], reqid_phase);
        if (data != NULL)
        {
            slice_str(data,next_reqid,reqid_phase_len + 3,reqid_phase_len + 7);
        }
        slice_str(info[i + 1], time_str, 7, 29);
        sscanf(time_str, "%d-%d-%d:%d:%d.%d", &yyyy, &mm, &ddhh, &MM, &ss, &SSS);
        next_millis = SSS + ss * 1000 + MM * 60 * 1000;

          // so sanh reqid hien tai voi tiep theo xem ban tin co loi khong
        if (strcmp(current_reqid, next_reqid) == 0)
        {
            int delay = next_millis - current_millis;
            total_delay += delay;
            num_reqs++;
        }
    }
    printf("Do tre trung binh la: %d Millisecond\n", total_delay / num_reqs);
}

int main()
{


    // Cau 1:
    file_to_str(file_str);
    int slbt = count_messages(file_str, "\"cmd\":\"set\"");
    printf("So luong ban tin: %d\n", slbt);

    // Tokenize file log thanh cac dong, moi dong luu 1 phan tu cua info
    char info[18][300]; //co 18 info
    int i = 0;
    char *token = strtok(file_str, "\n");

    // Tiep tuc cat chuoi cho den khi duoc 18 doan gia tri
    while (token != NULL)
    {
        // printf("Token = %s\n", token);
        // int token_len = strlen(token);
        //  printf("Do dai cua Token = %d\n", token_len);
        strcpy(info[i], token);
        // printf("Info[%d] = %s\n", i, info[i]);
        i++;
        token = strtok(NULL, "\n");
    }

    // Cau 2
    char network_address[100];
    printf("Nhap dia chi network: \n");
    scanf("%s", network_address);
    int sent_count = count_messages_from_device(info,network_address);
    printf("So ban tin gui di: %d\n", sent_count);

    // Cau 3
    count_network_endpoint(info);

    // Cau 4
    count_error_messages(info);

    // Cau 5
    calculate_max_delay(info);

    // Cau 6
    calculate_average_delay(info);

    return 0;
}
