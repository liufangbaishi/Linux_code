/*************************************************************************
	> File Name: delete_data.c
	> Author: weicheng
	> Created Time: Fri 06 Jul 2018 06:28:59 AM PDT
 ************************************************************************/

#include <stdio.h>
#include <mysql.h>
#include <string.h>

void delete_data(char *name)
{
    MYSQL* fd = mysql_init(NULL);
    if(mysql_real_connect(fd, "127.0.0.1", "root", "", "db_http", 3306, NULL, 0) == NULL) {
        printf("connect fail\n");
        return;
    }
    printf("<http>\n");
    printf("connect success\n");
    char sql[1024];
    sprintf(sql, "delete from stu where name='%s'", name);
    printf("%s\n", sql);
    mysql_query(fd, sql);
    printf("delete success\n");
    printf("</http>\n");
    mysql_close(fd);
}

int main()
{
    char method[24];
    char buff[1024];
    if(getenv("METHOD") != NULL) {
        strcpy(method, getenv("METHOD"));
        if(strcasecmp(method, "GET") == 0)
            strcpy(buff, getenv("QUERY_STRING"));
        else {
            int content_length = atoi(getenv("CONTENT_LENGTH"));
            int i=0;
            char c;
            for(; i<content_length; i++) {
                read(0, &c, 1);
                buff[i]=c;
            }
            buff[i]='\0';
        }

        char name[10];
        strtok(buff, "=");
        strcpy(name, strtok(NULL, "="));
        delete_data(name);
    }
    return 0;
}
