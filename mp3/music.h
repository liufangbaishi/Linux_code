/*************************************************************************
	> File Name: music.h
	> Author: weicheng
	> Created Time: Tue 03 Jul 2018 08:17:25 AM PDT
 ************************************************************************/

#ifndef _MUSIC_H
#define _MUSIC_H

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


extern int first;
extern int status;

void load_music(const char* path);
int menu();
void playPause();
void stop();
void next();
void prev();

#endif
