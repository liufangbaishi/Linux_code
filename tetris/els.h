/*************************************************************************
	> File Name: els.h
	> Author: weicheng
	> Created Time: Mon 02 Jul 2018 10:34:23 PM PDT
 ************************************************************************/

#ifndef _ELS_H
#define _ELS_H

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "./keyboard/keyboard.h"

#define BC 0 // back color
#define W 10 // back width
#define H 20 // back high
int FC=5; //front color
int background[H][W]={}; //back matrix
int cur = 0; // picture

struct shape { //picture matrix
    int s[5][5];
};
struct shape shape_arr[7] = {
    {0,0,0,0,0, 0,0,1,0,0, 0,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,1,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,1,1,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,1,1,0,0, 0,1,1,0,0, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,0,1,1,0, 0,1,1,0,0, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,0,0,0, 0,1,1,0,0, 0,0,1,1,0, 0,0,0,0,0, 0,0,0,0,0},
    {0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0},
};

struct data {
    int x;
    int y;
};
struct data t = {5,0};

void draw_element(int x, int y, int c); //draw element
void draw_shape(struct data* t, struct shape p, int c); //draw picture
void draw_back(); //draw back
void tetris_timer(struct data* t); // move with time
struct shape turn_90(struct shape p); //rotate 90
int tetris(struct data* t); //move with ctrl
void mclean_line(); // clean line
void set_back(struct data* t, struct shape p); // set back
int can_move(int x, int y, struct shape p); // whether or not move



#endif
