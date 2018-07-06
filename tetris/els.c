/*************************************************************************
	> File Name: els.c
	> Author: weicheng
	> Created Time: Mon 02 Jul 2018 05:42:31 PM PDT
 ************************************************************************/

#include "els.h"

void draw_element(int x, int y, int c)
{
    x *= 2;
    x++;
    y++;

    printf("\033[?25l");
    printf("\033[%d;%dH", y, x);
    printf("\033[3%dm\033[4%dm", c, c);
    printf("[]");
    fflush(stdout);
    printf("\033[0m");
}

void draw_shape(struct data* t, struct shape p, int c)
{
    int i;
    int j;
    for(i=0; i<5; i++) {
        for(j=0; j<5; j++){
            if(p.s[i][j] != 0) {
                draw_element(t->x+j, t->y+i, c);  //x->col   y->row
            }
        }
    }
}

void mclean_line()
{
    int i, j;
    for(i=0; i<H; i++) {
        int total = 0;
        for(j=0; j<W; j++) {
            if(background[i][j] != 0)
                total++;
        }
        if(total == W) {
            int k=0;
            for(k=i; k>0; k--) {
                memcpy(background[k], background[k-1], sizeof(background[k]));
            }
            memset(background[0], 0x00, sizeof(background[0]));
        }
    }
}

void draw_back()
{
    int i, j;
    for(i=0; i<H; i++) {
        for(j=0; j<W; j++) {
            if(background[i][j] == 0)
                draw_element(j, i, BC);
            else
                draw_element(j, i, background[i][j]);
        }
    }
}

void set_back(struct data* t, struct shape p)
{
    int i, j;
    for(i=0; i<5; i++) {
        for(j=0; j<5; j++) {
            if(p.s[i][j] != 0) {
                background[t->y+i][t->x+j] = FC;
            }
        }
    }
}

int can_move(int x, int y, struct shape p)
{
    int i, j;
    for(i=0; i<5; i++) {
        for(j=0; j<5; j++) {
            if(p.s[i][j] == 0)
                continue;
            if(x+j >= W)
                return 0;
            if(y+i >= H)
                return 0;
            if(x+j < 0)
                return 0;
            if(background[y+i][x+j] != 0)
                return 0;
        }
    }
    return 1;
}

void tetris_timer(struct data* t)
{
    int i;
    draw_shape(t, shape_arr[cur], BC);
    if(can_move(t->x, t->y+1, shape_arr[cur])) 
        t->y++;
    else {
        set_back(t, shape_arr[cur]);
        mclean_line();
        draw_back();
        do{
            FC = rand()%7;
        }while(FC == BC);
        t->y = 0;
        t->x = 0;
        cur = rand()%7;
        for(i=0; i<W; i++) {
            if(background[2][i] != 0) {
                printf("fail\n");
                printf("\033[?25h");
                recover_keyboard();
                exit(0);
            }
        }
    }
    draw_shape(t, shape_arr[cur], FC);
}

struct shape turn_90(struct shape p)
{
    struct shape t;
    int i, j;
    for(i=0; i<5; i++){
        for(j=0; j<5;j++) {
            t.s[i][j] = p.s[4-j][i];
        }
    }
    return t;
}

int tetris(struct data* t)
{
    int ret = 0;
    int c = get_key();
    if(is_up(c)){
        draw_shape(t, shape_arr[cur], BC);
        shape_arr[cur] = turn_90(shape_arr[cur]);
        if(can_move(t->x, t->y, shape_arr[cur]) == 0) {
            shape_arr[cur] = turn_90(shape_arr[cur]);
            shape_arr[cur] = turn_90(shape_arr[cur]);
            shape_arr[cur] = turn_90(shape_arr[cur]);
        }
        draw_shape(t, shape_arr[cur], FC);
    }
    else if(is_left(c)) {
        draw_shape(t, shape_arr[cur], BC);
        if(can_move(t->x-1, t->y, shape_arr[cur]))
            t->x--;
        draw_shape(t, shape_arr[cur], FC);
    }else if(is_right(c)) {
        draw_shape(t, shape_arr[cur], BC);
        if(can_move(t->x+1, t->y, shape_arr[cur]))
            t->x++;
        draw_shape(t, shape_arr[cur], FC);
    }else if(is_down(c)){
        draw_shape(t, shape_arr[cur], BC);
        if(can_move(t->x, t->y+1, shape_arr[cur]))
            t->y++;
        draw_shape(t, shape_arr[cur], FC);
    }
    else if(is_esc(c)) {
        ret = 1;
    }

    return ret;
}

void handler(int s)
{
    if(s ==SIGALRM)
        tetris_timer(&t);
    else if(s == SIGINT) {
        printf("\033[0m");
        printf("\033[?25h");
        recover_keyboard();
        exit(0);
    }
}

int main()
{
    srand(getpid());
    init_keyboard();
    draw_back();

    struct sigaction act;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);
    sigaction(SIGINT, &act, NULL);
    
    struct itimerval it;
    it.it_value.tv_sec = 0;
    it.it_value.tv_usec = 1;
    it.it_interval.tv_sec = 1;
    it.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &it, NULL);

    while(1) {
        //tetris_timer(&t);
        if(tetris(&t) == 1) {
            break;
        }
        //sleep(1);
    }
    //for(i=0; i<6; i++){
    //    //draw_element(i, 10, FC);
    //    //sleep(1);
    //    //draw_element(i, 10, BC);
    //    draw_shape(5, i, shape_arr[0], FC);
    //    sleep(1);
    //    draw_shape(5, i, shape_arr[0], BC);
    //}
    printf("\033[?25h");
    recover_keyboard();
    return 0;
}
