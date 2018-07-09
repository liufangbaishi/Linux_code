/*************************************************************************
	> File Name: test.c
	> Author: weicheng
	> Created Time: Thu 05 Jul 2018 01:49:58 AM PDT
 ************************************************************************/

#include <stdio.h>
#include <mysql.h>

void test()
{
    MYSQL* fd = mysql_init(NULL);
    if(mysql_real_connect(fd, "127.0.0.1", "root", "", "db_http", 3306, NULL, 0) == NULL) {
        printf("connect\n");
        return;
    }
    printf("connect successful\n");
    const char* sql = "insert into stu(name, sex, age) values(\"wei\", \"male\", 20)";
    mysql_query(fd, sql);
    printf("mysql client version:%s\n", mysql_get_client_info());
    mysql_close(fd);
}
int main()
{
    test();
    return 0;
}
