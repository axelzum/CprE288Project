/*
 * music.c
 * File for playing music from the bot
 * Created on: Apr 24, 2019
 * Author: Adam Ford, Harrison Majerus
 */
#include "open_interface.h"
/**
 * Queues the first half of the song "Old Town Road" by Lil Nas X
 *
 * @author: Adam Ford, Harrison Majerus
 *
 * @date: 4/23/19
 */
void music_init1(void){
    int index = 0;
    int notes = 12;
    unsigned char pitch[13] = { 75, 75, 75, 75, 75, 73, 71, 68, 68, 75, 73, 73};
    unsigned char len[13] = { 42, 14, 14, 14, 28, 28, 28, 14, 14, 14, 28, 42};
    oi_loadSong(index, notes, pitch, len);
}
/**
 * Queues the second half of the song "Old Town Road" by Lil Nas X
 *
 * @author: Adam Ford, Harrison Majerus
 *
 * @date: 4/23/19
 */
void music_init2(void){
    int index = 0;
    int notes = 10;
    unsigned char pitch2[13] = { 68, 68, 66, 0, 73, 73, 75, 71, 70, 68};
    unsigned char len2[13] = { 14, 14, 14, 28, 56, 14, 14, 28, 28, 28};
    oi_loadSong(index, notes, pitch2, len2);
}
/**
 * Plays the song
 *
 * @author Adam Ford
 * @param:
 *  index: index of the song to be played
 *
 * @date: 4/23/19
 */
void music_playSong(int index){
    oi_play_song(index);
}
