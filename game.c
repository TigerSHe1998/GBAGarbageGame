#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"
#include "images/garbage.h"
#include "images/title.h"
#include "images/win.h"

/* TODO: */
// Add any additional states you need for your app.
typedef enum {
  START,
  PLAY,
  WIN,
  LOSE,
} GBAState;

int main(void) {
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;
  // Load initial game state
  GBAState state = START;

  //create and initialize gamestate
  gameState cs;
  gameState ps;
  //initialize player data
  cs.player.size = 5;
  cs.player.row = 80;
  cs.player.col = 50;
  cs.player.color = RED;
  //initialize eatable trash data
  cs.trashNum = 1;
  cs.trashLeft = cs.trashNum;
  for (int i = 0; i < cs.trashNum; ++i) {
    cs.trashes[i].row = 65;
    cs.trashes[i].col = 140;
    cs.trashes[i].eaten = 0;
  }
  cs.score = 0;




  while (1) { //game loop
    previousButtons = currentButtons;  // Store the previous state of the buttons
    ps = cs;  //save previous gamestate
    currentButtons = BUTTONS;  // Load the current state of the buttons

    //select key to reset the whole game
    if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
      state = START;
      cs.player.size = 5;
      cs.player.row = 80;
      cs.player.col = 50;
      cs.player.color = RED;
      //initialize eatable trash data
      cs.trashNum = 1;
      for (int i = 0; i < cs.trashNum; ++i) {
        cs.trashes[i].row = 65;
        cs.trashes[i].col = 140;
        cs.trashes[i].eaten = 0;
      }
      cs.score = 0;
    }

    // code for different game states
    switch (state) {



      //title screen
      case START:

        //player movement
        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          cs.player.row--;
          if ((cs.player.row) < 0) {
            cs.player.row++;
          }
        }
        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          cs.player.row++;
          if ((cs.player.row + cs.player.size) > 160) {
            cs.player.row--;
          }
        }
        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
          cs.player.col--;
          if ((cs.player.col) < 0) {
            cs.player.col++;
          }
        }
        if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
          cs.player.col++;
          if ((cs.player.col + cs.player.size) > 240) {
            cs.player.col--;
          }
        }

        // collision detect and eat trash
        for (int i = 0; i < cs.trashNum; ++i) {
          if ((cs.player.row < cs.trashes[i].row + GARBAGE_HEIGHT)&&(cs.player.row + cs.player.size > cs.trashes[i].row)) {
            if ((cs.player.col < cs.trashes[i].col + GARBAGE_WIDTH)&&(cs.player.col + cs.player.size > cs.trashes[i].col)) {
              cs.trashes[i].eaten = 1;
            }
          }
        }

        //start screen draw
        waitForVBlank();
        drawFullScreenImageDMA(title);
        //drawCenteredString(-6,-12,240,180,"NMSL",YELLOW);
        drawRectDMA(cs.player.row,cs.player.col,cs.player.size,cs.player.size,cs.player.color);
        for (int i = 0; i < cs.trashNum; ++i) {
          if (!cs.trashes[i].eaten) {
            drawImageDMA(cs.trashes[i].row,cs.trashes[i].col,GARBAGE_WIDTH,GARBAGE_HEIGHT,garbage);
          }
        }

        //next level conditionals
        if (cs.trashes[0].eaten) {
          //initialize new gamestate
          cs.player.size = 5;
          cs.player.row = 80;
          cs.player.col = 120;
          cs.player.color = RED;
          cs.trashNum = 5;
          cs.trashLeft = cs.trashNum;
          for (int i = 0; i < cs.trashNum; ++i) {
            cs.trashes[i].row = randint(0, 160-GARBAGE_HEIGHT);
            cs.trashes[i].col = randint(0, 240-GARBAGE_WIDTH);
            cs.trashes[i].eaten = 0;
          }
          cs.score = 100;
          waitForVBlank();
          fillScreenDMA(0);
          //transfer to new game scene
          state = PLAY;
        }

        break;



      //gameplay scene
      case PLAY:

        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          cs.player.row--;
          if ((cs.player.row) < 0) {
            cs.player.row++;
          }
        }
        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          cs.player.row++;
          if ((cs.player.row + cs.player.size) > 160) {
            cs.player.row--;
          }
        }
        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
          cs.player.col--;
          if ((cs.player.col) < 0) {
            cs.player.col++;
          }
        }
        if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
          cs.player.col++;
          if ((cs.player.col + cs.player.size) > 240) {
            cs.player.col--;
          }
        }

        // collision detect
        for (int i = 0; i < cs.trashNum; ++i) {
          if ((cs.player.row < cs.trashes[i].row + GARBAGE_HEIGHT)&&(cs.player.row + cs.player.size > cs.trashes[i].row)) {
            if ((cs.player.col < cs.trashes[i].col + GARBAGE_WIDTH)&&(cs.player.col + cs.player.size > cs.trashes[i].col)) {
              cs.trashes[i].eaten = 1;
            }
          }
        }

        //generate score
        cs.score--;
        char buffer[51];
        sprintf(buffer, "SCORE: %d", cs.score);

        //wingame conditionals
        if (cs.trashLeft == 0) {
          waitForVBlank();
          fillScreenDMA(0);
          //transfer to new game scene
          state = WIN;
        }

        //losegame conditionals
        if (cs.score == 0) {
          waitForVBlank();
          fillScreenDMA(0);
          state = LOSE;
        }

        waitForVBlank();
        drawRectDMA(152, 0, 60, 10, RED);
        drawString(152,0,buffer,YELLOW);
        drawRectDMA(ps.player.row,ps.player.col,ps.player.size,ps.player.size,BLACK);
        drawRectDMA(cs.player.row,cs.player.col,cs.player.size,cs.player.size,cs.player.color);
        for (int i = 0; i < cs.trashNum; ++i) {
          if (!cs.trashes[i].eaten) {
            drawImageDMA(cs.trashes[i].row,cs.trashes[i].col,GARBAGE_WIDTH,GARBAGE_HEIGHT,garbage);
          }
          if (!ps.trashes[i].eaten && cs.trashes[i].eaten) {
            drawRectDMA(cs.trashes[i].row,cs.trashes[i].col,GARBAGE_WIDTH,GARBAGE_HEIGHT,BLACK);
            cs.score += 50;
            cs.trashLeft -= 1;
          }
        }
        // state = ?
        break;



      // win game scene
      case WIN:
        waitForVBlank();
        drawFullScreenImageDMA(win);
        break;



      // lose game scene
      case LOSE:
        waitForVBlank();
        drawCenteredString(-10,0,240,160,"YOU RAN OUT OF SCORE!",RED);
        drawCenteredString(10,0,240,160,"NO REWARD FOR YOU :(",RED);
        break;
    }
  }

  UNUSED(previousButtons);  // You can remove this once previousButtons is used
  return 0;
}


