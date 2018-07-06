#include "music.h"
#include "list.h"

extern node_t *cur;
extern node_t *head;

int first    = 1;   // 是否第一次播放

enum { STOP, PAUSE, PLAY};
int status = STOP;

void load_music( const char *path ) {
	DIR *pdir = opendir(path);
	if ( pdir == NULL ) perror("opendir"), exit(1);

	struct dirent *p = NULL;
	while ( (p=readdir(pdir)) != NULL ) {
		if ( p->d_name[0] == '.' ) 
				continue;
		list_insert(p->d_name);
	}

	closedir(pdir);
}

int menu( void ) {
	printf("+---------------------------+\n");
	printf(" 1. play/pause\n");
	printf(" 2. next\n");
	printf(" 3. prev\n");
	printf(" 4. stop\n");
	printf(" 0. exit\n");
	printf("+---------------------------+\n");
	list_show();
	
	int choose = 4;
	do {
		printf(" > ");
		scanf("%d", &choose);
		if ( choose>=0 && choose<=4 )
			break;
		printf("choose invalid\n");
		while ( getchar() != '\n' ) ;
	} while ( 1 );

	return choose;
}

void playPause()
{
	if ( first == 1 ) {
		char buf[1024] = {};
		sprintf(buf, "madplay -o wav:- /music/%s 2> /dev/null | aplay 2> /dev/null &", cur->name);
		system(buf);
		first = 0;
		status = PLAY;
	} else {
		if ( status == PLAY ) {
			system("killall -SIGSTOP aplay");
			status = PAUSE;
		} else if ( status == PAUSE ) {
			system("killall -SIGCONT aplay");
			status = PLAY;
		}
	}	
}

void stop()
{
	system("killall -SIGKILL aplay");
	first = 1;
}

void next()
{
	stop();
	cur = cur->next;
	if ( cur == head )
		cur = cur->next;
	playPause();
}

void prev()
{
	stop();
	cur = cur->prev;
	if ( cur == head )
		cur = cur->prev;
	playPause();
}

