#ifndef GAME_H
#define GAME_H

#include "gba.h"

                    /* TODO: */

            // Create any necessary structs //


/*
* For example, for a Snake game, one could be:
*
* typedef struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* } Snake;
*
*
*
*
*
* Example of a struct to hold state machine data:
*
* typedef struct state {
*   int currentState;
*   int nextState;
* } State
*
*/

typedef struct playerBlock {
    int size;
    int row;
    int col;
    u16 color;
} playerBlock;

typedef struct trash {
    int row;
    int col;
    int eaten;
} trash;

typedef struct gameState {
    struct playerBlock player;
    int trashNum;
    struct trash trashes[6];
    int trashLeft;
    int score;
} gameState;



#endif
