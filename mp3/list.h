/*************************************************************************
	> File Name: list.h
	> Author: weicheng
	> Created Time: Fri 06 Jul 2018 07:55:32 AM PDT
 ************************************************************************/

#ifndef _LIST_H
#define _LIST_H

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

typedef struct node {
	char *name; // 歌曲名字
	struct node *prev;
	struct node *next;
}node_t;

extern node_t *head;
extern node_t *cur; // 指向当前播放的歌

void list_init();
void list_insert(const char *name);
void list_show();

#endif
