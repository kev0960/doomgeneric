#include <stdint.h>
#include <syscall.h>
#include <string.h>
#include <stdio.h>
#include "doomgeneric.h"
#include "doomkeys.h"

static unsigned char convert_to_doom_key(unsigned int key) {
  switch (key) {
    case 0:
      key = KEY_ENTER;
      break;
    case 0x110:
      key = KEY_ESCAPE;
      break;
    case 0x11c:
      key = KEY_LEFTARROW;
      break;
    case 0x11e:
      key = KEY_RIGHTARROW;
      break;
    case 0x119:
      key = KEY_UPARROW;
      break;
    case 0x121:
      key = KEY_DOWNARROW;
      break;
    case 0x105:  // CTRL
      key = KEY_FIRE;
      break;
    case 0x20:  // Space
      key = KEY_USE;
      break;
    case 0x103:
    case 0x104:
      key = KEY_RSHIFT;
      break;
    default:
      break;
  }

  return key;
}

void DG_Init() {
  // Make console to forward response without buffering.
  // Also it will forward any characters (including non-ascii).
  console(SET_NO_BUFFER | SET_EVERYTHING | SET_NON_BLOCKING_IO | SET_RECORD_UP);
}

void DG_DrawFrame() {
  struct FrameBufferInfo buffer_info;
  buffer_info.screen_row = 0;
  buffer_info.screen_col = 0;
  buffer_info.buffer_width = DOOMGENERIC_RESX;
  buffer_info.buffer_height = DOOMGENERIC_RESY;

  screen(COPY_FRAME_BUFFER, DG_ScreenBuffer, &buffer_info);
}

int DG_GetKey(int* pressed, unsigned char* doomKey) {
  char buf[10];
  int cnt = read(0, buf, 10);
  if (cnt == 0) {
    return 0;
  }

  buf[cnt] = 0;

  if (buf[0] == 1) {  // UP
    *pressed = 0;
  } else if (buf[0] == 2) {
    *pressed = 1;
  }

  int unicode;
  utf8_str_to_unicode_num(buf + 1, &unicode);
  *doomKey = convert_to_doom_key(unicode);
}

void DG_SleepMs(uint32_t ms) {
  usleep(ms * 1000);
}

uint32_t DG_GetTicksMs() {
  return mstick();
}

void DG_SetWindowTitle(const char * title) {
  (void)(title);
}
