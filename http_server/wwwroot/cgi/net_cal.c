/*************************************************************************
	> File Name: net_cal.c
	> Author: weicheng
	> Created Time: Fri 22 Jun 2018 10:46:02 AM CST
 ************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void cal_net(char * buf)
{
    int data1;
    int data2;

    sscanf(buf, "first_data=%d&second_data=%d", &data1, &data2);
    printf("<html>\n");
    printf("<h3> %d + %d = %d</h3>\n", data1, data2, data1+data2);
    printf("<h3> %d - %d = %d</h3>\n", data1, data2, data1-data2);
    printf("<h3> %d * %d = %d</h3>\n", data1, data2, data1*data2);
    printf("<h3> %d / %d = %d</h3>\n", data1, data2, data1/data2);
    printf("<h3> %d %% %d = %d</h3>\n", data1, data2, data1%data2);
    printf("</html>\n");
}


int main()
{
    char method[32];
    char buf[1024];

    if(getenv("METHOD") != NULL)
    {
        strcpy(method, getenv("METHOD"));
        if(strcasecmp(method, "GET") == 0)
            strcpy(buf, getenv("QUERY_STRING"));
        else if(strcasecmp(method, "POST") == 0)
        {
            int content_length = atoi(getenv("CONTENT_LENGTH"));
            int i = 0;
            char c;
            for(; i<content_length; i++)
            {
                read(0, &c, 1);
                buf[i] = c;
            }
            buf[i] = '\0';
        }
        else{
        }

        cal_net(buf);
    }
    return 0;
}
