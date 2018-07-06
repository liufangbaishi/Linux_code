/*************************************************************************
	> File Name: main.c
	> Author: weicheng
	> Created Time: Fri 06 Jul 2018 08:04:24 AM PDT
 ************************************************************************/

#include "music.h"
#include "list.h"

extern node_t *cur;
extern node_t *head;

int main( int argc, char *argv[] ) {
	list_init();
	load_music("/music");
	if ( head->next != head )
		cur = head->next;

	do {
		int choose = menu();
		switch ( choose ) {
		case 1:
			playPause();
			break;
		case 2:
			next();
			break;
		case 3:
			prev();
			break;
		case 4:
			stop();
			break;
		case 0:
			printf("谢谢欣赏\n");
			system("killall -SIGKILL aplay");
			exit(0);
			break;
		default:
			// do nothing
			break;
		}
	} while ( 1 );

	return 0;
}

