/*
 * music.c
 * File for playing music from the bot
 * Created on: Apr 24, 2019
 * Author: Adam Ford, Harrison Majerus
 */

#ifndef MUSIC_H_
#define MUSIC_H_
/**
 * Queues the first half of the song "Old Town Road" by Lil Nas X
 *
 * @author: Adam Ford, Harrison Majerus
 *
 * @date: 4/23/19
 */
void music_init1(void);
/**
 * Queues the second half of the song "Old Town Road" by Lil Nas X
 *
 * @author: Adam Ford, Harrison Majerus
 *
 * @date: 4/23/19
 */
void music_init2(void);
/**
 * Plays the song
 *
 * @author Adam Ford
 * @param:
 *  index: index of the song to be played
 *
 * @date: 4/23/19
 */
void music_playSong(int index);

#endif /* MUSIC_H_ */
