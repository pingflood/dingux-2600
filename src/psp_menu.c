/*
 *  Copyright (C) 2006 Ludovic Jacomme (ludovic.jacomme@gmail.com)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>

#include "Atari.h"
#include "global.h"
#include "psp_sdl.h"
#include "psp_kbd.h"
#include "psp_menu.h"
#include "psp_fmgr.h"
#include "psp_menu_set.h"
#include "psp_menu_cheat.h"
#include "psp_menu_help.h"
#include "psp_editor.h"
#include "gp2x_psp.h"

extern SDL_Surface *back_surface;

enum {
  MENU_LOAD_SLOT,
  MENU_SAVE_SLOT,
  MENU_DEL_SLOT,
  MENU_SCREENSHOT,

  // MENU_HELP,
  // MENU_LOAD_ROM,
  // MENU_EDITOR,

  MENU_CHEATS,
  // MENU_KEYBOARD,
  // MENU_JOYSTICK,
  MENU_SETTINGS,

  MENU_RESET,
  // MENU_BACK,
  MENU_EXIT,

  MAX_MENU_ITEM
};

  static menu_item_t menu_list[] =
  {
    // { "Volume" },

    { "Load state" },
    { "Save state" },
    { "Delete state" },
    { "Save Screenshot" },

    // { "Help" }, /* dc 20130702 */
    // { "Load Rom" },    /* dc 20130702 */
    // { "Comments" },

    { "Cheats" },
    // { "Keyboard" }, 
    // { "Joystick" },
    { "Settings" },

    { "Reset Game" },
    // { "Back to Game" },
    { "Exit" }
  };

  static int cur_menu_id = 0;
  // static int cur_menu_id = MENU_LOAD_ROM; /* dc 20130702 */

  static int cur_slot    = 0;

void 
psp_menu_display_save_name()
{
  /* dc 20130702 */
  // return;

  char buffer[128];
  // int Length;
  // snprintf(buffer, 30, "Game: %s", ATARI.atari_save_name);
  snprintf(buffer, 50, "%s", ATARI.atari_save_name);
  // Length = strlen(buffer);
  // psp_sdl_back2_print(300 - (6*Length),  5, buffer, PSP_MENU_TEXT2_COLOR);
  psp_sdl_back2_print(10,  5, buffer, PSP_MENU_TEXT2_COLOR);
}

void
string_fill_with_space(char *buffer, int size)
{
  int length = strlen(buffer);
  int index;

  for (index = length; index < size; index++) {
    buffer[index] = ' ';
  }
  buffer[size] = 0;
}

void 
psp_display_screen_menu(void)
{
  char buffer[64];
  int menu_id = 0;
  int slot_id = 0;
  int color   = 0;
  int x       = 0;
  int y       = 0;
  int y_step  = 0;
  int x_step  = 0; /* dc 20130702 */

  psp_sdl_blit_background();
  
  x      = 10;
  y      = 20; /* dc 20130702 */
  y_step = 10;

  for (menu_id = 0; menu_id < MAX_MENU_ITEM; menu_id++, y += y_step) {
    color = PSP_MENU_TEXT_COLOR;
    if (cur_menu_id == menu_id) {
      color = PSP_MENU_SEL_COLOR;
      if (cur_menu_id == MENU_LOAD_SLOT) color = PSP_MENU_SEL2_COLOR;
      else if (cur_menu_id == MENU_DEL_SLOT || cur_menu_id == MENU_EXIT) color = PSP_MENU_WARNING_COLOR;
    }
    // else if (menu_id == MENU_EXIT) color = PSP_MENU_WARNING_COLOR;
    // else if (menu_id == MENU_HELP) color = PSP_MENU_GREEN_COLOR;

    // if (menu_id == MENU_EDITOR) {
    //   if (ATARI.comment_present) {
    //     psp_sdl_back2_print(x, y, "View comment", color);
    //   } else {
    //     psp_sdl_back2_print(x, y, "New comment", color);
    //   }

    // } else {
      psp_sdl_back2_print(x, y, menu_list[menu_id].title, color);
    // }


    switch (menu_id) {
        case MENU_SCREENSHOT:
            sprintf(buffer,": %d", ATARI.psp_screenshot_id);
            string_fill_with_space(buffer, 4);
            psp_sdl_back2_print(140, y, buffer, color);
        case MENU_DEL_SLOT:
        case MENU_SETTINGS:
        case MENU_RESET:
          y += y_step;
          break;
    }

     // else
    // if (menu_id == MENU_VOLUME) {
    //   sprintf(buffer,": %d", gp2xGetSoundVolume());
    //   string_fill_with_space(buffer, 4);
    //   psp_sdl_back2_print(130, y, buffer, color);
    //   y += y_step;
    // } else
    // if (menu_id == MENU_DEL_SLOT) {
    //   y += y_step;
    // } else
    // if (menu_id == MENU_SETTINGS) {
    //   y += y_step;
    // } else
    // if (menu_id == MENU_BACK) {
    //   y += y_step;
    // } else
    // if (menu_id == MENU_LOAD_ROM) {
      // y += y_step;
    // } else
    // if (menu_id == MENU_CHEATS) {
    // } else
    // if (menu_id == MENU_HELP) {y += y_step; }

    // y += y_step;
  }
 if (cur_menu_id <= MENU_DEL_SLOT) {
    y_step = 10;
    y      = 20 + cur_menu_id * y_step; /* dc 20130702 */
    x_step = 30; /* dc 20130702 */
    x      = 142; /* dc 20130702 */

    for (slot_id = 0; slot_id < ATARI_MAX_SAVE_STATE; slot_id++) {
      if (slot_id == cur_slot) {
        color = PSP_MENU_SEL2_COLOR;
        switch (cur_menu_id) {
            case MENU_SAVE_SLOT:
              color = PSP_MENU_SEL_COLOR;
              break;
            case MENU_DEL_SLOT:
              color = PSP_MENU_WARNING_COLOR;
              break;
        }
      }
      else color = PSP_MENU_TEXT_COLOR;

      if (ATARI.atari_save_state[slot_id].used) {
        sprintf(buffer, "[x]");
      } else {
        sprintf(buffer, "[ ]");
      }
      // string_fill_with_space(buffer, 32); /* dc 20130702 */
      // psp_sdl_back2_print(100, y, buffer, color); /* dc 20130702 */
      psp_sdl_back2_print(x, y, buffer, color);

      // y += y_step; /* dc 20130702 */
      x += x_step;
    }
    y += 1.5*y_step;
    x = 140;

    if (ATARI.atari_save_state[cur_slot].thumb) {
      psp_sdl_blit_thumb(x, y, ATARI.atari_save_state[cur_slot].surface);
    } 
  }
  /* dc 20130702 */
  /*else {
    psp_sdl_blit_thumb(110,70, thumb_surface);
  }*/

  psp_menu_display_save_name();
}

static void
psp_main_menu_reset(void)
{
  /* Reset ! */
  psp_display_screen_menu();
  psp_sdl_back2_print( 90, 110, "Reset Atari!", PSP_MENU_WARNING_COLOR);
  psp_sdl_flip();
  atari_emulator_reset();
  sleep(1);
}

static int
psp_main_menu_load(int format)
{
  int ret;

  ret = psp_fmgr_menu(format);
  if (ret ==  1) /* load OK */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 110, "File loaded!", 
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);
    return 1;
  }
  else 
  if (ret == -1) /* Load Error */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 110, "Can't load file!", 
                       PSP_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  return 0;
}

/* used by command keys */
int
psp_main_menu_load_current()
{
  if (ATARI.atari_save_state[cur_slot].used) {
    return atari_snapshot_load_slot(cur_slot);
  }
  return -1;
}

int
psp_main_menu_save_current()
{
  return atari_snapshot_save_slot(cur_slot);
}

static int
psp_main_menu_load_slot()
{
  int error;

  if (! ATARI.atari_save_state[cur_slot].used) {

    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 110, "Slot is empty!", 
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);

    return 0;
  }

  error = atari_snapshot_load_slot(cur_slot);

  if (! error) /* save OK */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 110, "File loaded!", 
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);

    return 1;
  }
  else 
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 110, "Can't load file!", 
                       PSP_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  return 0;
}

static void
psp_main_menu_save_slot()
{
  int error;
  error = atari_snapshot_save_slot(cur_slot);

  if (! error) /* save OK */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 110, "File saved!", 
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  else 
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 110, "Can't save file!", 
                       PSP_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
}

static void
psp_main_menu_del_slot()
{
  int error;

  if (! ATARI.atari_save_state[cur_slot].used) {

    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 110, "Slot is empty!", 
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);

    return;
  }

  error = atari_snapshot_del_slot(cur_slot);

  if (! error) /* save OK */
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 110, "File deleted!", 
                       PSP_MENU_NOTE_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
  else 
  {
    psp_display_screen_menu();
    psp_sdl_back2_print( 90, 110, "Can't delete file!", 
                       PSP_MENU_WARNING_COLOR);
    psp_sdl_flip();
    sleep(1);
  }
}

static void
psp_main_menu_cur_slot(int step)
{
  if (step == 1) {
    cur_slot++; if (cur_slot >= ATARI_MAX_SAVE_STATE) cur_slot = 0;
  } else if (step == -1) {
    cur_slot--; if (cur_slot < 0) cur_slot = ATARI_MAX_SAVE_STATE - 1;
  }
}

static void
psp_main_menu_screenshot(void)
{
  psp_screenshot_mode = 10;
}

static void
psp_main_menu_volume(int step)
{
  if (step == 1) {
    gp2xIncreaseVolume();
  } else if (step == -1) {
    gp2xDecreaseVolume();
  }
}

int
psp_main_menu_exit(void)
{
  // gp2xCtrlData c;

  // psp_display_screen_menu();
  // psp_sdl_back2_print( 90, 110, "Press B to confirm!", PSP_MENU_WARNING_COLOR);
  // psp_sdl_flip();

  // psp_kbd_wait_no_button();

  // do
  // {
  //   gp2xCtrlReadBufferPositive(&c, 1);
  //   c.Buttons &= PSP_ALL_BUTTON_MASK;

  //   if (c.Buttons & GP2X_CTRL_CROSS) {
      psp_sdl_clear_screen(0); psp_sdl_flip();
      psp_sdl_clear_screen(0); psp_sdl_flip();
      psp_sdl_clear_screen(0); psp_sdl_flip();
      psp_sdl_exit(0);
  //   }

  // } while (c.Buttons == 0);

  // psp_kbd_wait_no_button();

  return 0;
}

void
psp_main_menu_editor()
{
  char TmpFileName[MAX_PATH];
  sprintf(TmpFileName, "%s/txt/%s.txt", ATARI.atari_home_dir, ATARI.atari_save_name);
  psp_editor_menu( TmpFileName );
}

int 
psp_main_menu(void)
{
  gp2xCtrlData c;
  long        new_pad;
  long        old_pad;
  int         last_time;
  int         end_menu;

  atari_audio_pause();

  psp_kbd_wait_no_button();

  old_pad   = 0;
  last_time = 0;
  end_menu  = 0;

  while (! end_menu)
  {
    psp_display_screen_menu();
    psp_sdl_flip();

    while (1)
    {
      gp2xCtrlReadBufferPositive(&c, 1);
      c.Buttons &= PSP_ALL_BUTTON_MASK;

      if (c.Buttons) break;
    }

    new_pad = c.Buttons;

    if ((old_pad != new_pad) || ((c.TimeStamp - last_time) > PSP_MENU_MIN_TIME)) {
      last_time = c.TimeStamp;
      old_pad = new_pad;

    } else continue;

    if ((c.Buttons & GP2X_CTRL_LTRIGGER) == GP2X_CTRL_LTRIGGER)
    {
      psp_settings_menu();
      old_pad = new_pad = 0;
    }
    else if ((c.Buttons & GP2X_CTRL_RTRIGGER) == GP2X_CTRL_RTRIGGER)
    {
      psp_main_menu_reset();
      end_menu = 1;
    }
    else if ((new_pad == GP2X_CTRL_LEFT ) || (new_pad == GP2X_CTRL_RIGHT))
    {
      int step = -1;

      if (new_pad & GP2X_CTRL_RIGHT) step = 1;

      switch (cur_menu_id ) 
      {
        case MENU_LOAD_SLOT : psp_main_menu_cur_slot(step);
        break;
        case MENU_SAVE_SLOT : psp_main_menu_cur_slot(step);
        break;
        case MENU_DEL_SLOT  : psp_main_menu_cur_slot(step);
        break;
        // case MENU_VOLUME     : psp_main_menu_volume(step);
                               // old_pad = new_pad = 0;
        // break;              
      }
    }
    else if ((new_pad == GP2X_CTRL_CIRCLE))
    {

      switch (cur_menu_id) 
      {
        case MENU_LOAD_SLOT : if (psp_main_menu_load_slot()) {
                                end_menu = 1;
                              }
        break;
        case MENU_SAVE_SLOT : psp_main_menu_save_slot();
        break;
        case MENU_DEL_SLOT  : psp_main_menu_del_slot();
        break;


        // case MENU_LOAD_ROM  : if (psp_main_menu_load(FMGR_FORMAT_ROM)) {
                                // end_menu = 1;
                              // }
                              // old_pad = new_pad = 0;
        // break;              

        // case MENU_EDITOR    : psp_main_menu_editor();
                              // old_pad = new_pad = 0;
        // break;

        case MENU_CHEATS    : psp_cheat_menu();
                              old_pad = new_pad = 0;
        break;              

        // case MENU_KEYBOARD   : psp_keyboard_menu();
        //                        old_pad = new_pad = 0;
        // break;
        // case MENU_JOYSTICK   : psp_joystick_menu();
        //                        old_pad = new_pad = 0;
        // break;
        case MENU_SETTINGS   : psp_settings_menu();
                               old_pad = new_pad = 0;
        break;

        case MENU_SCREENSHOT : psp_main_menu_screenshot();
                               end_menu = 1;
        break;              

        case MENU_RESET     : psp_main_menu_reset();
                              end_menu = 1;
        break;

        // case MENU_BACK      : end_menu = 1;
        // break;

        case MENU_EXIT      : psp_main_menu_exit();
        break;

        // case MENU_HELP      : psp_help_menu();
                              // old_pad = new_pad = 0;
        // break;              
      }

    } else
    if(new_pad & GP2X_CTRL_UP) {

      if (cur_menu_id > 0) cur_menu_id--;
      else                 cur_menu_id = MAX_MENU_ITEM-1;

    } else
    if(new_pad & GP2X_CTRL_DOWN) {
      if (cur_menu_id < (MAX_MENU_ITEM-1)) cur_menu_id++;
      else                                 cur_menu_id = 0;

    } else  
    if(new_pad & GP2X_CTRL_SQUARE) {
      /* Cancel */
      end_menu = -1;
    } else 
    if((new_pad & GP2X_CTRL_CROSS) || (new_pad & GP2X_CTRL_SELECT)) {
      /* Back to ATARI */
      end_menu = 1;
    }
  }
 
  psp_kbd_wait_no_button();

  psp_sdl_clear_screen( PSP_MENU_BLACK_COLOR ); psp_sdl_flip();
  psp_sdl_clear_screen( PSP_MENU_BLACK_COLOR ); psp_sdl_flip();
  psp_sdl_clear_screen( PSP_MENU_BLACK_COLOR ); psp_sdl_flip();

  psp_sdl_clear_blit(0);

  atari_audio_resume();

  return 1;
}

