/*************************************************************************
	> File Name: list.c
	> Author: weicheng
	> Created Time: Fri 06 Jul 2018 07:57:21 AM PDT
 ************************************************************************/

#include "list.h"

node_t *head = NULL;
node_t *cur = NULL; // 指向当前播放的歌

void list_init( void )
{
	head = malloc(sizeof(node_t));
	memset(head, 0x00, sizeof(node_t));

	head->next = head->prev = head;
}

void list_insert( const char *name ) {
	node_t *p = malloc(sizeof(node_t));
	memset(p, 0x00 ,sizeof(node_t));

	p->name = malloc(strlen(name)+1);
	strcpy(p->name, name);

	p->next = head->next;
	p->prev = head;
	head->next->prev = p;
	head->next = p;
}

void list_show( void )
{
	node_t *p = head->next;
	
	while ( p != head ) {
		printf("%s ", p->name);
		if ( p == cur )
			printf(" <==cur");
		printf("\n");
		p = p->next;
	}
}
